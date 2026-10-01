// error codes
#define CAM_OK            0
#define CAM_ERR_GENERIC  -1
#define CAM_ERR_NULL_CFG -2 // config pointer was NULL
#define CAM_ERR_INIT     -3 // esp_camera_init() failed
#define CAM_ERR_NOT_INIT -4 // cam_init() not called / failed
#define CAM_ERR_CAPTURE  -5 // esp_camera_fb_get() returned NULL
 
typedef struct {
    // control
    int8_t pin_pwdn;
    int8_t pin_reset;
    int8_t pin_xclk;
    // SCCB
    int8_t pin_sda;
    int8_t pin_scl;
    // data bus D0..D7 (sensor Y2..Y9)
    int8_t pin_d[8];
    // sync
    int8_t pin_vsync;
    int8_t pin_href;
    int8_t pin_pclk;
    // flash LED
    int8_t pin_flash;
    // image settings
    uint32_t xclk_freq_hz;
    pixformat_t pixel_format;
    framesize_t frame_size;
    int jpeg_quality; // 0-63, lower = better
    size_t fb_count;
} Cam_Config;
 
// Defaults for ESP32-S3-EYE / Freenove S3 CAM pin layout.
// !! Verify every pin against YOUR board's schematic. !!
#define CAM_CONFIG_DEFAULT() {            \
    .pin_pwdn  = -1,                      \
    .pin_reset = -1,                      \
    .pin_xclk  = 15,                      \
    .pin_sda   = 4,                       \
    .pin_scl   = 5,                       \
    .pin_d     = {11, 9, 8, 10, 12, 18, 17, 16}, \
    .pin_vsync = 6,                       \
    .pin_href  = 7,                       \
    .pin_pclk  = 13,                      \
    .pin_flash = -1,                      \
    .xclk_freq_hz = 20000000,             \
    .pixel_format = PIXFORMAT_JPEG,       \
    .frame_size   = FRAMESIZE_SVGA,       \
    .jpeg_quality = 12,                   \
    .fb_count     = 2                     \
}

// everything will return -1 on failure (or a specific negative number to specify exactly)
// initialize OV2640 camera
ssize_t cam_init(const Cam_Config *config);

ssize_t cam_capture();

// helper function for camera that flashes in-mailbox LED during camera capture
ssize_t cam_flash(size_t time_ms);

