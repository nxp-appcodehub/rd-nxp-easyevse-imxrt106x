// SPDX-License-Identifier: Apache-2.0
// Copyright 2020 - 2023 Pionix GmbH and Contributors to EVerest
// Copyright 2026 NXP

#include <random>

#include <ocpp/common/websocket/websocket_base.hpp>
#include <websocketpp_utils/base64.hpp>

namespace ocpp {

WebsocketBase::WebsocketBase() :
    m_is_connected(false),
    connected_callback(nullptr),
    stopped_connecting_callback(nullptr),
    message_callback(nullptr),
    reconnect_timer(nullptr),
    connection_attempts(1),
    reconnect_backoff_ms(0),
    shutting_down(false),
    reconnecting(false) {

    set_connection_options_base(connection_options);

#if 0
    this->ping_timer = std::make_unique<Everest::SteadyTimer>();
#else
    this->ping_timer = std::make_unique<FreeRTOSSteadyTimer>();
//    FreeRTOSSteadyTimer s_timer = FreeRTOSSteadyTimer();
//    this->ping_timer = &s_timer;
#endif
    const auto auth_key = connection_options.authorization_key;
    if (auth_key.has_value() and auth_key.value().length() < 16) {
        configPRINTF(("AuthorizationKey with only %d characters has been configured", auth_key.value().length()));
    }
}

WebsocketBase::~WebsocketBase() {
    this->cancel_reconnect_timer();
}

void WebsocketBase::set_connection_options_base(const WebsocketConnectionOptions& connection_options) {
    this->connection_options = connection_options;
}

void WebsocketBase::register_connected_callback(const std::function<void(OcppProtocolVersion protocol)>& callback) {
    this->connected_callback = callback;
}

void WebsocketBase::register_disconnected_callback(const std::function<void()>& callback) {
    this->disconnected_callback = callback;
}

void WebsocketBase::register_stopped_connecting_callback(
    const std::function<void(const WebsocketCloseReason reason)>& callback) {
    this->stopped_connecting_callback = callback;
}

void WebsocketBase::register_message_callback(const std::function<void(const std::string& message)>& callback) {
    this->message_callback = callback;
}

void WebsocketBase::register_connection_failed_callback(const std::function<void(ConnectionFailedReason)>& callback) {
    this->connection_failed_callback = callback;
}

bool WebsocketBase::initialized() {
    if (this->connected_callback == nullptr) {
        configPRINTF(("Not properly initialized: please register connected callback."));
        return false;
    }
    if (this->stopped_connecting_callback == nullptr) {
        configPRINTF(("Not properly initialized: please closed_callback."));
        return false;
    }
    if (this->message_callback == nullptr) {
        configPRINTF(("Not properly initialized: please register message callback."));
        return false;
    }

    return true;
}

void WebsocketBase::disconnect(const WebsocketCloseReason code) {
    if (!this->initialized()) {
        configPRINTF(("Cannot disconnect a websocket that was not initialized"));
        return;
    }

    {
        std::lock_guard<std::mutex> lk(this->reconnect_mutex);
        if (code == WebsocketCloseReason::Normal) {
            this->shutting_down = true;
        }
#if 0
        if (this->reconnect_timer) {
            this->reconnect_timer.get()->cancel();
        }
#else
        if (this->reconnect_timer) {
            this->reconnect_timer->stop();
        }
#endif
    }

    if (this->ping_timer) {
        this->ping_timer->stop();
    }

    configPRINTF(("Disconnecting websocket..."));
    this->close(code, "");
}

bool WebsocketBase::is_connected() {
    return this->m_is_connected;
}

std::optional<std::string> WebsocketBase::getAuthorizationHeader() {
    std::optional<std::string> auth_header = std::nullopt;
    const auto authorization_key = this->connection_options.authorization_key;
    if (authorization_key.has_value()) {
        configPRINTF(("AuthorizationKey present, encoding authentication header"));
        std::string plain_auth_header =
            this->connection_options.csms_uri.get_chargepoint_id() + ":" + authorization_key.value();

        // TODO (ioan): replace with libevse-security usage
        auth_header.emplace(std::string("Basic ") + ocpp::base64_encode(plain_auth_header));

        configPRINTF(("Basic Auth header: %s", auth_header.value().c_str()));
    }

    return auth_header;
}

#if 0
void WebsocketBase::log_on_fail(const std::error_code& ec, const boost::system::error_code& transport_ec,
                                const int http_status) {
    configPRINTF(("Failed to connect to websocket server error_code: %d, reason: %s , HTTP response code: %d, category: %s",
    		ec.value(), ec.message(), http_status, ec.category().name()));
   configPRINTF(("transport error code: %d, Transport error category: %s ", transport_ec.value(), transport_ec.category().name()));
}
#else
void WebsocketBase::log_on_fail(const std::error_code& ec,
                                const int http_status) {
    configPRINTF(("Failed to connect to websocket server error_code: %d, reason: %s , HTTP response code: %d, category: %s",
    		ec.value(), ec.message(), http_status, ec.category().name()));
}
#endif
long WebsocketBase::get_reconnect_interval() {

    // We need to add 1 to the repeat times since the first try is already connection_attempt 1
    if (this->connection_attempts > (this->connection_options.retry_backoff_repeat_times + 1)) {
        return this->reconnect_backoff_ms;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, this->connection_options.retry_backoff_random_range_s);

    int random_number = distr(gen);

    if (this->connection_attempts == 1) {
        this->reconnect_backoff_ms = (this->connection_options.retry_backoff_wait_minimum_s + random_number) * 1000;
        return this->reconnect_backoff_ms;
    }

    this->reconnect_backoff_ms = this->reconnect_backoff_ms * 2 + (random_number * 1000);
    return this->reconnect_backoff_ms;
}

void WebsocketBase::cancel_reconnect_timer() {
    std::lock_guard<std::mutex> lk(this->reconnect_mutex);
#if 0
    if (this->reconnect_timer) {
        this->reconnect_timer.get()->cancel();
    }
#else
    if (this->reconnect_timer) {
        this->reconnect_timer->stop();
    }
#endif
    this->reconnect_timer = nullptr;
}

void WebsocketBase::set_websocket_ping_interval(int32_t interval_s) {
    if (this->ping_timer) {
        this->ping_timer->stop();
    }
#if 1 //EVSE
    if (interval_s > 0) {
        this->ping_timer->interval([this]() { this->ocpp_ping(); }, std::chrono::seconds(interval_s));
    }
#endif
    this->connection_options.ping_interval_s = interval_s;
}

void WebsocketBase::set_authorization_key(const std::string& authorization_key) {
    this->connection_options.authorization_key = authorization_key;
}

void WebsocketBase::on_pong_timeout(std::string msg) {
    if (!this->reconnecting) {
        configPRINTF(("Reconnecting because of a pong timeout after %ds", this->connection_options.pong_timeout_s));
        this->reconnecting = true;
        this->close(WebsocketCloseReason::GoingAway, "Pong timeout");
    }
}

} // namespace ocpp
