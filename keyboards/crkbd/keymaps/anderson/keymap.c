/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include "taphold.h"

#define _MAIN 0
#define _ALPHA 1
#define _BETA 2

enum custom_keycodes {
    KC_MAIN = SAFE_RANGE,
    KC_ALPHA,
    KC_BETA,
    KC_SET_TIME,
};

#define TAPHOLD_CONFIG_SIZE 3
taphold_t taphold_config[TAPHOLD_CONFIG_SIZE] = {
    {.key=KC_ALPHA, .mode=TAPHOLD_LAYER, .shortAction=KC_ESC, .longAction=_ALPHA},
    {.key=KC_BETA, .mode=TAPHOLD_LAYER, .shortAction=KC_EQL, .longAction=_BETA},
    {.key=KC_RCTRL, .mode=TAPHOLD_MOD, .shortAction=KC_MINS, .longAction=KC_LCTRL},
};
uint16_t taphold_config_size = TAPHOLD_CONFIG_SIZE;
uint32_t taphold_timeout = 90;

static volatile bool ctrl_pressed = false;
static volatile bool alt_pressed = false;
static volatile bool shift_pressed = false;
static volatile bool gui_pressed = false;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Main layer
                                                       │MUTE │           │L_MOD│
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃ TAB ┃  Q  │  W  │  E  │  R  │  T  ┃  Y  │  U  │  I  │  O  │  P  ┃ BSP ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────╂─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃𝛼/ESC┃  A  │  S  │  D  │  F  │  G  ┃  H  │  J  │  K  │  L  │  ;  ┃ RET ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────╂─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃SHIFT┃  Z  │  X  │  C  │  V  │  B  ┃  N  │  M  │  ,  │  .  │  /  ┃CTL/-┃
       ┗━━━━━┻─────┼─────┼─────┼─────┼─────╂─────┼─────┼─────┼─────┼─────┻━━━━━┛
                         │ ALT │ GUI │SPACE┃SPACE│ 𝛽/= │  '  │
                         ┷━━━━━┷━━━━━┷━━━━━┻━━━━━┷━━━━━┷━━━━━┷
       */
    [_MAIN] = LAYOUT_split_3x6_3( \
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC, \
        KC_ALPHA,KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_ENT,  \
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RCTRL,\
                                   KC_LALT, KC_LGUI, KC_SPC,  KC_SPC,  KC_BETA, KC_QUOT \
    ),

    /* Alpha layer (𝛼)
                                                       │     │           │     │
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃     ┃PREV │PLAY │NEXT │     │NUMLK┃  -  │ ^^^ │  ^  │ vvv │  ~  ┃ DEL ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────╂─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃     │VOL -│VOL +│     │CPSLK┃HOME │ <-- │  v  │ --> │  `  ┃  \  ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────╂─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃     │     │     │     │SCRLK┃ END │  =  │  [  │  ]  │  (  ┃  )  ┃
       ┗━━━━━┻─────┼─────┼─────┼─────┼─────╂─────┼─────┼─────┼─────┼─────┻━━━━━┛
                         │     │     │     ┃     │     │     │
                         ┷━━━━━┷━━━━━┷━━━━━┻━━━━━┷━━━━━┷━━━━━┷
       */
    [_ALPHA] = LAYOUT_split_3x6_3( \
        _______, KC_MPRV, KC_MPLY, KC_MNXT, _______, KC_NLCK, KC_MINS, KC_PGUP, KC_UP,   KC_PGDN, KC_TILD, KC_DEL,  \
        _______, _______, KC_VOLD, KC_VOLU, _______, KC_CAPS, KC_HOME, KC_LEFT, KC_DOWN, KC_RIGHT,KC_GRV,  KC_BSLS, \
        _______, _______, _______, _______, _______, KC_SLCK, KC_END,  KC_EQL,  KC_LBRC, KC_RBRC, KC_LPRN ,KC_RPRN, \
                                   _______, _______, _______, _______, _______, _______ \
    ),

    /* Beta layer (𝛽)
                                                       │     │           │     │
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃ RGB ┃  1  │  2  │  3  │  4  │  5  ┃  6  │  7  │  8  │  9  │  0  ┃ F12 ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────╂─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃L_MOD┃ F1  │ F2  │ F3  │ F4  │ F5  ┃ F6  │ F7  │ F8  │ F9  │ F10 ┃ F11 ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────╂─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃RESET│DEBUG│     │     │TIME ┃SLEEP│     │  {  │  }  │PTSCR┃     ┃
       ┗━━━━━┻─────┼─────┼─────┼─────┼─────╂─────┼─────┼─────┼─────┼─────┻━━━━━┛
                         │     │     │     ┃     │     │     │
                         ┷━━━━━┷━━━━━┷━━━━━┻━━━━━┷━━━━━┷━━━━━┷
       */
    [_BETA] = LAYOUT_split_3x6_3( \
        RGB_TOG, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,     KC_6,    KC_7,   KC_8,    KC_9,    KC_0,    KC_F12,
        _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,    KC_F6,  KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  \
        _______, RESET,   DEBUG,   _______, _______, KC_SET_TIME,KC_SLEP,_______,KC_LCBR,KC_RCBR, KC_PSCR, _______, \
                                   _______, _______, _______, _______, _______, _______ \
    )
};

#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return is_master ? OLED_ROTATION_270 : OLED_ROTATION_180;
}

#define L_BASE 0
#define L_LOWER 2
#define L_RAISE 4
#define L_ADJUST 8

void oled_render_layer_state(void) {
    oled_write_P(PSTR("Layer: "), false);
    switch (layer_state) {
        case L_BASE:
            oled_write_ln_P(PSTR("Default"), false);
            break;
        case L_LOWER:
            oled_write_ln_P(PSTR("Lower"), false);
            break;
        case L_RAISE:
            oled_write_ln_P(PSTR("Raise"), false);
            break;
        case L_ADJUST:
        case L_ADJUST|L_LOWER:
        case L_ADJUST|L_RAISE:
        case L_ADJUST|L_LOWER|L_RAISE:
            oled_write_ln_P(PSTR("Adjust"), false);
            break;
    }
}


char keylog_str[24] = {};

const char code_to_name[60] = {
    ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    'R', 'E', 'B', 'T', '_', '-', '=', '[', ']', '\\',
    '#', ';', '\'', '`', ',', '.', '/', ' ', ' ', ' '};

void set_keylog(uint16_t keycode, keyrecord_t *record) {
  char name = ' ';
    if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) ||
        (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) { keycode = keycode & 0xFF; }
  if (keycode < 60) {
    name = code_to_name[keycode];
  }

  // update keylog
  snprintf(keylog_str, sizeof(keylog_str), "%dx%d, k%2d : %c",
           record->event.key.row, record->event.key.col,
           keycode, name);
}

void oled_render_keylog(void) {
    oled_write(keylog_str, false);
}

void render_bootmagic_status(bool status) {
    /* Show Ctrl-Gui Swap options */
    static const char PROGMEM logo[][2][3] = {
        {{0x97, 0x98, 0}, {0xb7, 0xb8, 0}},
        {{0x95, 0x96, 0}, {0xb5, 0xb6, 0}},
    };
    if (status) {
        oled_write_ln_P(logo[0][0], false);
        oled_write_ln_P(logo[0][1], false);
    } else {
        oled_write_ln_P(logo[1][0], false);
        oled_write_ln_P(logo[1][1], false);
    }
}

void oled_render_logo(void) {
    static const char PROGMEM crkbd_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4,
        0};
    oled_write_P(crkbd_logo, false);
}

void oled_task_user(void) {
    if (is_master) {
        /* Host Keyboard Layer Status */
        uint8_t current_layer = get_highest_layer(layer_state);

        /* Layer */
        static const char PROGMEM icons[4][3][6] = {
            {
                { 0x80, 0x81, 0x82, 0x83, 0x84, 0 },
                { 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0 },
                { 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0 }
            },
            {
                { 0x85, 0x86, 0x87, 0x88, 0x89, 0 },
                { 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0 },
                { 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0 }
            },
            {
                { 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0 },
                { 0xaa, 0xab, 0xac, 0xad, 0xae, 0 },
                { 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0 }
            },
            {
                { 0x8f, 0x90, 0x91, 0x92, 0x93, 0 },
                { 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0 },
                { 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0 }
            }
        };
        uint8_t icon_index = current_layer == _MAIN ? 3 : current_layer == _ALPHA ? 1 : 2;
        oled_write_ln_P(PSTR("Layer"), false);
        for (int i = 0; i < 3; i++) {
            oled_set_cursor(0, i + 4);
            oled_write_P(icons[icon_index][i], false);
        }

        /* Modifiers */
        static const char PROGMEM mods[][2] = {
            {0x94, 0x95}, // CTL
            {0x96, 0x97}, // ALT
            {0x98, 0x99}, // GUI
            {0x9a, 0x9b},  // SFT
            /* {0x9c, 0x9d},  // EMPTY */
        };

        char mod_data[16] = "\x9c\x9d \x9c\x9d\x9c\x9d \x9c\x9d \x07\x07\x07 \0";
        /* char mod_data[13] = "\x9c\x9d\x9c\x9d\x9c\x9d\x9c\x9d \x07\x07\x07\0"; */
        if (ctrl_pressed) memcpy(mod_data, mods[0], 2);
        if (alt_pressed) memcpy(mod_data + 3, mods[1], 2);
        if (gui_pressed) memcpy(mod_data + 5, mods[2], 2);
        if (shift_pressed) memcpy(mod_data + 8, mods[3], 2);
        led_t led_usb_state = host_keyboard_led_state();
        if (led_usb_state.num_lock) mod_data[11] = 'N';
        if (led_usb_state.caps_lock) mod_data[12] = 'C';
        if (led_usb_state.scroll_lock) mod_data[13] = 'S';

        oled_set_cursor(0, 9);
        oled_write(mod_data, false);

    } else {
        /* Matrix */
        static const char matrix_chars[] = {
            0xb4, // None
            0xb5, // Upper
            0xb6, // Lower
            0xb7  // Both
        };

        for (uint8_t row = 0; row < MATRIX_ROWS / 2; row += 2) {
            for (uint8_t half = 0; half < 2; half++) {
                uint16_t bits1 = matrix_get_row(row + half * 4);
                uint16_t bits2 = matrix_get_row(row + 1 + half * 4);

                for (uint8_t col = 0; col < MATRIX_COLS; col++) {
                    // Right half is mirrored
                    uint8_t matrix_col = !half ? col : MATRIX_COLS - col - 1;

                    uint8_t bit1 = (bits1 >> matrix_col) & 1;
                    uint8_t bit2 = (bits2 >> matrix_col) & 1;
                    uint8_t matrix_char = matrix_chars[bit1 | (bit2 << 1)];
                    if (row == 2 && ((!half && col < 3) || (half && col > 3))) {
                        matrix_char = bit1 ? 0xbd : 0xbc;
                    }
                    oled_set_cursor(4 + col + half * 6 + half, 1 + row / 2);
                    oled_write_char(matrix_char, false);
                }
            }
        }
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        set_keylog(keycode, record);
    }
    if (keycode == KC_LCTRL || keycode == KC_RCTRL) {
        ctrl_pressed = record->event.pressed;
    } else if (keycode == KC_LALT) {
        alt_pressed = record->event.pressed;
    } else if (keycode == KC_LSFT) {
        shift_pressed = record->event.pressed;
    } else if (keycode == KC_LGUI) {
        gui_pressed = record->event.pressed;
    }

    return taphold_process(keycode, record);
    // return true;
}
#endif // OLED_DRIVER_ENABLE
