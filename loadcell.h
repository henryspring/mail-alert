// error codes
#define LOADCELL_OK            0
#define LOADCELL_ERR_GENERIC  -1
#define LOADCELL_ERR_NULL_CFG -2 // config pointer was NULL
#define LOADCELL_ERR_NOT_INIT -3 // loadcell_init() not called / failed
#define LOADCELL_ERR_TIMEOUT  -4 // DOUT never went low (no conversion ready / not wired)
#define LOADCELL_ERR_SATURATED -5 // raw reading pinned at 0x800000 / 0x7FFFFF
 
// input channel + gain
typedef enum {
    LOADCELL_A_GAIN_128 = 25, // channel A, +/-20 mV full scale
    LOADCELL_B_GAIN_32  = 26, // channel B, +/-80 mV full scale
    LOADCELL_A_GAIN_64  = 27 // channel A, +/-40 mV full scale
} Loadcell_Gain;
 
typedef struct {
    int8_t pin_dout; // HX711 DOUT  (low = conversion ready)
    int8_t pin_sck; // HX711 PD_SCK (serial clock + power-down)
    Loadcell_Gain gain; // use LOADCELL_A_GAIN_128 for the TAL220B
    float scale_factor; // raw counts per gram (found by calibrating with a known mass)
    uint8_t avg_samples; // readings averaged per tare/measure (reduces noise)
} Loadcell_Config;
 
#define LOADCELL_CONFIG_DEFAULT() {      \
    .pin_dout = -1, .pin_sck = -1,       \
    .gain = LOADCELL_A_GAIN_128,         \
    .scale_factor = 1.0f,                \
    .avg_samples = 10                    \
}
 
// initialize HX711: configure pins, select gain, wait for first valid conversion
ssize_t loadcell_init(const Loadcell_Config *config);
 
// zero the scale with the mailbox empty
ssize_t loadcell_tare();
 
// measure current weight
ssize_t loadcell_measure(int32_t *grams);
 
// put HX711 in power-down (PD_SCK high)
ssize_t loadcell_powerdown();
 
// wake HX711 (PD_SCK low)
ssize_t loadcell_powerup();