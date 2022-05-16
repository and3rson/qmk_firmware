#include "endgame48_v2.h"
#include "smoothled.h"
#include "quantum.h"
#include "raw_hid.h"
#include "bitwise.h"
#include "oled/oled_driver.h"
#include "symbols.h"
#include "string.h"
#include "stdio.h"
#include "eeprom.h"
#include "encoder.h"

static uint32_t reset_requested = 0;

extern uint32_t default_color(void);

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
    if (reset_requested && timer_read32() - reset_requested > 150) {
        rgblight_setrgb((DEBUG_COLOR >> 16) & 0xFF, (DEBUG_COLOR >> 8) & 0xFF, DEBUG_COLOR & 0xFF);
        reset_keyboard();
    }

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

void toggle_rgb(void) {
    rgblight_toggle_noeeprom();
    smoothled_render();
}

void request_reset(void) {
    reset_requested = timer_read32();
}

void mute(void) {
    tap_code(KC_MUTE);
}

typedef struct {
    const char *title;
    void (*func)(void);
} menu_item_t;

static uint32_t last_render = 0;
static bool is_menu = false;
static uint8_t menu_selected = 0;
static menu_item_t menu[] = {
    {
        .title="Mute",
        .func=mute,
    },
    {
        .title="RGB",
        .func=toggle_rgb,
    },
    {
        .title="Reset",
        .func=request_reset,
    },
    {
        .title="DelEE",
        .func=eeconfig_init,
    },
};
static uint8_t menu_size = sizeof(menu) / sizeof(menu_item_t);

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (is_menu) {
        if (record->event.pressed) {
            if (keycode == KC_EGMENU) {
                menu[menu_selected].func();
                is_menu = false;
            } else {
                is_menu = false;
            }
        }
        return false;
    }

    if (keycode == KC_EGMENU && record->event.pressed) {
        is_menu = !is_menu;
        menu_selected = 0;
        return false;
    }
    return process_record_user(keycode, record);
}

bool encoder_update_kb(uint8_t index, bool clockwise) {
    if (is_menu) {
        if (clockwise) {
            menu_selected = (menu_selected + 1) % menu_size;
        } else {
            menu_selected = menu_selected ? menu_selected - 1 : menu_size - 1;
        }
    } else {
        encoder_update_user(index, clockwise);
    }
    return false;
}

bool oled_task_kb(void) {
    if (timer_elapsed32(last_render) > 100) {
        last_render = timer_read32();
        if (reset_requested) {
            oled_clear();
            oled_write_P(logo, false);
            oled_write_P(resetIcon, false);
            oled_write("\nREADY\n FOR \nFLASH", false);
        } else if (is_menu) {
            oled_clear();
            oled_write_P(logo, false);
            oled_write(" CFG \n", false);
            /* uint8_t b = eeprom_read_byte((uint8_t*)EECONFIG_SIZE); */
            /* eeprom_update_byte((uint8_t*)EECONFIG_SIZE, ++b); */
            /* char dbg[6]; */
            /* sprintf(dbg, "B: %d\n", b); */
            /* oled_write(dbg, false); */
            for (uint8_t i = 0; i < menu_size; i++) {
                char buf[6] = "     \0";
                uint8_t len = strlen(menu[i].title);
                memcpy(buf, menu[i].title, len > 5 ? 5 : len);
                oled_write(buf, menu_selected == i);
            }
        } else {
            oled_task_user();
        }
    }
    return false;
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
