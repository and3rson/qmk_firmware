#include "endgame48_v2.h"
#include "smoothled.h"
#include "quantum.h"
#include "raw_hid.h"
#include "bitwise.h"
#include "oled/oled_driver.h"

uint32_t default_color(void);

void keyboard_post_init_kb(void) {
    /* rgblight_disable(); */
    /* rgblight_sethsv(0, 0, 0); */
    /* rgblight_enable_noeeprom(); */
    /* rgblight_setrgb(0, 255, 0); */
    /* rgblight_enable_noeeprom(); */
}

void matrix_init_kb(void) {
    /* smoothled_set(0xFFFFFFF); */
    // rxtx_init();
    matrix_init_user();
}

void matrix_scan_kb(void) {
    smoothled_process();
    matrix_scan_user();
}

void suspend_power_down_kb(void) {
    /* smoothled_set(0, 1500); */
    smoothled_set(0, 0);
}

void suspend_wakeup_init_kb(void) {
    // Fix rgblight on wakeup
    smoothled_set(default_color(), 1500);
}

void raw_hid_receive(uint8_t *data, uint8_t length) {
    /* if (length >= 4 && data[0] == 0x07 && data[1] == 0x83) { */
        /* rgblight_sethsv(data[2], data[3], 255); */
    /* } */
    /* if (length >= 3) { */
    /* sprintf(debug_line, "%d", length); */
    /* raw_hid_send(data, length); */
    /* rgblight_setrgb(data[0], data[1], data[2]); */
    /* } */
}

oled_rotation_t oled_init_kb(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}
