// Error codes
#define SERVER_OK              0
#define SERVER_ERR_GENERIC    -1
#define SERVER_ERR_NULL_ARG   -2
#define SERVER_ERR_NOT_INIT   -3 // initBL()/wifiInit() not called first
#define SERVER_ERR_BLE        -4 // Bluetooth stack / GATT setup failed
#define SERVER_ERR_TIMEOUT    -5 // no BLE connection / Wi-Fi join in time
#define SERVER_ERR_WIFI       -6 // Wi-Fi join failed (bad credentials, no AP)
#define SERVER_ERR_WIFI_LOST  -7 // Wi-Fi dropped while active (listen() returns this)
#define SERVER_ERR_NETWORK    -8 // HTTP/socket error talking to server

#define WIFI_SSID_MAX   32
#define WIFI_PASS_MAX   64

// Wi-Fi credentials, received from the phone over Bluetooth
typedef struct {
    char ssid[WIFI_SSID_MAX + 1];
    char password[WIFI_PASS_MAX + 1];
} Wifi_Credentials;

// settings for the gateway
typedef struct {
    const char *ble_name; // advertised Bluetooth name, e.g. "MailAlert-Gateway"
    const char *server_url; // where exportToServer() sends events
    uint32_t ble_timeout_ms; // how long listenBL() waits (0 = forever)
    uint32_t wifi_timeout_ms; // how long wifiInit() waits to connect
} Server_Config;

#define SERVER_CONFIG_DEFAULT() {          \
    .ble_name = "MailAlert-Gateway",       \
    .server_url = NULL,                    \
    .ble_timeout_ms = 0,                   \
    .wifi_timeout_ms = 15000               \
}

// mailbox event as received over LoRa and forwarded to the server
typedef enum {
    MAIL_EVENT_DOOR_OPENED = 1,
    MAIL_EVENT_DOOR_CLOSED = 2,
    MAIL_EVENT_LOW_BATTERY = 3
} Mail_EventType;

typedef struct {
    uint16_t seq; // sequence number (gateway drop duplicates)
    Mail_EventType type;
    uint8_t battery_pct; // 0-100, from battery_getcapacity()
} Mail_Event;

// set up the Bluetooth stack and GATT service (advertises ble_name, exposes a
// writable characteristic for Wi-Fi credentials)
ssize_t initBL(const Server_Config *config);

// start advertising and block until a phone connects and sends Wi-Fi
// credentials (or ble_timeout_ms expires); writes them to *creds
ssize_t listenBL(Wifi_Credentials *creds);

// connect to Wi-Fi with the given credentials; returns 0 once an IP is obtained
ssize_t wifiInit(const Wifi_Credentials *creds);

// active state: while Wi-Fi is connected, receive packets from the mailbox over
// LoRa and call exportToServer() for each new event.
ssize_t listen();

// send one mailbox event to the hosted server; returns 0 on success
ssize_t exportToServer(const Mail_Event *event);