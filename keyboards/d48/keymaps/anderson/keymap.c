#include "d48.h"
#include "taphold.h"
#include "seq.h"
#include "layout_defs.h"
#include <stdio.h>

// Note: don't forget there's some more code in qmk_firmware/users/anderson dir

#define _MAIN 0
#define _ALPHA 1
#define _BETA 2

enum custom_keycodes {
    KC_MAIN = SAFE_RANGE,
    KC_ALPHA,
    KC_BETA,
#ifdef LIGHTMODE_ENABLE
    KC_LIGHT_MODE,
#endif
    KC_SEQ
};
#ifdef LIGHTMODE_ENABLE
#endif

// TapHold is my own implementation of the `LT` macro. It's processed in `process_record_user()`.
#define TAPHOLD_CONFIG_SIZE 3
taphold_t taphold_config[TAPHOLD_CONFIG_SIZE] = {
    {.key=KC_ALPHA, .mode=TAPHOLD_LAYER, .shortAction=KC_ESC, .longAction=_ALPHA},
    {.key=KC_BETA, .mode=TAPHOLD_LAYER, .shortAction=KC_EQL, .longAction=_BETA},
    {.key=KC_RCTRL, .mode=TAPHOLD_MOD, .shortAction=KC_MINS, .longAction=KC_LCTRL},
};
uint16_t taphold_config_size = TAPHOLD_CONFIG_SIZE;
uint32_t taphold_timeout = 90;

// Seq is implementation of unicode macros similar to UCIS, but with unicode strings.
#define SEQ_CONFIG_SIZE 3
seq_t seq_config[SEQ_CONFIG_SIZE] = {
    {.sequence="temp", .result="42°C"},
    {.sequence="table", .result="┳━━┳"},
    {.sequence="shrug", .result="¯\\_(ツ)_/¯"}
};
uint16_t seq_config_size = SEQ_CONFIG_SIZE;

// Colors
uint32_t layer_colors[3] = {
    [_MAIN] = 0xFF0010,
    [_ALPHA] = 0x4020FF,
    [_BETA] = 0x20FF00,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Main layer
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃ TAB ┃  Q  │  W  │  E  │  R  │  T  ┃  Y  │  U  │  I  │  O  │  P  ┃ BSP ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────╂─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃𝛼/ESC┃  A  │  S  │  D  │  F  │  G  ┃  H  │  J  │  K  │  L  │  ;  ┃ RET ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────╂─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃SHIFT┃  Z  │  X  │  C  │  V  │  B  ┃  N  │  M  │  ,  │  .  │  /  ┃CTL/-┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────╂─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃LCTRL┃     │     │ ALT │ GUI │SPACE┃SPACE│ 𝛽/= │  '  │     │     ┃     ┃
       ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛
       */
    [_MAIN] = LAYOUT_kc( \
        TAB,  Q,    W,    E,    R,    T,    Y,    U,    I,    O,    P,    BSPC, \
        ALPHA,A,    S,    D,    F,    G,    H,    J,    K,    L,    SCLN, ENT,  \
        LSFT, Z,    X,    C,    V,    B,    N,    M,    COMM, DOT,  SLSH, RCTRL,\
        LCTRL,_____,_____,LALT, LGUI, SPC,  SPC,  BETA, QUOT, _____,_____,_____ \
    ),

    /* Alpha layer (𝛼)
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃     ┃PREV │PLAY │NEXT │     │     ┃  -  │ ^^^ │  ^  │ vvv │  ~  ┃ DEL ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────╂─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃     │VOL -│VOL +│     │     ┃HOME │ <-- │  v  │ --> │  `  ┃  \  ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────╂─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃     │     │     │     │     ┃ END │  =  │  [  │  ]  │  (  ┃  )  ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────╂─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃     │     │     │     │     ┃     │     │     │     │     ┃     ┃
       ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛
       */
    [_ALPHA] = LAYOUT_kc( \
        _____,MPRV, MPLY, MNXT, _____,_____,MINS, PGUP, UP,   PGDN, TILD, DEL,  \
        _____,_____,VOLD, VOLU, _____,_____,HOME, LEFT, DOWN, RIGHT,GRV,  BSLS, \
        _____,_____,_____,_____,_____,_____,END,  EQL,  LBRC, RBRC, LPRN ,RPRN, \
        _____,_____,_____,_____,_____,_____,_____,_____,_____,_____,_____,_____ \
    ),

    /* Beta layer (𝛽)
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃ RGB ┃  1  │  2  │  3  │  4  │  5  ┃  6  │  7  │  8  │  9  │  0  ┃ F12 ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────╂─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃L_MOD┃ F1  │ F2  │ F3  │ F4  │ F5  ┃ F6  │ F7  │ F8  │ F9  │ F10 ┃ F11 ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────╂─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃RESET│DEBUG│     │     │     ┃SLEEP│ SEQ │  {  │  }  │PTSCR┃     ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────╂─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃     │     │     │     │     ┃     │     │     │     │     ┃     ┃
       ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛
       */
    [_BETA] = LAYOUT_kc( \
        RGB_TOG,1,  2,    3,   4,    5,     6,    7,    8,    9,    0,    F12,
#ifdef LIGHTMODE_ENABLE
        LIGHT_MODE,
#else
        _____,
#endif
              F1,   F2,   F3,   F4,   F5,   F6,   F7,   F8,   F9,   F10,  F11,  \
        _____,RESET,DEBUG,_____,_____,_____,SLEP, SEQ,  LCBR, RCBR, PSCR, _____,\
        _____,_____,_____,_____,_____,_____,_____,_____,_____,_____,_____,_____ \
    )
};

static bool alpha_pressed = false;
static bool beta_pressed = false;

static bool ctrl_pressed = false;
static bool alt_pressed = false;
static bool shift_pressed = false;
static bool gui_pressed = false;

static bool is_in_seq = false;

void keyboard_post_init_user(void) {
    // debug_enable = true;
    // debug_matrix = true;
}

void eeconfig_init_user(void) {
    set_unicode_input_mode(UC_LNX);
}

void matrix_init_user(void) {
#ifdef LIGHTMODE_ENABLE
    set_light_mode(SMOOTHLED, layer_colors[_MAIN]);
#endif
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == KC_SEQ && record->event.pressed) {
        seq_start();
        layer_off(_BETA);
        is_in_seq = true;
        return false;
    }
    if (is_in_seq) {
        if (record->event.pressed) {
            if (!seq_feed(keycode)) {
                is_in_seq = false;
            }
        }
        return false;
    }
    if (keycode == KC_LCTRL || keycode == KC_RCTRL) {
        ctrl_pressed = record->event.pressed;
    } else if (keycode == KC_LALT) {
        alt_pressed = record->event.pressed;
    } else if (keycode == KC_LSFT) {
        shift_pressed = record->event.pressed;
    } else if (keycode == KC_LGUI) {
        gui_pressed = record->event.pressed;
    } else if (keycode == KC_ALPHA) {
        alpha_pressed = record->event.pressed;
    } else if (keycode == KC_BETA) {
        beta_pressed = record->event.pressed;
    }

    if (keycode == RESET) {
        rgblight_setrgb(255, 255, 0);
    }
#ifdef LIGHTMODE_ENABLE
    if (record->event.pressed && keycode == KC_LIGHT_MODE) {
        next_light_mode(layer_colors[_MAIN]);
    }
#endif
    if (keycode == KC_LCTRL) {
        // Some Overlay1_Enable fuckery!
        (record->event.pressed ? register_code : unregister_code)(KC_LCTRL);
        return false;
    }
    return taphold_process(keycode, record);
}

uint32_t layer_state_set_user(uint32_t state) {
#ifdef LIGHTMODE_ENABLE
    uint8_t layer = biton32(state);
    update_light_mode(layer_colors[layer]);
#endif
    // smoothled_set(layer_colors[layer]);
    return state;
}

void encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        if (!alpha_pressed) {
            tap_code(clockwise ? KC_VOLD : KC_VOLU);
        } else {
            tap_code(clockwise ? KC_MPRV : KC_MNXT);
        }
    } else if (index == 1) {
        if (!alpha_pressed) {
            tap_code(clockwise ? KC_UP : KC_DOWN);
        } else {
            tap_code(clockwise ? KC_LEFT : KC_RIGHT);
        }
    }
}

#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_0;
}

void oled_task_user(void) {
    // Host Keyboard Layer Status
    uint8_t current_layer = biton32(layer_state);

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
    for (int i = 0; i < 3; i++) {
        oled_set_cursor(0, i + 1);
        oled_write_P(icons[icon_index][i], false);
    }

    oled_set_cursor(6, 1);
    oled_write_P(PSTR("-D48 Custom-\n"), false);

    uint8_t layer_index = current_layer == _MAIN ? 0 : current_layer == _ALPHA ? 1 : 2;
    const char *layers[3] = {
        PSTR("MAIN"),
        PSTR("ALPHA"),
        PSTR("BETA")
    };
    oled_set_cursor(6, 2);
    oled_write_P("Layer: ", false);
    oled_write_P(layers[layer_index], false);
    oled_write_P("\n", false);

    static const char PROGMEM mods[][2] = {
        {0x94, 0x95}, // CTL
        {0x96, 0x97}, // ALT
        {0x98, 0x99}, // GUI
        {0x9a, 0x9b},  // SFT
        // {0x9c, 0x9d},  // EMPTY
    };

    char mod_data[13] = "\x9c\x9d\x9c\x9d\x9c\x9d\x9c\x9d \x07\x07\x07\0";
    if (ctrl_pressed) strncpy(mod_data, mods[0], 2);;
    if (alt_pressed) strncpy(mod_data + 2, mods[1], 2);;
    if (gui_pressed) strncpy(mod_data + 4, mods[2], 2);;
    if (shift_pressed) strncpy(mod_data + 6, mods[3], 2);;
    uint8_t led_usb_state = host_keyboard_leds();
    if (led_usb_state & (1 << USB_LED_NUM_LOCK)) mod_data[9] = 'N';
    if (led_usb_state & (1 << USB_LED_CAPS_LOCK)) mod_data[10] = 'C';
    if (led_usb_state & (1 << USB_LED_SCROLL_LOCK)) mod_data[11] = 'S';

    oled_set_cursor(6, 3);
    oled_write(mod_data, false);

    /*[>uint8_t led_usb_state = host_keyboard_leds();<]*/
    /*[>oled_write_P(led_usb_state & (1<<USB_LED_NUM_LOCK) ? PSTR("NUMLCK ") : PSTR("       "), false);<]*/
    /*[>oled_write_P(led_usb_state & (1<<USB_LED_CAPS_LOCK) ? PSTR("CAPLCK ") : PSTR("       "), false);<]*/
    /*[>oled_write_P(led_usb_state & (1<<USB_LED_SCROLL_LOCK) ? PSTR("SCRLCK ") : PSTR("       "), false);<]*/
}
#endif

