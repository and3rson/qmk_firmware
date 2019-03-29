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

void matrix_init_user(void) {
    smoothled_set(layer_colors[_MAIN]);
    rxtx_init();
}

void matrix_scan_user(void) {
    smoothled_process();
    rxtx_process();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    /*if (keycode == KC_BETA && record->event.pressed) {*/
    /*    print("Foo\n");*/
    /*}*/
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

