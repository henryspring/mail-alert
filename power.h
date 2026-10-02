typedef struct {
    uint8_t i2c_addr;
    int i2c_port;
} Battery_Config;

// Initialize battery fuel gauge
int battery_init(const Battery_Config *config);

// Read battery voltage in volts
int battery_get_voltage(float *voltage);

// Read estimated state of charge (0-100%)
int battery_get_percent(float *percent);

// Read battery current in amps
int battery_get_current(float *current);

// Return 1 if battery is charging, 0 if not
int battery_is_charging(void);
