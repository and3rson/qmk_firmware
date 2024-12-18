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

typedef struct menu_item_t {
    char title[6];
    void (*func)(void);
    struct menu_t *submenu;
} menu_item_t;

typedef struct menu_t {
    char   *title;
    uint8_t count;
    uint8_t current;
    uint8_t scroll;
    void (*opened)(struct menu_t *);
    menu_item_t *items;
} menu_t;

static uint32_t last_render = 0;
/* static bool     is_menu     = false; */
/* static uint32_t menu_path   = 0; */
/* static uint32_t menu_level  = 0; */

static int8_t  depth = -1;
static menu_t *menus[8];

#define IN_MENU depth != -1
#define CURRENT_MENU menus[depth]
#define MAX_ITEMS 10

void init_eeprom_menu(menu_t *this) {
    for (int i = 0; i < this->count; i++) {
        uint8_t b = eeprom_read_byte((uint8_t *)i);
        sprintf(this->items[i].title, "%02X:%02X", i, b);
    }
}
static menu_t list_eeprom_menu = {
    .title  = "EEPRM",
    .count  = 64,
    .items  = (menu_item_t[64]){},
    .opened = init_eeprom_menu,
};
static menu_t main_menu = {
    .title = "CFG",
    .count = 5,
    .items =
        (menu_item_t[]){
            {
                .title = "Mute",
                .func  = mute,
            },
            {
                .title = "RGB",
                .func  = toggle_rgb,
            },
            {
                .title = "Reset",
                .func  = request_reset,
            },
            {
                .title   = "LsEE",
                .submenu = &list_eeprom_menu,
            },
            {
                .title = "DelEE",
                .func  = eeconfig_init,
            },
        },
};
/* static menu_t *menu = NULL; */
/* static menu_item_t menu[] = { */
/* }; */
/* static uint8_t menu_size = sizeof(menu) / sizeof(menu_item_t); */

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (IN_MENU) {
        // Menu is open
        if (record->event.pressed) {
            if (keycode == KC_EGMENU) {
                menu_t     *menu = CURRENT_MENU;
                menu_item_t item = menu->items[menu->current];
                if (item.submenu != NULL) {
                    item.submenu->current = 0;
                    item.submenu->scroll  = 0;
                    menus[++depth]        = item.submenu;
                    item.submenu->opened(item.submenu);
                } else if (item.func != NULL) {
                    item.func();
                    depth = -1;
                }
            } else {
                depth--;
            }
        }
        return false;
    } else if (keycode == KC_EGMENU) {
        // Menu is not open, KCEGMENU pressed
        if (record->event.pressed) {
            main_menu.current = 0;
            main_menu.scroll  = 0;
            menus[++depth]    = &main_menu;
        }
        return false;
    }
    return process_record_user(keycode, record);
}

bool encoder_update_kb(uint8_t index, bool clockwise) {
    if (IN_MENU) {
        menu_t *menu = CURRENT_MENU;
        if (clockwise) {
            if (menu->current < menu->count - 1) {
                menu->current++;
            }
            if (menu->current - menu->scroll >= MAX_ITEMS) {
                menu->scroll++;
            }
        } else {
            if (menu->current) {
                menu->current--;
            }
            if (menu->current < menu->scroll) {
                menu->scroll--;
            }
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
        } else if (IN_MENU) {
            oled_clear();
            oled_write_P(logo, false);
            menu_t *menu     = CURRENT_MENU;
            char    title[7] = "     \0\0";
            sprintf(title, "%-5s", menu->title);
            title[5] = '\n';
            oled_write(title, false);
            /* uint8_t b = eeprom_read_byte((uint8_t*)EECONFIG_SIZE); */
            /* eeprom_update_byte((uint8_t*)EECONFIG_SIZE, ++b); */
            /* char dbg[6]; */
            /* sprintf(dbg, "B: %d\n", b); */
            /* oled_write(dbg, false); */
            for (uint8_t i = menu->scroll; i < (menu->count >= MAX_ITEMS ? menu->scroll + MAX_ITEMS : menu->count); i++) {
                char buf[6] = "     \0";
                sprintf(buf, "%-5s", menu->items[i].title);
                oled_write(buf, menu->current == i);
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
