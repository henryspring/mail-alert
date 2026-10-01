// error codes
#define DOOR_OK             0
#define DOOR_ERR_GENERIC   -1
#define DOOR_ERR_NULL_ARG  -2
#define DOOR_ERR_NOT_INIT  -3 // door_switch_init() not called / failed
 
typedef enum {
    DOOR_EDGE_RISING, // pin goes low -> high
    DOOR_EDGE_FALLING, // pin goes high -> low
    DOOR_EDGE_CHANGE // either
} Door_Edge;
 
typedef struct {
    int8_t pin; // GPIO connected to the switch
    uint8_t door_open_level; // pin level when door is OPEN
    uint16_t debounce_ms; // ignore changes within this window
} Door_Switch_Config;
 
#define DOOR_SWITCH_CONFIG_DEFAULT() {   \
    .pin = -1,                           \
    .use_internal_pullup = 1,            \
    .door_open_level = 0,                \
    .debounce_ms = 20                    \
}
 
// configure the switch GPIO (input + pull-up per config)
ssize_t door_switch_init(const Door_Switch_Config *config);
 
// register a callback that runs when the switch triggers on the given edge.
// this is also the wake-up source from sleep.
ssize_t door_switch_attachISR(void (*callback)(void), Door_Edge edge);
 
// read debounced switch state: 1 = door open, 0 = door closed
ssize_t door_switch_readSwitch();