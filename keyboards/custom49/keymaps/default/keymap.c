#include "custom49.h"
#include "taphold.h"
#include "smoothled.h"
#include "pro_micro.h"

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

enum custom_keycodes {
  KC_MAIN = SAFE_RANGE,
  KC_ALPHA,
  KC_BETA
};

// TapHold is my own implementation of the `LT` macro. It's processed in `process_record_user()`.
#define TAPHOLD_CONFIG_SIZE 3
taphold_t taphold_config[TAPHOLD_CONFIG_SIZE] = {
    {.key=KC_ALPHA, .mode=TAPHOLD_LAYER, .shortAction=KC_ESC, .longAction=_ALPHA},
    {.key=KC_BETA, .mode=TAPHOLD_LAYER, .shortAction=KC_EQL, .longAction=_BETA},
    {.key=KC_LCTRL, .mode=TAPHOLD_MOD, .shortAction=KC_MINS, .longAction=KC_LCTRL},
    /*{.key=KC_V, .mode=TAPHOLD_MOD, .shortAction=KC_V, .longAction=KC_LALT},*/
    /*{.key=KC_B, .mode=TAPHOLD_MOD, .shortAction=KC_B, .longAction=KC_LGUI}*/
};
uint16_t taphold_config_size = TAPHOLD_CONFIG_SIZE;
uint32_t taphold_timeout = 90;

uint32_t layer_colors[3] = {
    [_MAIN] = 0xFF0010,
    /*[_ALPHA] = 0xFF0040,*/
    [_ALPHA] = 0x4020FF,
    [_BETA] = 0x20FF00,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Main layer
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃ TAB ┃  Q  │  W  │  E  │  R  │  T  ┃  Y  │  U  │  I  │  O  │  P  ┃ BSP ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃𝛼/ESC┃  A  │  S  │  D  │  F  │  G  ┃  H  │  J  │  K  │  L  │  ;  ┃ RET ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃SHIFT┃  Z  │  X  │  C  │  V  │  B  ┃  N  │  M  │  ,  │  .  │  /  ┃LCTRL┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃CTRL ┃     │     │ ALT │ GUI │SPACE┃SPACE│  𝛽  │  '  │     │     ┃     ┃
       ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛
       */
    [_MAIN] = LAYOUT_kc( \
        TAB,  Q,    W,    E,    R,    T,           Y,    U,    I,    O,    P,    BSPC, \
        ALPHA,A,    S,    D,    F,    G,           H,    J,    K,    L,    SCLN, ENT,  \
        LSFT, Z,    X,    C,    V,    B,           N,    M,    COMM, DOT,  SLSH, LCTRL,\
        LCTRL,_____,_____,LALT, LGUI, SPC,         SPC,  BETA, QUOT, _____,_____,_____ \
    ),

    /* Alpha layer (𝛼)
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃     ┃PREV │PLAY │NEXT │     │     ┃  -  │ ^^^ │  ^  │ vvv │  ~  ┃ DEL ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃     │VOL -│VOL +│     │     ┃HOME │ <-- │  v  │ --> │  `  ┃  \  ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃     │     │     │     │     ┃ END │  =  │  [  │  ]  │  (  ┃  )  ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃     │     │     │     │     ┃     │     │     │     │     ┃     ┃
       ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛
       */
    [_ALPHA] = LAYOUT_kc( \
        _____,MPRV, MPLY, MNXT, _____,_____,MINS, PGUP, UP,   PGDN, TILD, DEL,  \
        _____,_____,VOLD, VOLU, _____,_____,HOME, LEFT, DOWN, RIGHT,GRV,  BSLS, \
        _____,_____,_____,_____,_____,_____,END,  EQL,  LBRC, RBRC, SH(9),SH(0),\
        _____,_____,_____,_____,_____,_____,_____,_____,_____,_____,_____,_____ \
    ),

    /* Beta layer (𝛽)
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃ RGB ┃  1  │  2  │  3  │  4  │  5  ┃  6  │  7  │  8  │  9  │  0  ┃ F12 ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃RGB_M┃ F1  │ F2  │ F3  │ F4  │ F5  ┃ F6  │ F7  │ F8  │ F9  │ F10 ┃ F11 ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃RESET│DEBUG│     │     │     ┃     │     │  {  │  }  │     ┃     ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃     │     │     │     │     ┃     │     │     │     │     ┃     ┃
       ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛
       */
    [_BETA] = LAYOUT_kc( \
        RGB_TOG,K1, K2,   K3,   K4,   K5,   K6,   K7,   K8,   K9,   K0,   F12,  \
        RGB_MOD,F1, F2,   F3,   F4,   F5,   F6,   F7,   F8,   F9,   F10,  F11,  \
        _____,RESET,DEBUG,_____,_____,_____,_____,_____,LCBR, RCBR, _____,_____,\
        _____,_____,_____,_____,_____,_____,_____,_____,_____,_____,_____,_____ \
    )
};

static uint32_t last_key_pressed = 0;

void rxtx_init(void) {
    TX_RX_LED_INIT;
}

void rxtx_tap(keyrecord_t *record) {
    last_key_pressed = timer_read32();
    if (record->event.key.row < 4) {
        RXLED1;
    } else {
        TXLED1;
    }
}

void rxtx_process(void) {
    if (timer_elapsed(last_key_pressed) > 0) {
        RXLED0;
        TXLED0;
    }
}

/*static bool alpha_pressed = false;*/
static bool beta_pressed = false;
static uint8_t keys_pressed = 0;
static uint16_t last_keycode = 0;

static uint32_t last_scan = 0;
static uint16_t sps = 0;
static uint16_t avg_delta = 0;
static uint32_t last_sps_update = 0;

void matrix_init_user(void) {
    smoothled_set(layer_colors[_MAIN]);
    rxtx_init();
}

void matrix_scan_user(void) {
    smoothled_process();
    rxtx_process();
    uint32_t now = timer_read32();
    uint32_t delta = now - last_scan;
    if (now - last_sps_update > 250) {
        avg_delta = delta;
        sps = 1000 / delta;
        last_sps_update = now;
    }
    last_scan = timer_read32();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    /*if (keycode == KC_BETA && record->event.pressed) {*/
    /*    print("Foo\n");*/
    /*}*/
    if (record->event.pressed) {
        keys_pressed++;
        last_keycode = keycode;
    } else {
        keys_pressed--;
        last_keycode = 0;
    }

    if (keycode == RESET) {
        rgblight_setrgb(255, 255, 0);
    }
    /*if (keycode == KC_ALPHA) {*/
    /*    alpha_pressed = record->event.pressed;*/
    /*}*/
    if (keycode == KC_BETA) {
        beta_pressed = record->event.pressed;
    }
    if (record->event.pressed) {
        rxtx_tap(record);
    }
    return taphold_process(keycode, record);
}

uint32_t layer_state_set_user(uint32_t state) {
    uint8_t layer = biton32(state);
    smoothled_set(layer_colors[layer]);
    return state;
}

void encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        if (beta_pressed) {
            if (clockwise) {
                tap_code(KC_VOLD);
            } else {
                tap_code(KC_VOLU);
            }
        } else {
            if (clockwise) {
                tap_code(KC_UP);
            } else {
                tap_code(KC_DOWN);
            }
        }
    }
}

/*#ifdef OLED_DRIVER_ENABLE*/
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
  /*if (!is_keyboard_master())*/
  /*  return OLED_ROTATION_180;  // flips the display 180 degrees if offhand*/
  /*return rotation;*/
}

/*static uint32_t last_render = 0;*/

void oled_task_user(void) {
    // Host Keyboard Layer Status
    oled_write_P(PSTR("\n\n-C49-\n"), false);
    uint8_t current_layer = biton32(layer_state);
    /*oled_write_P(PSTR("     "), current_layer == _MAIN);*/
    /*oled_write_P(current_layer == _MAIN ? PSTR("< 0 >") : PSTR("  0  "), current_layer == _MAIN);*/
    /*oled_write_P(PSTR("     "), current_layer == _MAIN || current_layer == _ALPHA);*/
    /*oled_write_P(current_layer == _ALPHA ? PSTR("< A >") : PSTR("  A  "), current_layer == _ALPHA);*/
    /*oled_write_P(PSTR("     "), current_layer == _ALPHA || current_layer == _BETA);*/
    /*oled_write_P(current_layer == _BETA ? PSTR("< B >") : PSTR("  B  "), current_layer == _BETA);*/
    /*oled_write_P(PSTR("     "), current_layer == _BETA);*/

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

    uint8_t icon_index = current_layer == _MAIN ? 3 : current_layer == _ALPHA ? 1 : 2;
    oled_write_P(layers[icon_index], current_layer != _MAIN);

    oled_write_P(PSTR("\n"), false);

    char key_data[4] = {' ', ' ', '0' + keys_pressed, 0};
    oled_write(key_data, false);

    if (last_keycode && 0) {
        char data[6] = {0};
        if (last_keycode == KC_ALPHA) {
            memcpy(data, "<A>", 3);
        } else if (last_keycode == KC_BETA) {
            memcpy(data, "<B>", 3);
        } else {
            sprintf(data, "%d", last_keycode);
        }
        oled_write_ln(data, false);
    } else {
        oled_write_ln("", false);
    }

    char sps_data[6] = {0};
    sprintf(sps_data, " %d", sps);
    /*sprintf(data, "%d", avg_delta);*/
    oled_write(sps_data, false);

    // Host Keyboard LED Status
    /*uint8_t led_usb_state = host_keyboard_leds();*/
    /*oled_write_P(led_usb_state & (1<<USB_LED_NUM_LOCK) ? PSTR("NUMLCK ") : PSTR("       "), false);*/
    /*oled_write_P(led_usb_state & (1<<USB_LED_CAPS_LOCK) ? PSTR("CAPLCK ") : PSTR("       "), false);*/
    /*oled_write_P(led_usb_state & (1<<USB_LED_SCROLL_LOCK) ? PSTR("SCRLCK ") : PSTR("       "), false);*/
}
/*#endif*/

