// Error codes
#define LORA_OK             0
#define LORA_ERR_GENERIC   -1
#define LORA_ERR_NULL_CFG  -2 // config pointer was NULL
#define LORA_ERR_NOT_INIT  -3 // lora_init() not called / failed
#define LORA_ERR_CONFIG    -4 // module did not accept / echo parameters
#define LORA_ERR_TIMEOUT   -5 // AUX never went idle, or no packet in time
#define LORA_ERR_TOO_LONG  -6 // payload larger than module buffer (512 B)
 
// max bytes the module sends per pacsket
#define LORA_PACKET_SIZE   58
// module serial input buffer
#define LORA_BUFFER_SIZE   512
 
// operating modes, set with M1:M0 pins
typedef enum {
    LORA_MODE_NORMAL = 0, // M1=0 M0=0: TX + RX, transparent transmission
    LORA_MODE_WAKE   = 1, // M1=0 M0=1: like normal, but prepends wake-up code
    LORA_MODE_POWER_SAVE = 2, // M1=1 M0=0: RX only, periodic sleep; sender must use WAKE
    LORA_MODE_SLEEP  = 3 // M1=1 M0=1: sleep (~2 uA) / configuration, fixed 9600 8N1
} Lora_Mode;
 
// air data rate, both ends must match
typedef enum {
    LORA_AIR_2K4  = 2, // default and longest range
    LORA_AIR_4K8  = 3,
    LORA_AIR_9K6  = 4,
    LORA_AIR_19K2 = 5
} Lora_AirRate;
 
typedef struct {
    // host side (UART + control pins)
    int8_t uart_num; // MCU UART port connected to module TXD/RXD
    int8_t pin_tx; // MCU TX  -> module RXD
    int8_t pin_rx; // MCU RX  <- module TXD
    int8_t pin_m0;
    int8_t pin_m1;
    int8_t pin_aux;
    int8_t pin_reset; // -1 if not connected
    uint32_t uart_baud; // normal-mode baud, default 9600 (config mode is always 9600 8N1)
 
    // radio settings (must match on both ends)
    uint16_t address; // 0x0000-0xFFFF; 0xFFFF = broadcast / accept all
    uint8_t channel; // 900 MHz models: 0-69, carrier = 862 + channel MHz
    uint8_t tx_power;    // 0 = max (30 or 20 dBm by model), 1/2/3 = lower steps
    uint8_t fec_enable;  // 1 = forward error correction (default, both ends must match)
    uint8_t  wake_time;   // 0-7 = 250..2000 ms wake-on-air period (WAKE / POWER_SAVE modes)

    Lora_mode mode;
    Lora_AirRate rate;
} Lora_Config;
 
// default: 9600 baud UART, address 0, channel 53 (915 MHz), 2.4 kbps, full power, FEC on.
// pin numbers at -1 need to be filled according to schematic
#define LORA_CONFIG_DEFAULT() {                         \
    .uart_num = 1, .pin_tx = -1, .pin_rx = -1,          \
    .pin_m0 = -1, .pin_m1 = -1, .pin_aux = -1,          \
    .pin_reset = -1, .uart_baud = 9600,                 \
    .address = 0x0000, .channel = 53,                   \
    .air_rate = LORA_AIR_2K4, .tx_power = 0,            \
    .fec_enable = 1, .wake_time = 0                     \
}
 
// initialize LoRa module. teset it, write parameters in sleep/config mode, verify, then enter normal mode.
ssize_t lora_init(const Lora_Config *config);
 
// transmit payload function. waits for AUX to go idle first. Returns number of bytes sent.
ssize_t lora_send(const uint8_t *data, size_t len);
 
// wait up to timeout_ms for a packet (AUX goes low when data arrives). Returns number of bytes received, 0 on timeout
ssize_t lora_recv(uint8_t *buf, size_t max_len, uint32_t timeout_ms);
 
// wake the radio. leave sleep mode, and wait for AUX to go high
ssize_t lora_powerup();
 
// finish any pending transmit, then enter sleep mode
ssize_t lora_powerdown();