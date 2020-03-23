#include "d48.h"
#include "taphold.h"
#include "smoothled.h"
#include "dmc12.h"
#include "seq.h"
#include <stdio.h>
/*#include "pro_micro.h"*/

#define KC______ KC_TRNS
#define KC_DEBUG DEBUG
#define KC_RESET RESET
#define KC_RGB_TOG RGB_TOG
#define KC_LIGHT_MODE LIGHT_MODE

#define _MAIN 0
#define _ALPHA 1
#define _BETA 2

enum custom_keycodes {
    KC_MAIN = SAFE_RANGE,
    KC_ALPHA,
    KC_BETA,
    KC_LIGHT_MODE,
    KC_SEQ
};

// TapHold is my own implementation of the `LT` macro. It's processed in `process_record_user()`.
#define TAPHOLD_CONFIG_SIZE 3
taphold_t taphold_config[TAPHOLD_CONFIG_SIZE] = {
    {.key=KC_ALPHA, .mode=TAPHOLD_LAYER, .shortAction=KC_ESC, .longAction=_ALPHA},
    {.key=KC_BETA, .mode=TAPHOLD_LAYER, .shortAction=KC_EQL, .longAction=_BETA},
    {.key=KC_RCTRL, .mode=TAPHOLD_MOD, .shortAction=KC_MINS, .longAction=KC_LCTRL},
    /*{.key=KC_V, .mode=TAPHOLD_MOD, .shortAction=KC_V, .longAction=KC_LALT},*/
    /*{.key=KC_B, .mode=TAPHOLD_MOD, .shortAction=KC_B, .longAction=KC_LGUI}*/
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
    /*[_ALPHA] = 0xFF0040,*/
    [_ALPHA] = 0x4020FF,
    [_BETA] = 0x20FF00,
};

// Light modes
enum light_mode_enum { SMOOTHLED, DMC12, LIGHT_MODE_SIZE };
typedef enum light_mode_enum light_mode_t;
uint8_t light_mode = SMOOTHLED;

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
        RGB_TOG,1,  2,    3,   4,    5,     6,    7,    8,    9,    0,   F12,  \
        LIGHT_MODE,F1,F2, F3,   F4,   F5,   F6,   F7,   F8,   F9,   F10,  F11,  \
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

void set_light_mode(light_mode_t value, uint32_t color) {
    light_mode = value;
    if (light_mode == SMOOTHLED) {
        smoothled_set(color);
    } else {
        dmc12_start(color, true);
    }
}

void process_light_mode(void) {
    if (light_mode == SMOOTHLED) {
        smoothled_process();
    } else {
        dmc12_process();
    }
}

void update_light_mode(uint32_t color) {
    if (light_mode == SMOOTHLED) {
        smoothled_set(color);
    } else {
        dmc12_start(color, false);
    }
}

void eeconfig_init_user(void) {
    set_unicode_input_mode(UC_LNX);
}

void matrix_init_user(void) {
    set_light_mode(SMOOTHLED, layer_colors[_MAIN]);
}

void matrix_scan_user(void) {
    process_light_mode();
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
    if (record->event.pressed && keycode == LIGHT_MODE) {
        light_mode = (light_mode + 1) % LIGHT_MODE_SIZE;
        set_light_mode(light_mode, layer_colors[_MAIN]);
    }
    if (keycode == KC_LCTRL) {
        // Some Overlay1_Enable fuckery!
        (record->event.pressed ? register_code : unregister_code)(KC_LCTRL);
        return false;
    }
    return taphold_process(keycode, record);
}

uint32_t layer_state_set_user(uint32_t state) {
    uint8_t layer = biton32(state);
    update_light_mode(layer_colors[layer]);
    // smoothled_set(layer_colors[layer]);
    return state;
}

void encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        /*if (alpha_pressed) {*/
        /*    if (clockwise) {*/
        /*        tap_code(KC_VOLD);*/
        /*    } else {*/
        /*        tap_code(KC_VOLU);*/
        /*    }*/
        /*} else {*/
        /*    if (clockwise) {*/
        /*        tap_code(KC_UP);*/
        /*    } else {*/
        /*        tap_code(KC_DOWN);*/
        /*    }*/
        /*}*/
        if (clockwise) {
            tap_code(KC_VOLD);
        } else {
            tap_code(KC_VOLU);
        }
    } else if (index == 1) {
        if (clockwise) {
            tap_code(KC_UP);
        } else {
            tap_code(KC_DOWN);
        }
    }
}

#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_0;
    /*return OLED_ROTATION_270;*/
    if (!is_keyboard_master())
        return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
    return rotation;
}

/*static uint32_t last_fps_update = 0;*/
/*static uint32_t last_render = 0;*/

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
        /*for (int k = 0; k < 5; k++) {*/
        /*    oled_write_char(icons[icon_index][i][k], false);*/
        /*}*/
        /*oled_write_P(icons[icon_index][i], current_layer != _MAIN);*/
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

    char mod_data[6] = "\x07\x07\x07\x07\x07\0";
    if (ctrl_pressed) mod_data[0] = 'C';
    if (alt_pressed) mod_data[1] = 'A';
    if (shift_pressed) mod_data[3] = 'S';
    if (gui_pressed) mod_data[4] = 'G';
    oled_set_cursor(6, 3);
    oled_write(mod_data, false);

    /*uint32_t this_render = timer_read32();*/

    /*if (this_render - last_fps_update > 250) {*/
    /*    last_fps_update = this_render;*/
    /*    char fps_buf[16];*/
    /*    uint16_t fps = 1000 / (this_render - last_render);*/
    /*    sprintf(fps_buf, "%d fps", fps);*/
    /*    oled_write_ln(fps_buf, false);*/
    /*}*/
    /*last_render = this_render;*/

    // Host Keyboard LED Status
    /*[>uint8_t led_usb_state = host_keyboard_leds();<]*/
    /*[>oled_write_P(led_usb_state & (1<<USB_LED_NUM_LOCK) ? PSTR("NUMLCK ") : PSTR("       "), false);<]*/
    /*[>oled_write_P(led_usb_state & (1<<USB_LED_CAPS_LOCK) ? PSTR("CAPLCK ") : PSTR("       "), false);<]*/
    /*[>oled_write_P(led_usb_state & (1<<USB_LED_SCROLL_LOCK) ? PSTR("SCRLCK ") : PSTR("       "), false);<]*/
}
#endif

