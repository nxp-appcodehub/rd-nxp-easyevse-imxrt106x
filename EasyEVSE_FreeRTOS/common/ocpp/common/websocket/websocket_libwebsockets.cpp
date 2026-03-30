// SPDX-License-Identifier: Apache-2.0
// Copyright 2020 - 2023 Pionix GmbH and Contributors to EVerest
// Copyright 2025-2026 NXP

#include <ocpp/common/websocket/websocket_libwebsockets.hpp>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <fstream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>

extern "C" {
#include <libwebsockets.h>

#include "lws-write.h"
#include "lws-client.h"
#include "lws-http.h"
#include "x509_openssl.h"
#include "ssl_types.h"
#include "lws-service.h"
#include "lws-writeable.h"
#include "lws-http.h"
#include "csms_ca.h"
#include "cs_cert.h"
}

using namespace std::chrono_literals;

namespace {
std::optional<std::filesystem::path> keylog_file;

void keylog_callback(const SSL* ssl, const char* line) {
#if EASYEVSE
	return;
#else
    if (keylog_file.has_value()) {
        std::ofstream keylog_ofs;
        keylog_ofs.open(keylog_file.value(), std::ofstream::out | std::ofstream::app);
        keylog_ofs << line << std::endl;
        keylog_ofs.close();
    }
#endif /* EASYEVSE */
}
} // namespace


template <> class std::default_delete<lws_context> {
public:
    void operator()(lws_context* ptr) const {
        ::lws_context_destroy(ptr);
    }
};

namespace ocpp {

enum class EConnectionState {
    INITIALIZE,   ///< Initialization state
    CONNECTING,   ///< Trying to connect
    RECONNECTING, ///< After the first connect attempt, we'll change the state to reconnecting
    CONNECTED,    ///< Successfully connected
    ERROR,        ///< We couldn't connect, but we will try again soon internally
    FINALIZED,    ///< We finalized the connection and we're never going to connect again
};

/// \brief Message to return in the callback to close the socket connection
static constexpr int LWS_CLOSE_SOCKET_RESPONSE_MESSAGE = -1;

/// \brief How much we wait for a message to be sent in seconds
static constexpr int MESSAGE_SEND_TIMEOUT_S = 1;

/// \brief Current connection data, sets the internal state of the
struct ConnectionData {
    explicit ConnectionData(WebsocketLibwebsockets* owner) :
        wsi(nullptr), owner(owner), is_running(true), is_stopped_run(false), state(EConnectionState::INITIALIZE) {
    }

    ~ConnectionData() {
        state = EConnectionState::FINALIZED;
        is_running = false;
        wsi = nullptr;
        owner = nullptr;
    }

    void bind_thread_client(std::thread::id id) {
        std::lock_guard lock(this->mutex);
        //websocket_client_thread_id = id;
        websocket_client_thread_id = std::move(id);
    }

    void bind_thread_message(std::thread::id id) {
        std::lock_guard lock(this->mutex);
        //websocket_recv_thread_id = id;
        websocket_recv_thread_id = std::move(id);
    }

    std::thread::id get_client_thread_id() {
        std::lock_guard lock(this->mutex);
        return websocket_client_thread_id;
    }

    std::thread::id get_message_thread_id() {
        std::lock_guard lock(this->mutex);
        return websocket_recv_thread_id;
    }

    void update_state(EConnectionState in_state) {
        std::lock_guard lock(this->mutex);
        state = in_state;
    }

    EConnectionState get_state() {
        std::lock_guard lock(this->mutex);
        return state;
    }

    /// \brief Requests an active connection to awake from 'poll' and process again
    void request_awake() {
        if (std::this_thread::get_id() == this->websocket_client_thread_id) {
            throw std::runtime_error("Attempted to awake connection from websocket thread!");
        }

        std::lock_guard lock(this->mutex);

        if (lws_ctx) {
            lws_cancel_service(lws_ctx.get());
        }
    }

    /// \brief Requests the threads that are processing to exit as soon as possible
    /// in a ordered manner
    void do_interrupt_and_exit() {
        if (std::this_thread::get_id() == this->websocket_client_thread_id) {
            throw std::runtime_error("Attempted to interrupt connection from websocket thread!");
        }

        std::lock_guard lock(this->mutex);

        if (is_running) {
            is_running = false;

            // Notify if we are on a different thread
            if (lws_ctx) {
                // Attempt to revive the running thread
                lws_cancel_service(lws_ctx.get());
            }
        }
    }

    bool is_interupted() {
        std::lock_guard lock(this->mutex);
        return (is_running == false);
    }

    void mark_stop_executed() {
        std::lock_guard lock(this->mutex);
        this->is_stopped_run = true;
    }

    bool is_stop_executed() {
        std::lock_guard lock(this->mutex);
        return this->is_stopped_run;
    }

public:
    /// \brief This should be used for a cleanup before calling the
    ///        init functions because releasing the unique ptrs has
    ///        as an effect the invocation of 'callback_minimal' during
    ///        '::lws_context_destroy(ptr);' and that causes a deadlock
    void reset_connection_data() {
        // Destroy them outside the lock scope
        std::unique_ptr<lws_context> clear_lws;

        {
            std::lock_guard lock(this->mutex);
            this->wsi = nullptr;

            if (this->lws_ctx) {
                this->lws_ctx.swap(clear_lws);
            }
        }
    }

    void init_connection_context(lws_context* lws_ctx) {

        std::lock_guard lock(this->mutex);

        if (this->lws_ctx) {
            throw std::runtime_error("Cleanup must be called before re-initing a connection!");
        }

        // Reset the close status
        is_stopped_run = false;

        // Causes a deadlock in callback_minimal if not reset
        this->lws_ctx = std::unique_ptr<lws_context>(lws_ctx);
    }

    void init_connection(lws* lws) {
        std::lock_guard lock(this->mutex);
        this->wsi = lws;
    }

    lws* get_conn() {
        std::lock_guard lock(this->mutex);
        return wsi;
    }

    lws_context* get_context() {
        std::lock_guard lock(this->mutex);
        return lws_ctx.get();
    }

    // No need for sync here since its set on construction
    WebsocketLibwebsockets* get_owner() {
        return owner;
    }

private:
    // libwebsockets state
    std::unique_ptr<lws_context> lws_ctx;
    // Internal used WSI
    lws* wsi;
    // Owner, set on creation
    WebsocketLibwebsockets* owner;

    // State variables
    bool is_running;
    bool is_stopped_run;
    EConnectionState state;

    std::mutex mutex;

    /// \brief Websocket client thread ID
    std::thread::id websocket_client_thread_id;
    /// \brief Websocket received message thread ID
    std::thread::id websocket_recv_thread_id;

    // Required for access of state
    friend class WebsocketLibwebsockets;
};

struct WebsocketMessage {
    WebsocketMessage() : sent_bytes(0), message_sent(false) {
    }

public:
    std::string payload;
    lws_write_protocol protocol;

    // How many bytes we have sent to libwebsockets, does not
    // necessarily mean that all bytes have been sent over the wire,
    // just that these were sent to libwebsockets
    size_t sent_bytes;
    // If libwebsockets has sent all the bytes through the wire
    std::atomic_bool message_sent;
};


WebsocketLibwebsockets::WebsocketLibwebsockets(const WebsocketConnectionOptions& connection_options,
                                               std::shared_ptr<EvseSecurity> evse_security) :
    WebsocketBase(),
    evse_security(evse_security),
    stop_deferred_handler(false),
    connected_ocpp_version{OcppProtocolVersion::Unknown} {

    set_connection_options(connection_options);

    configPRINTF(("Initialised WebsocketLibwebsockets with URI: %s", this->connection_options.csms_uri.string().c_str()));
}

WebsocketLibwebsockets::~WebsocketLibwebsockets() {
    std::lock_guard lock(this->connection_mutex);

    std::shared_ptr<ConnectionData> local_conn_data = conn_data;
    if (local_conn_data != nullptr) {
        auto tid = std::this_thread::get_id();

        if (tid == local_conn_data->get_client_thread_id() || tid == local_conn_data->get_message_thread_id()) {
            configPRINTF(("Trying to destruct websocket from utility thread!"));
            std::terminate();
        }
    }

    if (this->m_is_connected || is_trying_to_connect_internal()) {
        this->close_internal(WebsocketCloseReason::Normal, "websocket destructor");
    }

    // In the dtor we must make sure the deferred callback thread
    // finishes since the callbacks capture a reference to 'this'
    if (this->deferred_callback_thread != nullptr && this->deferred_callback_thread->joinable()) {
        this->stop_deferred_handler.store(true);
        this->deferred_callback_queue.notify_waiting_thread();

        this->deferred_callback_thread->join();
    }
}

void WebsocketLibwebsockets::set_connection_options(const WebsocketConnectionOptions& connection_options) {
    switch (connection_options.security_profile) { // `switch` used to lint on missing enum-values
    case security::SecurityProfile::OCPP_1_6_ONLY_UNSECURED_TRANSPORT_WITHOUT_BASIC_AUTHENTICATION:
    case security::SecurityProfile::UNSECURED_TRANSPORT_WITH_BASIC_AUTHENTICATION:
    case security::SecurityProfile::TLS_WITH_BASIC_AUTHENTICATION:
    case security::SecurityProfile::TLS_WITH_CLIENT_SIDE_CERTIFICATES:
        break;
    default:
        throw std::invalid_argument("unknown `security_profile`, value = " +
                                    std::to_string(connection_options.security_profile));
    }

    if (connection_options.ocpp_versions.empty()) {
        throw std::invalid_argument("Connection options must contain at least 1 option");
    }

    if (std::any_of(connection_options.ocpp_versions.begin(), connection_options.ocpp_versions.end(),
                    [](OcppProtocolVersion version) { return version == OcppProtocolVersion::Unknown; })) {
        throw std::invalid_argument("Ocpp_versions may not contain 'Unknown'");
    }

    set_connection_options_base(connection_options);

    // Set secure URI only if it is in TLS mode
    if (connection_options.security_profile >
        security::SecurityProfile::UNSECURED_TRANSPORT_WITH_BASIC_AUTHENTICATION) {
        this->connection_options.csms_uri.set_secure(true);
    }

    this->connection_attempts = 1; // reset connection attempts
}

static int callback_minimal(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len) {
    // Get user safely, since on some callbacks (void *user) can be different than what we set
    if (wsi != nullptr) {
    	configPRINTF(("callback_minimal called with valid wsi"));
        if (ConnectionData* data = reinterpret_cast<ConnectionData*>(lws_wsi_user(wsi)))
    	{
            auto* owner = data->get_owner();
            if (owner not_eq nullptr) {
                return owner->process_callback(wsi, static_cast<int>(reason), user, in, len);
            } else {
                configPRINTF(("callback_minimal called, but data->owner is nullptr. Reason: %d", reason));
            }
        }
    } else {
    	configPRINTF(("callback_minimal called with wsi NULL"));
    }

    return 0;
}

static int private_key_callback(char* buf, int size, int rwflag, void* userdata) {
    const auto* password = static_cast<const std::string*>(userdata);
    const std::size_t max_pass_len = (size - 1); // we exclude the endline
    const std::size_t max_copy_chars =
        std::min(max_pass_len, password->length()); // truncate if pass is too large and buffer too small

    std::memset(buf, 0, size);
    std::memcpy(buf, password->c_str(), max_copy_chars);

    return max_copy_chars;
}

constexpr auto local_protocol_name = "lws-everest-client";
static const struct lws_protocols protocols[] = {{local_protocol_name, callback_minimal, 256, 0, 0, NULL, 0},
                                                 LWS_PROTOCOL_LIST_TERM};

bool WebsocketLibwebsockets::tls_init(SSL_CTX* ctx, const std::string& path_chain, const std::string& path_key,
                                      bool custom_key, std::optional<std::string>& password) {
	    return true;
}

void WebsocketLibwebsockets::thread_websocket_message_recv_loop(std::shared_ptr<ConnectionData> local_data) {
    if (local_data == nullptr) {
        throw std::runtime_error("Null 'ConnectionData' in message thread, fatal error!");
    }

    configPRINTF(("Init recv loop with ID: %d", std::this_thread::get_id()));

    while (!local_data->is_interupted()) {
        // Process all messages
        while (true) {
            std::string message{};

            {
                if (recv_message_queue.empty())
                    break;

                message = recv_message_queue.pop();
            }

            // Invoke our processing callback, that might trigger a send back that
            // can cause a deadlock if is not managed on a different thread
            this->message_callback(message);
        }

        // While we are empty, sleep, only if we have not been interrupted in the
        // message_callback. An interrupt can be caused in the message callback
        // if we receive a certain message type that will cause the implementation
        // in the charge point to attempt a reconnect (BasicAuthPass for example)
        if (!local_data->is_interupted()) {
            recv_message_queue.wait_on_queue_element(1s);
        }
    }

    configPRINTF(("Exit recv loop with ID: %d", std::this_thread::get_id()));
}

extern "C"{

extern void vLoggingPrintf(const char *pcFormatString, ...);

void lws_log_wrapper(int level, const char *line)
{
    vLoggingPrintf("%s", line);
}

}

bool WebsocketLibwebsockets::initialize_connection_options(std::shared_ptr<ConnectionData>& local_data) {
//     lws_set_log_level(LLL_ERR | LLL_WARN | LLL_NOTICE | LLL_INFO | LLL_DEBUG | LLL_PARSER | LLL_HEADER | LLL_EXT |
//                              LLL_CLIENT | LLL_LATENCY | LLL_THREAD | LLL_USER, nullptr);
    lws_set_log_level(LLL_ERR, lws_log_wrapper);

    lws_context_creation_info info;
    memset(&info, 0, sizeof(lws_context_creation_info));

    info.options = LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
    info.port = CONTEXT_PORT_NO_LISTEN; /* we do not run any server */
    info.protocols = protocols;

    if (this->connection_options.iface.has_value()) {
        configPRINTF(("Using network iface: %s", this->connection_options.iface.value().c_str()));

        info.iface = this->connection_options.iface.value().c_str();
        info.bind_iface = 1;
    }

    // Set reference to ConnectionData since 'data' can go away in the websocket
    info.user = local_data.get();
    info.fd_limit_per_thread = MEMP_NUM_NETCONN;

    // Lifetime of this is important since we use the data from this in private_key_callback()
    std::string path_key;
    std::string path_chain;
    std::optional<std::string> private_key_password;

    if (this->connection_options.security_profile == 2 || this->connection_options.security_profile == 3) {
    	// Set ciphers
    	info.ssl_cipher_list = this->connection_options.supported_ciphers_12.c_str();
        info.client_ssl_cipher_list = this->connection_options.supported_ciphers_12.c_str();

    	// Set client certificates (if needed)
        if (this->connection_options.security_profile == 3) {

            /* Parse the leaf client certificate to get password */
            info.client_ssl_private_key_password = NULL;

            info.client_ssl_cert_filepath = NULL;
            info.client_ssl_cert_mem = &ocpp_client_leaf_cert[0];
            info.client_ssl_cert_mem_len = ocpp_client_leaf_cert_len;

            if (strlen(ocpp_client_leaf_key_filepath) > 0)
            {
                info.client_ssl_private_key_filepath = ocpp_client_leaf_key_filepath;
                info.client_ssl_key_mem = NULL;
                info.client_ssl_key_mem_len = 0;
                configPRINTF(("Using SE key: %s", ocpp_client_leaf_key_filepath));
            }
            else
            {
                info.client_ssl_private_key_filepath = NULL;
                info.client_ssl_key_mem = &ocpp_client_leaf_key[0];
                info.client_ssl_key_mem_len = ocpp_client_leaf_key_len;
                configPRINTF(("Using software key from memory"));
            }
        }

        // CA certificate from buffer
        if (this->evse_security->is_ca_certificate_installed(ocpp::CaCertificateType::CSMS)) {
            info.client_ssl_ca_filepath = NULL;
            info.client_ssl_ca_mem = &ocpp_root_ca_cert[0];
            info.client_ssl_ca_mem_len = ocpp_root_ca_cert_len;
        }
        else
        {
            configPRINTF(("CA cert appears not to be installed!"));
        }

        // Default verify paths
        if (this->connection_options.use_ssl_default_verify_paths) {
            //info.options |= LWS_SERVER_OPTION_SSL_ECDH;
        }

        info.mbedtls_client_preload_filepath = NULL;
    }

    lws_context* lws_ctx = lws_create_context(&info);
    if (nullptr == lws_ctx) {
        configPRINTF(("lws create context failed\r\n"));
        return false;
    }

    // Conn acquire the lws context and security context
    local_data->init_connection_context(lws_ctx);

    return true;
}

void WebsocketLibwebsockets::thread_websocket_client_loop(std::shared_ptr<ConnectionData> local_data) {
    if (local_data == nullptr) {
        throw std::runtime_error("Null 'ConnectionData' in client thread, fatal error!");
    }

    //configPRINTF(("in thread_websocket_client_loop, this is: %p and this->conn is: %p", this, local_data));

      configPRINTF(("Init client loop with ID: %d", std::this_thread::get_id()));
    bool try_reconnect = true;

    do {
        if (!initialize_connection_options(local_data)) {
            configPRINTF(("Could not initialize connection options.\r\n"));

            local_data->update_state(EConnectionState::ERROR);
            on_conn_fail(local_data.get());
        } else {
            lws_client_connect_info i;
            memset(&i, 0, sizeof(lws_client_connect_info));

            // No SSL
            int ssl_connection = 0;

            if (this->connection_options.security_profile == 2 || this->connection_options.security_profile == 3) {
                ssl_connection = LCCSCF_USE_SSL;

                // Skip server hostname check
                if (this->connection_options.verify_csms_common_name == false) {
                    ssl_connection |= LCCSCF_SKIP_SERVER_CERT_HOSTNAME_CHECK;
                }

                // Debugging if required
                // ssl_connection |= LCCSCF_ALLOW_SELFSIGNED;
                // ssl_connection |= LCCSCF_ALLOW_INSECURE;
                // ssl_connection |= LCCSCF_ALLOW_EXPIRED;
            }

            auto& uri = this->connection_options.csms_uri;
            lws* local_lws = nullptr;

            std::string ocpp_versions;
            bool first = true;
            for (auto version : this->connection_options.ocpp_versions) {
                if (!first) {
                    ocpp_versions += ", ";
                }
                first = false;
                ocpp_versions += conversions::ocpp_protocol_version_to_string(version);
            }

            // TODO: No idea who releases the strdup?
            i.context = local_data->lws_ctx.get();
            i.port = uri.get_port();
            i.address = strdup(uri.get_hostname().c_str()); // Base address, as resolved by getnameinfo
            i.path = strdup((uri.get_path() + uri.get_chargepoint_id()).c_str()); // Path of resource : "/cp001"
            i.host = i.address;
            i.origin = i.address;
            i.ssl_connection = ssl_connection;
            i.protocol = strdup(ocpp_versions.c_str());
            i.local_protocol_name = local_protocol_name;
            i.pwsi = &local_lws; // Will set the local_data->wsi to a valid value in case of a successful connect
            i.userdata = local_data.get(); // See lws_context 'user'

            if (this->connection_options.iface.has_value()) {
                i.iface = this->connection_options.iface.value().c_str();
            }

            // Print data for debug
            configPRINTF(("LWS connect with info port: %d address: %s path: %s protocol: %s security profile: %d",
                       i.port, i.address, i.path, i.protocol, this->connection_options.security_profile));

            if (lws_client_connect_via_info(&i) == nullptr) {
                configPRINTF(("LWS connect failed!\r\n"));
                // This condition can occur when connecting fails to an IP address
                // retries need to be attempted
                local_data->update_state(EConnectionState::ERROR);
                on_conn_fail(local_data.get());
            } else {
                local_data->init_connection(local_lws);

                // Process while we're running
                int n = 0;
                bool processing = true;

                do {
                    // We can grab the 'state' and 'lws_ctx' members here since we're only
                    // setting them from this thread and not from the exterior

                    // Set to -1 for continuous servicing, if required, not recommended
                    n = lws_service(local_data->lws_ctx.get(), 0);

                    auto state = local_data->state;
                    processing = (!local_data->is_interupted()) &&
                                 (state != EConnectionState::FINALIZED && state != EConnectionState::ERROR);

                    if (processing && !message_queue.empty()) {
                        lws_callback_on_writable(local_data->get_conn());
                    }
                } while (n >= 0 && processing);
            }
            // After this point no minimal_callback can be called, we have finished
            // using the connection information and we will recreate it if required
            local_data->reset_connection_data();
        } // End init connection

        long reconnect_delay = 0;

        if (local_data->is_interupted() || local_data->get_state() == EConnectionState::FINALIZED) {
            configPRINTF(("Connection interrupted or cleanly finalized, exiting websocket loop"));
            try_reconnect = false;
        } else if (local_data->get_state() != EConnectionState::CONNECTED) {
            // Any other failure than a successful connect

            // -1 indicates to always attempt to reconnect
            if (this->connection_options.max_connection_attempts == -1 or
                this->connection_attempts <= this->connection_options.max_connection_attempts) {
                local_data->update_state(EConnectionState::RECONNECTING);
                reconnect_delay = this->get_reconnect_interval();
                try_reconnect = true;

                // Increment reconn attempts
                this->connection_attempts += 1;

                configPRINTF(("Connection not successful, attempting internal reconnect in: %d ms", reconnect_delay));
            } else {
                local_data->update_state(EConnectionState::FINALIZED);
                try_reconnect = false;

                configPRINTF(("Connection reconnect attempts exhausted, exiting websocket loop, passing back control to the application logic"));
            }
        }

        // Wait until new connection attempt
        if (local_data->get_state() == EConnectionState::RECONNECTING) {
            auto end_time = std::chrono::steady_clock::now() + std::chrono::milliseconds(reconnect_delay);

            while ((std::chrono::steady_clock::now() < end_time) && (false == local_data->is_interupted())) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
//
//            long delay = reconnect_delay;
//            while ((delay > 0) && (false == local_data->is_interupted()))
//            {
//            	std::this_thread::sleep_for(std::chrono::milliseconds(100));
//            	delay -= 100;
//            }

            //std::this_thread::sleep_for(std::chrono::milliseconds(reconnect_delay));

            if (true == local_data->is_interupted()) {
                try_reconnect = false;
                configPRINTF(("Interrupred reconnect attempt, not reconnecting!"));
            } else {
                configPRINTF(("Attempting reconnect after a wait of: %d ms", reconnect_delay));
            }
        }
    } while (try_reconnect); // End trying to connect

    // Give back control to the application
    if (false == local_data->is_stop_executed()) {
        this->push_deferred_callback([this]() {
            if (this->stopped_connecting_callback) {
                this->stopped_connecting_callback(WebsocketCloseReason::Normal);
            } else {
                configPRINTF(("Stopped connecting callback not registered!"));
            }

            if (this->disconnected_callback) {
                this->disconnected_callback();
            } else {
                configPRINTF(("Disconnected callback not registered!"));
            }
        });
    }

    while (!local_data->is_interupted())
    {
    	std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Client loop finished for our tid
    configPRINTF(("Exit websocket client loop with ID: %d", std::this_thread::get_id()));

}

void WebsocketLibwebsockets::clear_all_queues() {
    this->message_queue.clear();
    this->recv_buffered_message.clear();
    this->recv_message_queue.clear();
}

void WebsocketLibwebsockets::safe_close_threads() {
    // If we already have a connection attempt started for now shut it down first
    std::shared_ptr<ConnectionData> local_conn_data = conn_data;
    bool in_message_thread = false;

    if (local_conn_data != nullptr) {
        auto tid = std::this_thread::get_id();

        if (tid == local_conn_data->get_client_thread_id()) {
             throw std::runtime_error("Trying to start/stop/reconnect from client thread!");
        }

        // TODO(ioan): reintroduce this check after we have solved the problem of a main processing
        // loop in the libocpp. The problem is that a start/stop operation is executed from the
        // message thread, because in message_callback we can initiate a reconnect when we receive
        // a message of the type 'BasicAuthPass'. In turn, because there's no main processing
        // loop with it's own thread, the response is polled immediately, and there's special behavior
        // that requires a reconnect that is called from the message thread.
        // The resulting problem is that we could have this thread dangling for a bit, since we are forced to detach

        // else if (std::this_thread::get_id() == local_conn_data->get_message_thread_id()) {
        //      EVLOG_AND_THROW(std::runtime_error("Trying to start/stop/reconnect connection from message thread!")); }

        in_message_thread = (tid == local_conn_data->get_message_thread_id());
        local_conn_data->do_interrupt_and_exit();
    }

    // Clear any pending outgoing/incoming messages on a new connection
    clear_all_queues();

    // Wait old thread for a clean state
    if (this->websocket_thread && this->websocket_thread->joinable()) {
        // Awake libwebsockets thread to quickly exit
        request_write();
        this->websocket_thread->join();
        this->websocket_thread.reset();
    }

    if (in_message_thread) {
        if (this->recv_message_thread) {
            // See the note above 'in_message_thread' on why we detach
            this->recv_message_thread->detach();
            this->recv_message_thread.reset();
        }
    } else {
        if (this->recv_message_thread && this->recv_message_thread->joinable()) {
            // Awake the receiving message thread to finish
            this->recv_message_thread->join();
            this->recv_message_thread.reset();
        }
    }
}

bool WebsocketLibwebsockets::is_trying_to_connect() {
    std::lock_guard lock(this->connection_mutex);
    return is_trying_to_connect_internal();
}

bool WebsocketLibwebsockets::is_trying_to_connect_internal() {
    std::shared_ptr<ConnectionData> local_conn_data = conn_data;
    return (local_conn_data != nullptr) && (local_conn_data->get_state() != EConnectionState::FINALIZED);
}

// Will be called from external threads as well
bool WebsocketLibwebsockets::start_connecting() {

    std::lock_guard lock(this->connection_mutex);

    if (!this->initialized()) {
        configPRINTF(("Websocket not properly initialized. A reconnect attempt will not be made."));
        return false;
    }

    // Clear shutting down so we allow to reconnect again as well
    this->shutting_down = false;

    configPRINTF(("Starting connection attempts to uri: %s with security-profile %d", this->connection_options.csms_uri.string().c_str(),
    		this->connection_options.security_profile));

    this->connected_ocpp_version = OcppProtocolVersion::Unknown;

    // If we already have a connection attempt started for now shut it down first
    safe_close_threads();

    // Create a new connection data (only created here, owner never changes)
    conn_data = std::make_shared<ConnectionData>(this);

    // Stop any pending reconnect timer
    {
        std::lock_guard<std::mutex> lk(this->reconnect_mutex);
        this->reconnect_timer_tpm.stop();
    }

    this->connection_attempts = 1; // reset connection attempts

    // This should always be running, start it only once
    if (this->deferred_callback_thread == nullptr) {
        this->deferred_callback_thread =
            std::make_unique<std::thread>(&WebsocketLibwebsockets::thread_deferred_callback_queue, this);
    }

    //configPRINTF(("before websocket_thread, this is: %p and this->conn is: %p", this, this->conn_data));
    // Release other threads
    this->websocket_thread =
        std::make_unique<std::thread>(&WebsocketLibwebsockets::thread_websocket_client_loop, this, this->conn_data);

    // TODO(ioan): remove this thread when the fix will be moved into 'MessageQueue'
    // The reason for having a received message processing thread is that because
    // if we dispatch a message receive from the client_loop thread, then the callback
    // will send back another message, and since we're waiting for that message to be
    // sent over the wire on the client_loop, not giving the opportunity to the loop to
    // advance we will have a dead-lock
    this->recv_message_thread = std::make_unique<std::thread>(
        &WebsocketLibwebsockets::thread_websocket_message_recv_loop, this, this->conn_data);

    // Bind threads for various checks
    this->conn_data->bind_thread_client(this->websocket_thread->get_id());
    this->conn_data->bind_thread_message(this->recv_message_thread->get_id());

    return true;
}

void WebsocketLibwebsockets::close(const WebsocketCloseReason code, const std::string& reason) {
    std::lock_guard lock(this->connection_mutex);
    close_internal(code, reason);
}

void WebsocketLibwebsockets::close_internal(const WebsocketCloseReason code, const std::string& reason) {
    bool trying_connecting = is_trying_to_connect_internal() || this->m_is_connected;

    if (!trying_connecting) {
        configPRINTF(("Trying to close inactive websocket with code: %d and reason: %s, returning", (int)code, reason.c_str()));
        return;
    }

    configPRINTF(("Closing websocket with code: %d and reason: %s", (int)code, reason.c_str()));

    // Close any ongoing thread
    safe_close_threads();

    // Release the connection data and state
    conn_data.reset();
    this->m_is_connected = false;
    {
        std::lock_guard<std::mutex> lk(this->reconnect_mutex);
        this->reconnect_timer_tpm.stop();
    }
}

void WebsocketLibwebsockets::reconnect(long delay) {
    std::lock_guard lock(this->connection_mutex);

    if (this->shutting_down) {
        configPRINTF(("Not reconnecting because the websocket is being shutdown."));
        return;
    }

    if (this->m_is_connected || is_trying_to_connect_internal()) {
        this->close_internal(WebsocketCloseReason::AbnormalClose, "before manually reconnecting");
    }

    configPRINTF(("Externally called reconnect in: %d ms", delay));

    std::lock_guard<std::mutex> lk(this->reconnect_mutex);

    this->reconnect_timer_tpm.timeout(
        [this]() {
            // close connection before reconnecting
            if (this->m_is_connected || is_trying_to_connect()) {
                this->close(WebsocketCloseReason::AbnormalClose, "before manually reconnecting");
            }

            this->start_connecting();
        },
        std::chrono::milliseconds(delay));
}

static bool send_internal(lws* wsi, WebsocketMessage* msg) {
    static std::vector<char> buff;

    std::string& message = msg->payload;
    size_t message_len = message.length();
    size_t buff_req_size = message_len + LWS_PRE;

    if (buff.size() < buff_req_size)
        buff.resize(buff_req_size);

    // Copy data in send buffer
    memcpy(&buff[LWS_PRE], message.data(), message_len);

    // TODO (ioan): if we require certain sending over the wire,
    // we'll have to send chunked manually something like this:

    // Ethernet MTU: ~= 1500bytes
    // size_t BUFF_SIZE = (MTU * 2);
    // char *buff = alloca(LWS_PRE + BUFF_SIZE);
    // memcpy(buff, message.data() + already_written, BUFF_SIZE);
    // int flags = lws_write_ws_flags(proto, is_start, is_end);
    // already_written += lws_write(wsi, buff + LWS_PRE, BUFF_SIZE - LWS_PRE, flags);

    auto sent = lws_write(wsi, reinterpret_cast<unsigned char*>(&buff[LWS_PRE]), message_len, msg->protocol);

    if (sent < 0) {
        // Fatal error, conn closed
        configPRINTF(("Error sending message over TLS websocket, conn closed."));
        msg->sent_bytes = 0;
        return false;
    }

    // Even if we have written all the bytes to lws, it doesn't mean that it has been sent over
    // the wire. According to the function comment (lws_write), until everything has been
    // sent, the 'LWS_CALLBACK_CLIENT_WRITEABLE' callback will be suppressed. When we received
    // another callback, it means that everything was sent and that we can mark the message
    // as certainly 'sent' over the wire
    msg->sent_bytes = sent;

    if (static_cast<size_t>(sent) < message_len) {
        configPRINTF(("Error sending message over TLS websocket. Sent bytes: %d Total to send: %d", sent, message_len));
        return false;
    }

    return true;
}

void WebsocketLibwebsockets::request_write() {
    if (this->m_is_connected) {
        std::shared_ptr<ConnectionData> local_data = conn_data;

        if (local_data != nullptr) {
            // Notify waiting processing thread to wake up. According to docs
            // it is ok  to call from another thread.
            local_data->request_awake();
        }
    } else {
        configPRINTF(("Requested write with offline TLS websocket!"));
    }
}

void WebsocketLibwebsockets::poll_message(const std::shared_ptr<WebsocketMessage>& msg) {
    if (this->m_is_connected == false) {
        configPRINTF(("Trying to poll message without being connected!"));
        return;
    }

    std::shared_ptr<ConnectionData> local_data = conn_data;
    if (local_data != nullptr) {
        auto cd_tid = local_data->get_client_thread_id();

        if (std::this_thread::get_id() == cd_tid) {
            throw std::runtime_error("Deadlock detected, polling send from client lws thread!");
        }

        // If we are interupted or finalized
        if (local_data->is_interupted() || local_data->get_state() == EConnectionState::FINALIZED) {
            configPRINTF(("Trying to poll message to interrupted/finalized state!"));
            return;
        }
    }

    configPRINTF(("Queueing message over TLS websocket: %s", msg->payload.c_str()));
    message_queue.push(msg);

    // Request a write callback
    request_write();

    configPRINTF(("Entering wait on custom event"));
    auto clock_var = std::chrono::steady_clock::now();
    // Simple milliseconds since epoch
    auto clock_s = std::chrono::duration_cast<std::chrono::seconds>(clock_var.time_since_epoch());

    message_queue.wait_on_custom_event([&] { return (true == msg->message_sent); },
                                       std::chrono::seconds(MESSAGE_SEND_TIMEOUT_S));

    if (msg->message_sent) {
        configPRINTF(("Successfully sent last message over TLS websocket!"));
    } else {
        configPRINTF(("Could not send last message over TLS websocket!"));
    }
}

// Will be called from external threads
bool WebsocketLibwebsockets::send(const std::string& message) {
    if (!this->initialized()) {
        configPRINTF(("Could not send message because websocket is not properly initialized."));
        return false;
    }

    auto msg = std::make_shared<WebsocketMessage>();
    msg->payload = std::move(message);
    msg->protocol = LWS_WRITE_TEXT;

    poll_message(msg);

    return msg->message_sent;
}

void WebsocketLibwebsockets::ocpp_ping() {
    if (!this->initialized()) {
        configPRINTF(("Could not send ping because websocket is not properly initialized."));
    }

    auto msg = std::make_shared<WebsocketMessage>();
    msg->payload = this->connection_options.ping_payload;
    msg->protocol = LWS_WRITE_PING;

    poll_message(msg);
}

int WebsocketLibwebsockets::process_callback(void* wsi_ptr, int callback_reason, void* user, void* in, size_t len) {
    enum lws_callback_reasons reason = static_cast<lws_callback_reasons>(callback_reason);

    lws* wsi = reinterpret_cast<lws*>(wsi_ptr);

    // The ConnectionData is thread bound, so that if we clear it in the 'WebsocketLibwebsockets'
    // we still have a chance to close the connection here
    ConnectionData* data = reinterpret_cast<ConnectionData*>(lws_wsi_user(wsi));

    // If we are in the process of deletion, just close socket and return
    if (nullptr == data) {
        return LWS_CLOSE_SOCKET_RESPONSE_MESSAGE;
    }

    configPRINTF(("============== process_callback with reason: %d ==============", reason));
    switch (reason) {
    case LWS_CALLBACK_OPENSSL_PERFORM_SERVER_CERT_VERIFICATION:
        /* CSMS CN is verified internally by lws and mbedtls */
        break;

    case LWS_CALLBACK_OPENSSL_LOAD_EXTRA_CLIENT_VERIFY_CERTS:
        break;

    case LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER: {
        configPRINTF(("Handshake with security profile: %d", this->connection_options.security_profile));

        unsigned char **ptr = reinterpret_cast<unsigned char**>(in), *end_header = (*ptr) + len;

        if (this->connection_options.hostName.has_value()) {
            auto& str = this->connection_options.hostName.value();

            if (0 != lws_add_http_header_by_name(wsi, reinterpret_cast<const unsigned char*>("User-Host"),
                                                 reinterpret_cast<const unsigned char*>(str.c_str()), str.length(), ptr,
                                                 end_header)) {
                throw std::runtime_error("Could not append User-Host header.");
            }
        }

        if (this->connection_options.security_profile == 1 || this->connection_options.security_profile == 2) {
        	std::optional<std::string> authorization_header = this->getAuthorizationHeader();

            if (authorization_header != std::nullopt) {
                auto& str = authorization_header.value();

                if (0 != lws_add_http_header_by_token(wsi, lws_token_indexes::WSI_TOKEN_HTTP_AUTHORIZATION,
                                                      reinterpret_cast<const unsigned char*>(str.c_str()), str.length(),
                                                      ptr, end_header)) {
                    throw std::runtime_error("Could not append authorization header.");
                }

                /*
                // TODO: See if we need to switch back here
                if (0 != lws_add_http_header_by_name(wsi, reinterpret_cast<const unsigned char*>("Authorization"),
                                                     reinterpret_cast<const unsigned char*>(str.c_str()), str.length(),
                                                     ptr, end_header)) {
                    EVLOG_AND_THROW(std::runtime_error("Could not append authorization header."));
                }
                */
            } else {
                throw
                    std::runtime_error("No authorization key provided when connecting with security profile 1 or 2.");
            }
        }

        return 0;
    } break;

    case LWS_CALLBACK_CLIENT_CONNECTION_ERROR: {
        std::string error_message = (in ? reinterpret_cast<char*>(in) : "(null)");
        configPRINTF(("CLIENT_CONNECTION_ERROR: [%s]. Attempting reconnect", error_message.c_str()));
        //ERR_print_errors_fp(stderr);

        if (error_message.find("HS: ws upgrade unauthorized") != std::string::npos) {
            this->push_deferred_callback([this]() {
                if (this->connection_failed_callback) {
                    this->connection_failed_callback(ConnectionFailedReason::FailedToAuthenticateAtCsms);
                } else {
                    configPRINTF(("Connection failed callback not registered!"));
                }
            });
        }

        data->update_state(EConnectionState::ERROR);
        on_conn_fail(data);

        return 0;
    }
    case LWS_CALLBACK_CONNECTING:
        configPRINTF(("Client connecting..."));
        data->update_state(EConnectionState::CONNECTING);
        break;

    case LWS_CALLBACK_CLIENT_FILTER_PRE_ESTABLISH:
        try {
            char buffer[16] = {0};
            lws_hdr_copy(wsi, buffer, 16, WSI_TOKEN_PROTOCOL);
            this->connected_ocpp_version = ocpp::conversions::string_to_ocpp_protocol_version(buffer);
        } catch (StringToEnumException& e) {
            configPRINTF(("CSMS did not select protocol: ")); // << e.what();
            this->connected_ocpp_version = OcppProtocolVersion::Unknown;
        }
        break;

    case LWS_CALLBACK_CLIENT_ESTABLISHED:
        data->update_state(EConnectionState::CONNECTED);
        on_conn_connected(data);

        // Attempt first write after connection
        lws_callback_on_writable(wsi);
        break;

    case LWS_CALLBACK_WS_PEER_INITIATED_CLOSE: {
        std::string close_reason(reinterpret_cast<char*>(in), len);
        unsigned char* pp = reinterpret_cast<unsigned char*>(in);
        unsigned short close_code = (unsigned short)((pp[0] << 8) | pp[1]);

        // In the case that the websocket (server) has closed the
        // connection we  must ALWAYS try to reconnect
        configPRINTF(("Websocket peer initiated close with reason: [%s] close code: [%d]. Reconnecting",
        		close_reason.c_str(), close_code));
        data->update_state(EConnectionState::ERROR);
        on_conn_fail(data);

        // Return 0 to print peer close reason
        return 0;
    }

    case LWS_CALLBACK_CLIENT_CLOSED:
        // Determine if the close connection was requested or if the server went away
        // case in which we receive a 'LWS_CALLBACK_CLIENT_CLOSED' that was not requested
        if (data->is_interupted()) {
            configPRINTF(("Client closed, was requested internally, finalizing connection, not reconnecting"));
            data->update_state(EConnectionState::FINALIZED);
            on_conn_close(data);
        } else {
            configPRINTF(("Client closed, was not requested internally, attempting reconnection"));
            // It means the server went away, attempt to reconnect
            data->update_state(EConnectionState::ERROR);
            on_conn_fail(data);
        }

        break;

    case LWS_CALLBACK_WS_CLIENT_DROP_PROTOCOL:
        break;

    case LWS_CALLBACK_CLIENT_WRITEABLE:
        on_conn_writable();
        if (false == message_queue.empty()) {
            lws_callback_on_writable(wsi);
        }
        break;

    case LWS_CALLBACK_CLIENT_RECEIVE_PONG: {
        if (false == message_queue.empty()) {
            lws_callback_on_writable(data->get_conn());
        }
    } break;

    case LWS_CALLBACK_CLIENT_RECEIVE:
        recv_buffered_message.append(reinterpret_cast<char*>(in), reinterpret_cast<char*>(in) + len);
        configPRINTF(("LWS_CALLBACK_CLIENT_RECEIVE recv_buffered_mesage: %s", recv_buffered_message.c_str()));
        // Message is complete
        if (lws_remaining_packet_payload(wsi) <= 0) {
            on_conn_message(std::move(recv_buffered_message));
            recv_buffered_message.clear();
        }

        if (false == message_queue.empty()) {
            lws_callback_on_writable(data->get_conn());
        }
        break;

    case LWS_CALLBACK_EVENT_WAIT_CANCELLED: {
        if (false == message_queue.empty()) {
            lws_callback_on_writable(data->get_conn());
        }
    } break;

    // not interested in these callbacks
    case LWS_CALLBACK_WSI_DESTROY:
    	configPRINTF(("wsi was destroyed in the libwebsockets layer"));
        // It means the server went away, attempt to reconnect
        data->update_state(EConnectionState::ERROR);
        on_conn_fail(data);
    	break;

    case LWS_CALLBACK_SERVER_NEW_CLIENT_INSTANTIATED:
    case LWS_CALLBACK_CLIENT_HTTP_DROP_PROTOCOL:
    case LWS_CALLBACK_PROTOCOL_INIT:
    case LWS_CALLBACK_PROTOCOL_DESTROY:
    case LWS_CALLBACK_WSI_CREATE:
    case LWS_CALLBACK_WSI_TX_CREDIT_GET:
    case LWS_CALLBACK_OPENSSL_LOAD_EXTRA_SERVER_VERIFY_CERTS:
    case LWS_CALLBACK_OPENSSL_PERFORM_CLIENT_CERT_VERIFICATION:
    case LWS_CALLBACK_SSL_INFO:
    case LWS_CALLBACK_HTTP:
    case LWS_CALLBACK_HTTP_BODY:
    case LWS_CALLBACK_HTTP_BODY_COMPLETION:
    case LWS_CALLBACK_HTTP_FILE_COMPLETION:
    case LWS_CALLBACK_HTTP_WRITEABLE:
    case LWS_CALLBACK_CLOSED_HTTP:
    case LWS_CALLBACK_FILTER_HTTP_CONNECTION:
    case LWS_CALLBACK_ADD_HEADERS:
    case LWS_CALLBACK_VERIFY_BASIC_AUTHORIZATION:
    case LWS_CALLBACK_CHECK_ACCESS_RIGHTS:
    case LWS_CALLBACK_PROCESS_HTML:
    case LWS_CALLBACK_HTTP_BIND_PROTOCOL:
    case LWS_CALLBACK_HTTP_DROP_PROTOCOL:
    case LWS_CALLBACK_HTTP_CONFIRM_UPGRADE:
    case LWS_CALLBACK_ESTABLISHED_CLIENT_HTTP:
    case LWS_CALLBACK_CLOSED_CLIENT_HTTP:
    case LWS_CALLBACK_RECEIVE_CLIENT_HTTP_READ:
    case LWS_CALLBACK_RECEIVE_CLIENT_HTTP:
    case LWS_CALLBACK_COMPLETED_CLIENT_HTTP:
    case LWS_CALLBACK_CLIENT_HTTP_WRITEABLE:
    case LWS_CALLBACK_CLIENT_HTTP_REDIRECT:
    case LWS_CALLBACK_CLIENT_HTTP_BIND_PROTOCOL:
    case LWS_CALLBACK_ESTABLISHED:
    case LWS_CALLBACK_CLOSED:
    case LWS_CALLBACK_SERVER_WRITEABLE:
    case LWS_CALLBACK_RECEIVE:
    case LWS_CALLBACK_RECEIVE_PONG:
    case LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION:
    case LWS_CALLBACK_CONFIRM_EXTENSION_OKAY:
    case LWS_CALLBACK_WS_SERVER_BIND_PROTOCOL:
    case LWS_CALLBACK_WS_SERVER_DROP_PROTOCOL:
    case LWS_CALLBACK_CLIENT_CONFIRM_EXTENSION_SUPPORTED:
    case LWS_CALLBACK_WS_EXT_DEFAULTS:
    case LWS_CALLBACK_FILTER_NETWORK_CONNECTION:
    case LWS_CALLBACK_WS_CLIENT_BIND_PROTOCOL:
    case LWS_CALLBACK_GET_THREAD_ID:
    case LWS_CALLBACK_ADD_POLL_FD:
    case LWS_CALLBACK_DEL_POLL_FD:
    case LWS_CALLBACK_CHANGE_MODE_POLL_FD:
    case LWS_CALLBACK_LOCK_POLL:
    case LWS_CALLBACK_UNLOCK_POLL:
    case LWS_CALLBACK_CGI:
    case LWS_CALLBACK_CGI_TERMINATED:
    case LWS_CALLBACK_CGI_STDIN_DATA:
    case LWS_CALLBACK_CGI_STDIN_COMPLETED:
    case LWS_CALLBACK_CGI_PROCESS_ATTACH:
    case LWS_CALLBACK_SESSION_INFO:
    case LWS_CALLBACK_GS_EVENT:
    case LWS_CALLBACK_HTTP_PMO:
    case LWS_CALLBACK_RAW_PROXY_CLI_RX:
    case LWS_CALLBACK_RAW_PROXY_SRV_RX:
    case LWS_CALLBACK_RAW_PROXY_CLI_CLOSE:
    case LWS_CALLBACK_RAW_PROXY_SRV_CLOSE:
    case LWS_CALLBACK_RAW_PROXY_CLI_WRITEABLE:
    case LWS_CALLBACK_RAW_PROXY_SRV_WRITEABLE:
    case LWS_CALLBACK_RAW_PROXY_CLI_ADOPT:
    case LWS_CALLBACK_RAW_PROXY_SRV_ADOPT:
    case LWS_CALLBACK_RAW_PROXY_CLI_BIND_PROTOCOL:
    case LWS_CALLBACK_RAW_PROXY_SRV_BIND_PROTOCOL:
    case LWS_CALLBACK_RAW_PROXY_CLI_DROP_PROTOCOL:
    case LWS_CALLBACK_RAW_PROXY_SRV_DROP_PROTOCOL:
    case LWS_CALLBACK_RAW_RX:
    case LWS_CALLBACK_RAW_CLOSE:
    case LWS_CALLBACK_RAW_WRITEABLE:
    case LWS_CALLBACK_RAW_ADOPT:
    case LWS_CALLBACK_RAW_CONNECTED:
    case LWS_CALLBACK_RAW_SKT_BIND_PROTOCOL:
    case LWS_CALLBACK_RAW_SKT_DROP_PROTOCOL:
    case LWS_CALLBACK_RAW_ADOPT_FILE:
    case LWS_CALLBACK_RAW_RX_FILE:
    case LWS_CALLBACK_RAW_WRITEABLE_FILE:
    case LWS_CALLBACK_RAW_CLOSE_FILE:
    case LWS_CALLBACK_RAW_FILE_BIND_PROTOCOL:
    case LWS_CALLBACK_RAW_FILE_DROP_PROTOCOL:
    case LWS_CALLBACK_TIMER:
    case LWS_CALLBACK_CHILD_CLOSING:
    case LWS_CALLBACK_VHOST_CERT_AGING:
    case LWS_CALLBACK_VHOST_CERT_UPDATE:
    case LWS_CALLBACK_MQTT_NEW_CLIENT_INSTANTIATED:
    case LWS_CALLBACK_MQTT_IDLE:
    case LWS_CALLBACK_MQTT_CLIENT_ESTABLISHED:
    case LWS_CALLBACK_MQTT_SUBSCRIBED:
    case LWS_CALLBACK_MQTT_CLIENT_WRITEABLE:
    case LWS_CALLBACK_MQTT_CLIENT_RX:
    case LWS_CALLBACK_MQTT_UNSUBSCRIBED:
    case LWS_CALLBACK_MQTT_DROP_PROTOCOL:
    case LWS_CALLBACK_MQTT_CLIENT_CLOSED:
    case LWS_CALLBACK_MQTT_ACK:
    case LWS_CALLBACK_MQTT_RESEND:
    case LWS_CALLBACK_MQTT_UNSUBSCRIBE_TIMEOUT:
    case LWS_CALLBACK_MQTT_SHADOW_TIMEOUT:
    case LWS_CALLBACK_USER:
        break;
    }

    // If we are interrupted, close the socket cleanly
    if (data->is_interupted()) {
        return LWS_CLOSE_SOCKET_RESPONSE_MESSAGE;
    }

    //Return -1 on fatal error (-1 is request to close the socket)
    return 0;
}

void WebsocketLibwebsockets::on_conn_connected(ConnectionData* conn_data) {
    // Called on the websocket client thread
    configPRINTF(("OCPP client successfully connected to server with version: %d", this->connected_ocpp_version));

    this->connection_attempts = 1; // reset connection attempts
    this->m_is_connected = true;
    this->reconnecting = false;

// fixed EasyEVSE
    this->set_websocket_ping_interval(this->connection_options.ping_interval_s);

    // Stop any dangling reconnect
    {
        std::lock_guard<std::mutex> lk(this->reconnect_mutex);
        this->reconnect_timer_tpm.stop();
    }

    // Clear any irrelevant data after a DC
    clear_all_queues();

    this->push_deferred_callback([this]() {
        if (connected_callback) {
            this->connected_callback(this->connected_ocpp_version);
        } else {
            configPRINTF(("Connected callback not registered!"));
        }
    });
}

void WebsocketLibwebsockets::on_conn_close(ConnectionData* conn_data) {
    // Called on the websocket client thread
    configPRINTF(("OCPP client closed connection to server"));

    this->m_is_connected = false;

    {
        std::lock_guard<std::mutex> lk(this->reconnect_mutex);
        this->reconnect_timer_tpm.stop();
    }

    // Clear any irrelevant data after a DC
    clear_all_queues();

    this->push_deferred_callback([this]() {
        if (this->stopped_connecting_callback) {
            this->stopped_connecting_callback(WebsocketCloseReason::Normal);
        } else {
            configPRINTF(("Stopped connecting callback not registered!"));
        }

        if (this->disconnected_callback) {
            this->disconnected_callback();
        } else {
            configPRINTF(("Disconnected callback not registered!"));
        }
    });

    // We have polled the stopped connected
    conn_data->mark_stop_executed();
}

void WebsocketLibwebsockets::on_conn_fail(ConnectionData* conn_data) {
    // Called on the websocket client thread
    configPRINTF(("OCPP client connection to server failed"));

    if (this->m_is_connected) {
        this->push_deferred_callback([this]() {
            if (this->disconnected_callback) {
                this->disconnected_callback();
            } else {
                configPRINTF(("Disconnected callback not registered!\r\n"));
            }
        });
    }

    this->m_is_connected = false;

    // Clear any irrelevant data after a DC
    clear_all_queues();

    // TODO: See if this is required for a faster fail
    // lws_set_timeout(conn_data->get_conn(), (enum pending_timeout)1, LWS_TO_KILL_ASYNC);
}

void WebsocketLibwebsockets::on_conn_message(std::string&& message) {
    // Called on the websocket client thread
    if (!this->initialized()) {
        configPRINTF(("Message received but TLS websocket has not been correctly initialized. Discarding message."));
        return;
    }

    recv_message_queue.push(std::move(message));
}

void WebsocketLibwebsockets::on_conn_writable() {
    // Called on the websocket client thread
    if (!this->initialized() || !this->m_is_connected) {
        configPRINTF(("Message sending but TLS websocket has not been correctly initialized/connected."));
        return;
    }

    std::shared_ptr<ConnectionData> local_data = conn_data;

    if (local_data == nullptr) {
        configPRINTF(("Message sending TLS websocket with null connection data!"));
        return;
    }

    if (local_data->is_interupted() || local_data->get_state() == EConnectionState::FINALIZED) {
        configPRINTF(("Trying to write message to interrupted/finalized state!"));
        return;
    }

    // Execute while we have messages that were polled
    while (true) {
        // Break if we have en empty queue
        if (message_queue.empty())
            break;

        auto message = message_queue.front();

        if (message == nullptr) {
            throw std::runtime_error("Null message in queue, fatal error!");
        }

        // This message was polled in a previous iteration
        if (message->sent_bytes >= message->payload.length()) {
            configPRINTF(("Websocket message fully written, popping processing thread from queue!"));

            // If we have written all bytes to libwebsockets it means that if we received
            // this writable callback everything is sent over the wire, mark it as sent and remove
            message->message_sent = true;
            message_queue.pop();
        } else {
            // If the message was not polled, we reached the first unpolled and break
            break;
        }
    }

    // If we still have message ONLY poll a single one that can be processed in the invoke of the function
    // libwebsockets is designed so that when a message is sent to the wire from the internal buffer it
    // will invoke 'on_conn_writable' again and we can execute the code above
    if (!message_queue.empty()) {
        // Poll a single message
        configPRINTF(("Client writable, sending message part!"));

        auto message = message_queue.front();

        if (message == nullptr) {
            throw std::runtime_error("Null message in queue, fatal error!");
        }

        if (message->sent_bytes >= message->payload.length()) {
            throw std::runtime_error("Already polled message should be handled above, fatal error!");
        }

        // Continue sending message part, for a single message only
        bool sent = send_internal(local_data->get_conn(), message.get());

        // If we failed, attempt again later
        if (!sent) {
            message->sent_bytes = 0;
        }
    }
}

void WebsocketLibwebsockets::push_deferred_callback(const std::function<void()>& callback) {
    if (!callback) {
        configPRINTF(("\r\n Attempting to push stale callback in deferred queue!"));
        return;
    }

    this->deferred_callback_queue.push(callback);
}

void WebsocketLibwebsockets::thread_deferred_callback_queue() {
    while (true) {
        std::function<void()> callback;
        {
            this->deferred_callback_queue.wait_on_queue_element_or_predicate(
                [this]() { return this->stop_deferred_handler.load(); });

            if (stop_deferred_handler and this->deferred_callback_queue.empty()) {
                break;
            }

            callback = this->deferred_callback_queue.pop();
        }

        // This needs to be out of lock scope otherwise we still keep the mutex locked while executing the callback.
        // This would block the callers of push_deferred_callback()
        if (callback) {
            callback();
        } else {
            configPRINTF(("Stale callback in deferred queue!"));
        }
    }
}
} // namespace ocpp
