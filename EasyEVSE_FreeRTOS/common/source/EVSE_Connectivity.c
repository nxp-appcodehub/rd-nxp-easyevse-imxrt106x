/*
 * Copyright 2023-2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "board.h"
#include "pin_mux.h"

#include "FreeRTOS.h"

/* lwIP Includes */

#include "lwip/apps/sntp.h"
#include "lwip/dns.h"
#include "lwip/netifapi.h"

#include "fsl_silicon_id.h"
#include "fsl_iomuxc.h"
#include "timers.h"

#include "EVSE_ConnectivityConfig.h"
#include "EVSE_Ocpp.h"
#include "EVSE_Connectivity.h"
#include "EVSE_UI.h"
#include "EVSE_Utils.h"
#if EVSE_EDGELOCK_AGENT
#include "EVSE_EdgeLock2goAgent.h"
#endif

#if (ENABLE_WIFI == 1)
#include "wpl.h"
#include "wlan.h"
#include "flash_littlefs.h"
#else
#include "ethernetif.h"
#include "fsl_enet.h"
#include "fsl_phyksz8081.h"
#endif /* ENABLE_WIFI */

#include "ping.h"
#include "sockets.h"

/*******************************************************************************   \
 * Definitions                                                                     \
 ******************************************************************************/

#define MICROSOFT_AZURE_DNS_IP "168.63.129.16"
#define IPv4_PLACEHOLDER       "255.255.255.255"

#define PING_RCV_TIMEOUT 1000
#define PING_MAX_FAILS   4

/* Ping target is 8.8.8.8 */
#define PING_TARGET_IP_1 8
#define PING_TARGET_IP_2 8
#define PING_TARGET_IP_3 8
#define PING_TARGET_IP_4 8

#if (ENABLE_WIFI == 1)

#define WIFI_NETWORK_LABEL       "EVSE_WIFI"
#define EVSE_WIFI_CRED_FILE_NAME "evse_wifi_cred.dat"

#else

#ifndef EVSE_ETH_NETIF_INIT_FN
/*! @brief Network interface initialization function. */
#define EVSE_ETH_NETIF_INIT_FN ethernetif0_init
#endif /* EVSE_ETH_NETIF_INIT_FN */

#endif /* (ENABLE_WIFI == 1) */

#define ERROR_LOOP loop(__FUNCTION__, __LINE__, ret)

static const char *networkConnectionStateString[EVSE_Network_Last + 1] = {
    [EVSE_Network_NotConnected]                = "No network",
    [EVSE_Network_InitComunicationModule]      = "Init communication module",
    [EVSE_Network_InitComunicationModuleError] = "Init communication module error",
    [EVSE_Network_InitCommunicationStack]      = "Init communication stack",
    [EVSE_Network_InitCommunicationStackError] = "Init communication stack error",
    [EVSE_Network_APConnectTry]                = "Trying to connect to AP",
    [EVSE_Network_APConnected]                 = "Connected to AP",
    [EVSE_Network_APConnectError]              = "Failed connection to AP",
    [EVSE_Network_DHCPRequest]                 = "Waiting for DHCP response",
    [EVSE_Network_DHCPError]                   = "Failed DHCP",
    [EVSE_Network_SNTPRequest]                 = "Waiting for UnixTime",
    [EVSE_Network_SNTPError]                   = "Failed SNTP",
    [EVSE_Network_NetworkConnected]            = "Connected to network",
    [EVSE_Network_NetworkDisconnected]         = "Disconnected from network",
    [EVSE_Network_EdgelockInit]                = "Initializing EdgeLock2GO",
    [EVSE_Network_WaitEdgelock]                = "Waiting for EdgeLock2GO",
    [EVSE_Network_OCPP_Init]                   = "Initializing OCPP",
    [EVSE_Network_Last]                        = "LAST",
};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void ping_setup(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if (ENABLE_WIFI == 1)
static evse_wifi_cred_t evse_wifi_credentials;
#else
static struct netif s_netif;
static phy_ksz8081_resource_t s_phy_resource;
static phy_handle_t phyHandle;
#endif /* (ENABLE_WIFI == 1) */

/* Store the ipv4 address to string */
static char s_IPv4[IP4ADDR_STRLEN_MAX];

/* Store the unix time received from the SNTP server */
static uint32_t s_unixTimeBase = 0;

/* Connection state */
static connectionState_t s_connectionState = EVSE_Network_NotConnected;
/* Task handles for EVSE */
static TaskHandle_t xEVSE_Connectivity_Handle;

static connectionModule_t connectionModule = EVSE_ConnectionModule_None;

static int ping_socket;
static ip4_addr_t ip4_ping_target;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void loop(const char *function, uint32_t line, int ret)
{
    configPRINTF((error("Error %s line %d: %d\r\n"), function, line, ret));

    while (true)
    {
        vTaskDelay(10);
    }
}

static void prvConnectivity_SetState(connectionState_t connectionState)
{
    if (s_connectionState == connectionState)
    {
        return;
    }

    if (connectionState == EVSE_Network_NetworkDisconnected)
    {
        memset(s_IPv4, 0, IP4ADDR_STRLEN_MAX);
    }

    s_connectionState = connectionState;

#if (ENABLE_LCD)
    EVSE_UI_SetEvent(EVSE_UI_NetworkStatus);
#endif /* ENABLE_LCD */
}

static void check_dhcp_state()
{
}

#if (ENABLE_WIFI == 0)

void LinkStatusChangeCallback(struct netif *netif)
{
    /* If we are here callbacks are attached. The communication stack has been initialised.
     * Safe to assume that we are over module init
     */
    if (netif != NULL)
    {
        if (((netif->flags & NETIF_FLAG_LINK_UP) == 0) && (s_connectionState != EVSE_Network_InitComunicationModule))
        {
            /* Connection down */
            prvConnectivity_SetState(EVSE_Network_NetworkDisconnected);
        }
        else if (((netif->flags & NETIF_FLAG_LINK_UP) == NETIF_FLAG_LINK_UP) &&
                 (s_connectionState == EVSE_Network_NetworkDisconnected))
        {
            /* Link is up */
            prvConnectivity_SetState(EVSE_Network_InitCommunicationStack);
        }
    }
}

static void MDIO_Init(void)
{
    (void)CLOCK_EnableClock(s_enetClock[ENET_GetInstance(EVSE_ETH_ENET)]);
    ENET_SetSMI(EVSE_ETH_ENET, EVSE_ETH_CLOCK_FREQ, false);
}

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_MDIOWrite(EVSE_ETH_ENET, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_MDIORead(EVSE_ETH_ENET, phyAddr, regAddr, pData);
}

/**
 * Init the connectivity stack for the ETH
 */
static void ConnectivityStack_Init()
{
    err_t ret                       = ERR_OK;
    ethernetif_config_t enet_config = {.phyHandle   = &phyHandle,
                                       .phyAddr     = EVSE_ETH_PHY_ADDRESS,
                                       .phyOps      = EVSE_ETH_PHY_OPS,
                                       .phyResource = EVSE_ETH_PHY_RESOURCE,
                                       .srcClockHz  = EVSE_ETH_CLOCK_FREQ,
#ifdef configMAC_ADDR
                                       .macAddress = configMAC_ADDR
#endif
    };

    prvConnectivity_SetState(EVSE_Network_InitCommunicationStack);
    MDIO_Init();
    s_phy_resource.read  = MDIO_Read;
    s_phy_resource.write = MDIO_Write;

    /* Set MAC address. */
#ifndef configMAC_ADDR
    (void)SILICONID_ConvertToMacAddr(&enet_config.macAddress);
#endif

    tcpip_init(NULL, NULL);

    ret |= netifapi_netif_add(&s_netif, NULL, NULL, NULL, &enet_config, EVSE_ETH_NETIF_INIT_FN, tcpip_input);
    ret |= netifapi_netif_set_default(&s_netif);
    ret |= netifapi_netif_set_up(&s_netif);

    /* Add status callback */
    LOCK_TCPIP_CORE();
    netif_set_link_callback(&s_netif, LinkStatusChangeCallback);
    netif_set_status_callback(&s_netif, LinkStatusChangeCallback);
    UNLOCK_TCPIP_CORE();

    if (ret == (err_t)ERR_OK)
    {
        /* Wait for link to be up */
        while (ethernetif_wait_linkup(&s_netif, 5000) != ERR_OK)
        {
            configPRINTF((
                error("PHY Auto-negotiation failed. Please check the cable connection and link partner setting.\r\n")));
        }
    }

    if (ret != (err_t)ERR_OK)
    {
        ERROR_LOOP;
    }

    ping_setup();
}

static void ConnectivityModule_Init(void)
{
    prvConnectivity_SetState(EVSE_Network_InitComunicationModule);
    BOARD_EnableETH();
}

static void DHCP_StartClient()
{
    err_t ret = ERR_OK;

    configPRINTF(("DHCP In Progress...\r\n"));
    prvConnectivity_SetState(EVSE_Network_DHCPRequest);
    /* Start the DHCP client request */
    ret = netifapi_dhcp_start(&s_netif);

    if (ret == (err_t)ERR_OK)
    {
        (void)ethernetif_wait_ipv4_valid(&s_netif, ETHERNETIF_WAIT_FOREVER);

        strcpy(s_IPv4, ipaddr_ntoa(&s_netif.ip_addr));
        configPRINTF((info("IPv4 Address: %s"), ipaddr_ntoa(&s_netif.ip_addr)));
        configPRINTF((info("GW Address: %s "), ipaddr_ntoa(&s_netif.gw)));
        configPRINTF((info("NetworkMask Address: %s"), ipaddr_ntoa(&s_netif.netmask)));
        configPRINTF((success("DHCP OK\r\n")));
    }
    else
    {
        prvConnectivity_SetState(EVSE_Network_DHCPError);
        ERROR_LOOP;
    }
}

#else
/* Link lost callback */
static void LinkStatusChangeCallback(linkState_t linkState)
{
    if (linkState == WPRET_LINK_LOST_RETRY)
    {
        configPRINTF((warning("-------- LINK LOST Retry --------\r\n")));
        prvConnectivity_SetState(EVSE_Network_NetworkDisconnected);
    }
    else if (linkState == WPRET_LINK_ESTABLISHED)
    {
        configPRINTF((success("-------- LINK REESTABLISHED --------\r\n")));
        prvConnectivity_SetState(EVSE_Network_APConnected);
    }
    else if (linkState == WPRET_LINK_LOST_NORETRY)
    {
        configPRINTF((error("-------- LINK LOST No retry--------\r\n")));
        /* Go back to init stack so we redo the whole reconnection flow */
        prvConnectivity_SetState(EVSE_Network_InitCommunicationStack);
        WPL_Leave();
        WPL_RemoveNetwork(WIFI_NETWORK_LABEL);
    }
}

static void ConnectivityStack_Init()
{
    prvConnectivity_SetState(EVSE_Network_InitCommunicationStack);
    wpl_ret_t ret = WPL_Start(LinkStatusChangeCallback);
    if (ret != WPLRET_SUCCESS)
    {
        prvConnectivity_SetState(EVSE_Network_InitCommunicationStackError);
        ERROR_LOOP;
    }

    ping_setup();
}

static void ConnectivityModule_Init(void)
{
    prvConnectivity_SetState(EVSE_Network_InitComunicationModule);
    wpl_ret_t ret = WPL_Init();

    if (ret != WPLRET_SUCCESS)
    {
        prvConnectivity_SetState(EVSE_Network_InitComunicationModuleError);
        ERROR_LOOP;
    }
}

static void WiFi_LoadPredefinedCredentials(evse_wifi_cred_t *wifi_credentials)
{
    if (wifi_credentials == NULL)
    {
        return;
    }

    memset(wifi_credentials->ssid.name, 0, SSID_MAX_SIZE);
    memset(wifi_credentials->pass.name, 0, PASS_MAX_SIZE);

    if ((strlen(WIFI_SSID) >= SSID_MAX_SIZE) || (strlen(WIFI_PASS) >= PASS_MAX_SIZE))
    {
        return;
    }

    memcpy(wifi_credentials->ssid.name, WIFI_SSID, strlen(WIFI_SSID));
    wifi_credentials->ssid.length = strlen(WIFI_SSID);

    memcpy(wifi_credentials->pass.name, WIFI_PASS, strlen(WIFI_PASS));
    wifi_credentials->pass.length = strlen(WIFI_PASS);

    if (EVSE_Connectivity_Wifi_Credentials_Flash_Save(wifi_credentials) != FLASH_FS_OK)
    {
        configPRINTF(
            (warning("The 'wifi print' command may not work as expected due to error saving credentials to flash.")));
    }
}

static void ConnectivityAP_Connect()
{
    prvConnectivity_SetState(EVSE_Network_APConnectTry);

    flash_fs_status_t status = EVSE_Connectivity_Wifi_Credentials_Flash_Read(&evse_wifi_credentials);
    if (status != FLASH_FS_OK)
    {
        configPRINTF((error("Failed to retrieve wifi credentials from flash")));
        configPRINTF((info("Loading predefined wifi credentials")));
        WiFi_LoadPredefinedCredentials(&evse_wifi_credentials);
    }

    wpl_ret_t ret =
        WPL_AddNetwork(evse_wifi_credentials.ssid.name, evse_wifi_credentials.pass.name, WIFI_NETWORK_LABEL);

    if (ret == WPLRET_SUCCESS)
    {
        do
        {
            /* this call is blocking. Status*/
            ret = WPL_Join(WIFI_NETWORK_LABEL);
            if (ret != WPLRET_SUCCESS)
            {
                prvConnectivity_SetState(EVSE_Network_APConnectError);
                /* wait 1s before trying another connection to AP */
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
        } while (ret != WPLRET_SUCCESS);
    }

    if (ret != WPLRET_SUCCESS)
    {
        prvConnectivity_SetState(EVSE_Network_APConnectError);
        ERROR_LOOP;
    }
    else
    {
        prvConnectivity_SetState(EVSE_Network_APConnected);
    }
}

static void DHCP_StartClient()
{
    struct wlan_ip_config addr;

    prvConnectivity_SetState(EVSE_Network_DHCPRequest);

    if (wlan_get_address(&addr) == WM_SUCCESS)
    {
        strcpy(s_IPv4, ipaddr_ntoa((ip4_addr_t *)&addr.ipv4.address));
        configPRINTF((info("IPv4 Address: %s"), ipaddr_ntoa((ip4_addr_t *)&addr.ipv4.address)));
        configPRINTF((info("GW Address: %s "), ipaddr_ntoa((ip4_addr_t *)&addr.ipv4.gw)));
        configPRINTF((info("NetworkMask Address: %s"), ipaddr_ntoa((ip4_addr_t *)&addr.ipv4.address)));
        configPRINTF((success("DHCP OK\r\n")));
    }
}

#endif /* ENABLE_WIFI */

/**
 * Try to get the unix time.
 */
static void SNTP_Connection()
{
    ip_addr_t sntp_ip  = {0};
    uint8_t sntp_tries = 0;
    uint8_t ret        = 0;

    prvConnectivity_SetState(EVSE_Network_SNTPRequest);

    while (sntp_tries < EVSE_SNTP_SYNC_MAX)
    {
        /* Resolve DNS for SNTP server */
        netconn_gethostbyname(EVSE_SNTP_SERVER_NAME, &sntp_ip);

        /* Check if sntp server IP is valid */
        if (ip_addr_isany_val(sntp_ip))
        {
            configPRINTF((error("SNTP servername '%s' DNS resolve failed"), EVSE_SNTP_SERVER_NAME));
            sntp_tries++;
            ret = 1;
            vTaskDelay(pdMS_TO_TICKS(EVSE_SNTP_UPDATE_INTERVAL));
            continue;
        }

        LOCK_TCPIP_CORE();
        sntp_setoperatingmode(SNTP_OPMODE_POLL);
        sntp_setserver(0, &sntp_ip);
        UNLOCK_TCPIP_CORE();
        ret        = ERR_OK;
        sntp_tries = 0;
        break;
    }

    configPRINTF(("SNTP Time Sync..."));

    while (sntp_tries < EVSE_SNTP_SYNC_MAX)
    {
        uint8_t sntp_update_tries = 0;
        ip4_addr_t sntp_ipv4      = sntp_ip;

        /* Output SNTP Server address.  */
        configPRINTF((info("SNTP Server address: %lu.%lu.%lu.%lu"), (sntp_ipv4.addr >> 24),
                      (sntp_ipv4.addr >> 16 & 0xFF), (sntp_ipv4.addr >> 8 & 0xFF), (sntp_ipv4.addr & 0xFF)));

        /* Start SNTP request */
        LOCK_TCPIP_CORE();
        sntp_init();
        UNLOCK_TCPIP_CORE();

        while ((sntp_update_tries < EVSE_SNTP_UPDATE_MAX) && (s_unixTimeBase == 0))
        {
            sntp_update_tries++;
            vTaskDelay(pdMS_TO_TICKS(EVSE_SNTP_UPDATE_INTERVAL));
        }

        LOCK_TCPIP_CORE();
        sntp_stop();
        UNLOCK_TCPIP_CORE();

        if (s_unixTimeBase != 0)
        {
            /* If Unix Time obtain, break the loop */
            ret        = ERR_OK;
            sntp_tries = 0;
            break;
        }
        else
        {
            configPRINTF((error("SNTP Invalid service running")));
            sntp_tries++;
        }
    }

    if (sntp_tries == EVSE_SNTP_SYNC_MAX)
    {
        configPRINTF((error("SNTP Time Sync failed.\r\n")));
        ret = ERR_TIMEOUT;
    }

    if (ret != (err_t)ERR_OK)
    {
        prvConnectivity_SetState(EVSE_Network_SNTPError);
        ERROR_LOOP;
    }
#if EVSE_EDGELOCK_AGENT
    if(!EVSE_EdgeLock_IsReady())
    {
        prvConnectivity_SetState(EVSE_Network_EdgelockInit);
    }
    else
    {
        prvConnectivity_SetState(EVSE_Network_NetworkConnected);
    }
#elif ENABLE_OCPP
    prvConnectivity_SetState(EVSE_Network_OCPP_Init);
#else
    prvConnectivity_SetState(EVSE_Network_NetworkConnected);
#endif

}

/**
 * Set this DNS if the dns resolve failed
 */
static void DNS_SetServer()
{
    static ip4_addr_t ipd_addr;

    ip4addr_aton(MICROSOFT_AZURE_DNS_IP, &ipd_addr);
    dns_setserver(0, &ipd_addr);
}

static void ping_setup(void)
{
    struct timeval timeout;

    IP4_ADDR(&ip4_ping_target, PING_TARGET_IP_1, PING_TARGET_IP_2, PING_TARGET_IP_3, PING_TARGET_IP_4);

    timeout.tv_sec  = PING_RCV_TIMEOUT / 1000;
    timeout.tv_usec = (PING_RCV_TIMEOUT % 1000) * 1000;

    // LOCK_TCPIP_CORE();
    ping_socket = lwip_socket(AF_INET, SOCK_RAW, IP_PROTO_ICMP);
    if (ping_socket < 0)
    {
        configPRINTF((error("Ping socket initialization failed.\r\n")));
    }

    if (lwip_setsockopt(ping_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
    {
        configPRINTF((error("Ping socket configuration failed.\r\n")));
    }
    // UNLOCK_TCPIP_CORE();
}

/**
 * Check the network connection by pinging 8.8.8.8
 */
static void EVSE_Connectivity_CheckNetwork(void)
{
    static int8_t ping_fails = 0;

    if (ping_send(ping_socket, &ip4_ping_target) == ERR_OK)
    {
        if (ping_recv(ping_socket) == ERR_OK)
        {
            ping_fails = 0;

            if (s_connectionState == EVSE_Network_NetworkDisconnected)
            {
                configPRINTF(("Network connection back up. Connecting ..."));
#if (ENABLE_WIFI == 1)
                prvConnectivity_SetState(EVSE_Network_APConnected);
#else
                prvConnectivity_SetState(EVSE_Network_InitCommunicationStack);
#endif /* (ENABLE_WIFI == 1) */
            }
        }
        else
        {
            ping_fails++;

            if (ping_fails > PING_MAX_FAILS)
            {
                configPRINTF(("Checking network connection failed. Disconnecting ..."));
                prvConnectivity_SetState(EVSE_Network_NetworkDisconnected);
                ping_fails = 0;
            }
        }
    }
    else
    {
        configPRINTF(("Error checking network connection"));
    }
}

static void EVSE_Connectivity_Task(void *args)
{
    s_connectionState = EVSE_Network_NotConnected;

    while (1)
    {
        switch (s_connectionState)
        {
            case EVSE_Network_NotConnected:
                ConnectivityModule_Init();
                break;
            case EVSE_Network_InitComunicationModule:
                ConnectivityStack_Init();
                break;
            case EVSE_Network_InitCommunicationStack:
#if (ENABLE_WIFI == 1)
                ConnectivityAP_Connect();
            case EVSE_Network_APConnected:
#endif /* ENABLE_WIFI */
                DHCP_StartClient();
                break;
            case EVSE_Network_DHCPRequest:
                //  DNS_SetServer();
                SNTP_Connection();
                break;
            case EVSE_Network_NetworkDisconnected:
#if ENABLE_OCPP
                EVSE_OCPP_SetEvent(EVSE_NETWORK_DOWN_EVENT);
#endif
            case EVSE_Network_NetworkConnected:
                EVSE_Connectivity_CheckNetwork();
                vTaskDelay(1000);
                break;
            case EVSE_Network_EdgelockInit:
#if EVSE_EDGELOCK_AGENT
                EVSE_EdgeLock_Init();
                prvConnectivity_SetState(EVSE_Network_WaitEdgelock);
#endif /* EVSE_EDGELOCK_AGENT */
                break;
            case EVSE_Network_WaitEdgelock:
#if EVSE_EDGELOCK_AGENT
                if(EVSE_EdgeLock_IsReady())
#endif /* EVSE_EDGELOCK_AGENT */
                {
#if ENABLE_OCPP
                    prvConnectivity_SetState(EVSE_Network_OCPP_Init);
#else
                    prvConnectivity_SetState(EVSE_Network_NetworkConnected);
#endif
                }
                vTaskDelay(100);
                break;
            case EVSE_Network_OCPP_Init:
#if ENABLE_OCPP
                EVSE_OCPP_Init();
#endif /* ENABLE_OCPP */
                prvConnectivity_SetState(EVSE_Network_NetworkConnected);
                break;
            default:
                configPRINTF((error("Error state")));
        }
    }

    vTaskDelete(NULL);
}

/*******************************************************************************
 * PUBLIC API
 ******************************************************************************/

const char *EVSE_Connectivity_GetStringFromState(connectionState_t connectivityState)
{
    return networkConnectionStateString[connectivityState];
}

connectionState_t EVSE_Connectivity_GetConnectionState()
{
    configPRINTF((networkConnectionStateString[s_connectionState]));
    return s_connectionState;
}

bool EVSE_Connectivity_IsConnectedToInternet()
{
    return (s_connectionState == EVSE_Network_NetworkConnected) ? true : false;
}

void EVSE_Connectivity_SetUnixTime(uint32_t sec)
{
    uint32_t currentOffsetSec = EVSE_GetSecondsSinceBoot();

    configPRINTF((info("SNTP time(s): %d"), sec));
    configPRINTF((info("Offset time (s): %d"), currentOffsetSec));

    /* Subtract the boot time, which will be added all the time when doing get */

    s_unixTimeBase = sec - currentOffsetSec;
    EVSE_UI_SetEvent(EVSE_UI_UpdateLocalTime);
}

uint32_t EVSE_Connectivity_GetUnixTime(void)
{
    /* Using time() to get unix time on x86.
       Note: User needs to implement own time function to get the real time on device, such as: SNTP.  */

    uint32_t currentOffsetSec = EVSE_GetSecondsSinceBoot();

    return (s_unixTimeBase + currentOffsetSec);
}

const char *EVSE_Connectivity_GetIPv4()
{
    if (s_IPv4[0] == '\0')
    {
        return NULL;
    }
    else
    {
        return s_IPv4;
    }
}

#if (ENABLE_WIFI == 1)
const char *EVSE_Connectivity_GetAPName()
{
    if (EVSE_Connectivity_IsConnectedToInternet() == true)
    {
        /* TODO make it based on the netif*/
        return evse_wifi_credentials.ssid.name;
    }
    else
    {
        return NULL;
    }
}
#endif

const connectionModule_t EVSE_Connectivity_GetConnectionModule()
{
    return connectionModule;
}

void EVSE_Connectivity_Init()
{
    BaseType_t taskCreateError = pdFALSE;

#if (ENABLE_WIFI == 1)
    connectionModule = EVSE_ConnectionModule_WiFi;
    BOARD_InitUSDHCPins();
#elif (ENABLE_ETH == 1)
    connectionModule = EVSE_ConnectionModule_ETH;
    BOARD_InitEthPins();
#endif

    if (xEVSE_Connectivity_Handle == NULL)
    {
        /* Init Connectivity */
        taskCreateError = xTaskCreate(EVSE_Connectivity_Task, "Connectivity", APP_CONECTIVITY_STACK_SIZE, NULL,
                                      APP_CONECTIVITY_PRIORITY, &xEVSE_Connectivity_Handle);

        if (taskCreateError != pdPASS)
        {
            while (1)
                ;
        }
    }
}

#if ENABLE_WIFI
flash_fs_status_t EVSE_Connectivity_Wifi_Credentials_Flash_Read(evse_wifi_cred_t *wifi_cred)
{
    flash_fs_status_t status = FLASH_FS_OK;
    uint32_t len             = 0;

    status = FLASH_LITTLEFS_Read(EVSE_WIFI_CRED_FILE_NAME, NULL, 0, &len);
    if ((status == FLASH_FS_OK) && (len <= sizeof(evse_wifi_cred_t)))
    {
        status = FLASH_LITTLEFS_Read(EVSE_WIFI_CRED_FILE_NAME, (uint8_t *)wifi_cred, 0, &len);
    }

    if (status != FLASH_FS_OK)
    {
        configPRINTF((error("Failed reading WiFi credentials.")));
    }

    return status;
}

flash_fs_status_t EVSE_Connectivity_Wifi_Credentials_Flash_Save(evse_wifi_cred_t *wifi_cred)
{
    flash_fs_status_t status = FLASH_FS_OK;

    status = FLASH_LITTLEFS_Save(EVSE_WIFI_CRED_FILE_NAME, (uint8_t *)wifi_cred, sizeof(evse_wifi_cred_t));
    if (status != FLASH_FS_OK)
    {
        configPRINTF((error("Failed to save WiFi credentials in flash memory.")));
    }
    else
    {
        configPRINTF((success("Updated WiFi credentials in flash memory.")));
    }

    return status;
}

flash_fs_status_t EVSE_Connectivity_Wifi_Credentials_Flash_Erase(void)
{
    flash_fs_status_t status = FLASH_FS_OK;

    status = FLASH_LITTLEFS_Erase(EVSE_WIFI_CRED_FILE_NAME);

    if (status != FLASH_FS_OK)
    {
        configPRINTF((error("Failed to delete WiFi credentials from flash memory.")));
    }

    return status;
}
#endif /* ENABLE_WIFI */
