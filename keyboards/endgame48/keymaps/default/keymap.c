#include "endgame48.h"
#include "taphold.h"
#include "smoothled.h"
#include "quantum.h"

#define KC______ KC_TRNS
#define KC_K1 KC_1
#define KC_K2 KC_2
#define KC_K3 KC_3
#define KC_K4 KC_4
#define KC_K5 KC_5
#define KC_K6 KC_6
#define KC_K7 KC_7
#define KC_K8 KC_8
#define KC_K9 KC_9
#define KC_K0 KC_0
#define SH(x) LSFT(KC_##x)
#define KC_SH SH
#define KC_DEBUG DEBUG
#define KC_RESET RESET
#define KC_RGB_TOG RGB_TOG
#define KC_RGB_MOD RGB_MOD
#define KC_MO MO

#define _MAIN 0
#define _ALPHA 1
#define _BETA 2
#define _SIGMA 3

enum custom_keycodes {
  KC_MAIN = SAFE_RANGE,
  KC_ALPHA,
  KC_BETA,
  KC_SIGMA
};

// TapHold is my own implementation of the `LT` macro. It's processed in `process_record_user()`.
#define TAPHOLD_CONFIG_SIZE 4
taphold_t taphold_config[TAPHOLD_CONFIG_SIZE] = {
    {.key=KC_ALPHA, .mode=TAPHOLD_LAYER, .shortAction=KC_ESC, .longAction=_ALPHA},
    {.key=KC_BETA, .mode=TAPHOLD_LAYER, .shortAction=KC_EQL, .longAction=_BETA},
    {.key=KC_SIGMA, .mode=TAPHOLD_LAYER, .shortAction=KC_MUTE, .longAction=_SIGMA},
    {.key=KC_LCTRL, .mode=TAPHOLD_MOD, .shortAction=KC_MINS, .longAction=KC_LCTRL},
    /*{.key=KC_V, .mode=TAPHOLD_MOD, .shortAction=KC_V, .longAction=KC_LALT},*/
    /*{.key=KC_B, .mode=TAPHOLD_MOD, .shortAction=KC_B, .longAction=KC_LGUI}*/
};
uint16_t taphold_config_size = TAPHOLD_CONFIG_SIZE;
uint32_t taphold_timeout = 100;

uint32_t layer_colors[4] = {
    [_MAIN] = 0x4020FF,
    /*[_ALPHA] = 0xFF0040,*/
    [_ALPHA] = 0xFF0010,
    [_BETA] = 0x20FF00,
    [_SIGMA] = 0x40FFFF
};
#define DEBUG_COLOR 0xFFFF00

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Main layer
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┓┏┳━┳┓┏━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃ TAB ┃  Q  │  W  │  E  │  R  │  T  ┃┃┃ ┃┃┃  Y  │  U  │  I  │  O  │  P  ┃ BSP ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨┃┃ ┃┃┣─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃𝛼/ESC┃  A  │  S  │  D  │  F  │  G  ┃┃┃ ┃┃┃  H  │  J  │  K  │  L  │  ;  ┃ RET ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨┗┻━┻┛┣─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃SHIFT┃  Z  │  X  │  C  │  V  │  B  ┃     ┃  N  │  M  │  ,  │  .  │  /  ┃LCTRL┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨     ┣─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃CTRL ┃     │     │ ALT │ GUI │SPACE┃ (o) ┃SPACE│ 𝛽/= │  '  │     │     ┃     ┃
       ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┛     ┗━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛
       */
    [_MAIN] = LAYOUT_kc( \
        TAB,  Q,    W,    E,    R,    T,          Y,    U,    I,    O,    P,    BSPC, \
        ALPHA,A,    S,    D,    F,    G,          H,    J,    K,    L,    SCLN, ENT,  \
        LSFT, Z,    X,    C,    V,    B,          N,    M,    COMM, DOT,  SLSH, LCTRL,\
        LCTRL,_____,_____,LALT, LGUI, SPC,        SPC,  BETA, QUOT, _____,_____,_____,\
        SIGMA \
    ),

    /* 𝛼 layer
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┓┏┳━┳┓┏━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃     ┃PREV │PLAY │NEXT │     │     ┃┃┃ ┃┃┃  -  │ ^^^ │  ^  │ vvv │  ~  ┃ DEL ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨┃┃ ┃┃┣─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃     │VOL -│VOL +│     │     ┃┃┃ ┃┃┃HOME │ <-- │  v  │ --> │  `  ┃  \  ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨┗┻━┻┛┣─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃     │     │     │     │     ┃     ┃ END │  =  │  [  │  ]  │  (  ┃  )  ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨     ┣─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃     │     │     │     │     ┃ (o) ┃     │     │     │     │     ┃     ┃
       ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┛     ┗━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛
       */
    [_ALPHA] = LAYOUT_kc( \
        _____,MPRV, MPLY, MNXT, _____,_____,      MINS, PGUP, UP,   PGDN, TILD, DEL,  \
        _____,_____,VOLD, VOLU, _____,_____,      HOME, LEFT, DOWN, RIGHT,GRV,  BSLS, \
        _____,_____,_____,_____,_____,_____,      END,  EQL,  LBRC, RBRC, SH(9),SH(0),\
        _____,_____,_____,_____,_____,_____,      _____,_____,_____,_____,_____,_____,\
        _____
    ),

    /* 𝛽 layer
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┓┏┳━┳┓┏━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃ RGB ┃  1  │  2  │  3  │  4  │  5  ┃┃┃ ┃┃┃  6  │  7  │  8  │  9  │  0  ┃ F12 ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨┃┃ ┃┃┣─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃RGB_M┃ F1  │ F2  │ F3  │ F4  │ F5  ┃┃┃ ┃┃┃ F6  │ F7  │ F8  │ F9  │ F10 ┃ F11 ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨┗┻━┻┛┣─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃RESET│DEBUG│SLEEP│WAKE │     ┃     ┃     │     │  {  │  }  │     ┃     ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨     ┣─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃     │     │     │     │     ┃ (o) ┃PTSCR│     │     │     │     ┃     ┃
       ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┛     ┗━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛
       */
    [_BETA] = LAYOUT_kc( \
        RGB_TOG,K1, K2,   K3,   K4,   K5,         K6,   K7,   K8,   K9,   K0,   F12,  \
        RGB_MOD,F1, F2,   F3,   F4,   F5,         F6,   F7,   F8,   F9,   F10,  F11,  \
        _____,RESET,DEBUG,PWR,  WAKE, _____,      _____,_____,LCBR, RCBR, _____,_____,\
        _____,_____,_____,_____,_____,_____,      PSCR, _____,_____,_____,_____,_____,\
        _____
    ),
    /* Σ layer
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┓┏┳━┳┓┏━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃     ┃     │     │     │     │     ┃┃┃ ┃┃┃     │     │     │     │     ┃     ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨┃┃ ┃┃┣─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃     │     │     │     │     ┃┃┃ ┃┃┃     │     │     │     │     ┃     ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨┗┻━┻┛┣─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃     │     │     │     │     ┃     ┃     │     │     │     │     ┃     ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨     ┣─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃     │     │     │     │     ┃ (o) ┃     │     │     │     │     ┃     ┃
       ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┛     ┗━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛
       */
    [_SIGMA] = LAYOUT_kc( \
        _____,_____,_____,_____,_____,_____,_____,_____,_____,_____,_____,_____,\
        _____,_____,_____,_____,_____,_____,_____,_____,_____,_____,_____,_____,\
        _____,_____,_____,_____,_____,_____,_____,_____,_____,_____,_____,_____,\
        _____,_____,_____,_____,_____,_____,_____,_____,_____,_____,_____,_____,\
        _____
    ),
};

// static uint32_t last_key_pressed = 0;

// void rxtx_init(void) {
//     TX_RX_LED_INIT;
// }

// void rxtx_tap(keyrecord_t *record) {
//     last_key_pressed = timer_read32();
//     if (record->event.key.row < 4) {
//         RXLED1;
//     } else {
//         TXLED1;
//     }
// }

// void rxtx_process(void) {
//     if (timer_elapsed(last_key_pressed) > 0) {
//         RXLED0;
//         TXLED0;
//     }
// }

static bool alpha_pressed = false;
static bool beta_pressed = false;
static bool sigma_pressed = false;
static uint8_t keys_pressed = 0;
/*static uint16_t last_keycode = 0;*/

/*static uint32_t last_scan = 0;*/
/*static uint16_t sps = 0;*/
/*static uint16_t avg_delta = 0;*/
/*static uint32_t last_sps_update = 0;*/

static bool ctrl_pressed = false;
static bool alt_pressed = false;
static bool shift_pressed = false;
static bool gui_pressed = false;

/*static char history[6] = {0};*/

/*static char charmap[] = {*/
/*    0, 0, 0, 0,*/
/*    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',*/
/*    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',*/
/*    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',*/
/*    0,*/
/*    0, 0, 0, ' ', 0, 0, 0, 0, '\\', 0,*/
/*    ':', 0, '`', ',', '.', '/'*/
/*};*/
/*const uint8_t charmap_size = sizeof(charmap) / sizeof(charmap[0]);*/

/*void add_to_history(uint16_t code) {*/
/*    // a-z0-9: 4-39*/
/*    // -: 224````````*/
/*    // `: 53*/
/*    // ~: 565*/
/*    //  : 44*/
/*    // :: 51*/
/*    // ,./: 54-56*/
/*    // BS: 42*/
/*    // RET: 48*/
/*    // \: 49*/

/*    int i = 0;*/
/*    for (; i < 5 && history[i]; i++);*/

/*    if (code == 42) {*/
/*        if (i) {*/
/*            history[i - 1] = 0;*/
/*        }*/
/*    } else if (code == 40) {*/
/*        while(i > 0) {*/
/*            history[--i] = 0;*/
/*        }*/
/*    } else if (code < charmap_size && charmap[code]) {*/
/*        if (i == 5) {*/
/*            for (int k = 0; k < i; k++) {*/
/*                history[k] = history[k + 1];*/
/*            }*/
/*            i--;*/
/*        }*/
/*        history[i] = charmap[code];*/
/*    }*/
/*}*/

void matrix_init_user(void) {
    smoothled_set(layer_colors[_MAIN]);
    // rxtx_init();
}

void matrix_scan_user(void) {
    smoothled_process();
    // rxtx_process();
    /*uint32_t now = timer_read32();*/
    /*uint32_t delta = now - last_scan;*/
    /*if (now - last_sps_update > 250) {*/
    /*    avg_delta = delta;*/
    /*    sps = 1000 / delta;*/
    /*    last_sps_update = now;*/
    /*}*/
    /*last_scan = timer_read32();*/
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    /*if (keycode == KC_BETA && record->event.pressed) {*/
    /*    print("Foo\n");*/
    /*}*/
    if (record->event.pressed) {
        keys_pressed++;
        /*last_keycode = keycode;*/
        /*add_to_history(keycode);*/
    } else {
        keys_pressed--;
        /*last_keycode = 0;*/
    }

    if (keycode == KC_LCTRL) {
        ctrl_pressed = record->event.pressed;
    } else if (keycode == KC_LALT) {
        alt_pressed = record->event.pressed;
    } else if (keycode == KC_LSFT) {
        shift_pressed = record->event.pressed;
    } else if (keycode == KC_LGUI) {
        gui_pressed = record->event.pressed;
    }

    if (keycode == RESET) {
        rgblight_setrgb((DEBUG_COLOR >> 16) & 0xFF, (DEBUG_COLOR >> 8) & 0xFF, DEBUG_COLOR & 0xFF);
    }
    if (keycode == KC_ALPHA) {
        alpha_pressed = record->event.pressed;
    }
    if (keycode == KC_BETA) {
        beta_pressed = record->event.pressed;
    }
    if (keycode == KC_SIGMA) {
        sigma_pressed = record->event.pressed;
    }
    // if (record->event.pressed) {
    //     rxtx_tap(record);
    // }
    return taphold_process(keycode, record);
}

uint32_t layer_state_set_user(uint32_t state) {
    uint8_t layer = biton32(state);
    smoothled_set(layer_colors[layer]);
    return state;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        if (alpha_pressed || sigma_pressed) {
            if (clockwise) {
                tap_code(KC_VOLD);
            } else {
                tap_code(KC_VOLU);
            }
        } else if (beta_pressed) {
            if (clockwise) {
                tap_code(KC_LEFT);
            } else {
                tap_code(KC_RIGHT);
            }
        } else {
            if (clockwise) {
                tap_code(KC_MS_WH_UP);
            } else {
                tap_code(KC_MS_WH_DOWN);
            }
        }
    }
    return false;
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
   return OLED_ROTATION_90;
}

/*static uint32_t last_render = 0;*/

// static uint32_t last_invert = 0;
// static bool is_inverted = false;

bool oled_task_user(void) {
    // Host Keyboard Layer Status
    oled_write_P(PSTR("\n\nEG 48\n\n"), false);
    uint8_t current_layer = biton32(layer_state);
    // oled_write_P(PSTR("     "), current_layer == _MAIN);
    // oled_write_P(current_layer == _MAIN ? PSTR("< 0 >") : PSTR("  0  "), current_layer == _MAIN);
    // oled_write_P(PSTR("     "), current_layer == _MAIN || current_layer == _ALPHA);
    // oled_write_P(current_layer == _ALPHA ? PSTR("< A >") : PSTR("  A  "), current_layer == _ALPHA);
    // oled_write_P(PSTR("     "), current_layer == _ALPHA || current_layer == _BETA);
    // oled_write_P(current_layer == _BETA ? PSTR("< B >") : PSTR("  B  "), current_layer == _BETA);
    // oled_write_P(PSTR("     "), current_layer == _BETA);

    static const char PROGMEM layers[4][26] = {
       {
           ' ', ' ', ' ', ' ', ' ',
           0x80, 0x81, 0x82, 0x83, 0x84,
           0xa0, 0xa1, 0xa2, 0xa3, 0xa4,
           0xc0, 0xc1, 0xc2, 0xc3, 0xc4,
           ' ', ' ', ' ', ' ', ' ', 0
       },
       {
           ' ', ' ', ' ', ' ', ' ',
           0x85, 0x86, 0x87, 0x88, 0x89,
           0xa5, 0xa6, 0xa7, 0xa8, 0xa9,
           0xc5, 0xc6, 0xc7, 0xc8, 0xc9,
           ' ', ' ', ' ', ' ', ' ', 0
       },
       {
           ' ', ' ', ' ', ' ', ' ',
           0x8a, 0x8b, 0x8c, 0x8d, 0x8e,
           0xaa, 0xab, 0xac, 0xad, 0xae,
           0xca, 0xcb, 0xcc, 0xcd, 0xce,
           ' ', ' ', ' ', ' ', ' ', 0
       },
       {
           ' ', ' ', ' ', ' ', ' ',
           0x8f, 0x90, 0x91, 0x92, 0x93,
           0xaf, 0xb0, 0xb1, 0xb2, 0xb3,
           0xcf, 0xd0, 0xd1, 0xd2, 0xd3,
           ' ', ' ', ' ', ' ', ' ', 0
       }
    };

    // uint32_t now = timer_read32();
    // uint32_t diff = now - last_invert;
    // if (diff > 40) {
    //     last_invert = now;
    //     is_inverted = !is_inverted;
    // }

    oled_write_P(layers[current_layer], current_layer != _MAIN); // && is_inverted);

    oled_write_P(PSTR("\n\n"), false);

    /*char key_data[4] = {' ', ' ', '0' + keys_pressed, 0};*/
    /*oled_write(key_data, false);*/

    /*char sps_data[6] = {0};*/
    /*sprintf(sps_data, " %d", sps);*/
    /*[>sprintf(data, "%d", avg_delta);<]*/
    /*oled_write(sps_data, false);*/
    /*oled_write_P(PSTR("\n\n"), false);*/

    /*char str[6] = {0};*/
    /*if (last_keycode) {*/
    /*    sprintf(str, "%d", last_keycode);*/
    /*}*/
    /*oled_write(str, false);*/

    /*oled_write(history, false);*/
    /*if (!history[4]) {*/
    /*    oled_write_P(PSTR("\n"), false);*/
    /*}*/

    char mod_data[6] = "\x07\x07\x07\x07\x07\0";
    if (ctrl_pressed) mod_data[0] = 'C';
    if (alt_pressed) mod_data[1] = 'A';
    if (shift_pressed) mod_data[3] = 'S';
    if (gui_pressed) mod_data[4] = 'G';
    oled_write(mod_data, false);

    /*if (last_keycode) {*/
    /*    char data[6] = {0};*/
    /*    if (last_keycode == KC_ALPHA) {*/
    /*        memcpy(data, "<A>", 3);*/
    /*    } else if (last_keycode == KC_BETA) {*/
    /*        memcpy(data, "<B>", 3);*/
    /*    } else {*/
    /*        sprintf(data, "%d", last_keycode);*/
    /*    }*/
    /*    oled_write_ln(data, false);*/
    /*} else {*/
    /*    oled_write_ln("", false);*/
    /*}*/

    // Host Keyboard LED Status
    /*uint8_t led_usb_state = host_keyboard_leds();*/
    /*oled_write_P(led_usb_state & (1<<USB_LED_NUM_LOCK) ? PSTR("NUMLCK ") : PSTR("       "), false);*/
    /*oled_write_P(led_usb_state & (1<<USB_LED_CAPS_LOCK) ? PSTR("CAPLCK ") : PSTR("       "), false);*/
    /*oled_write_P(led_usb_state & (1<<USB_LED_SCROLL_LOCK) ? PSTR("SCRLCK ") : PSTR("       "), false);*/

    return false;
}
/*#endif*/

