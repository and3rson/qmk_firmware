#include "glider.h"
#include "taphold.h"

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

#define _MAIN 0
#define _ALPHA 1
#define _BETA 2

enum custom_keycodes {
  KC_MAIN = SAFE_RANGE,
  KC_ALPHA,
  KC_BETA
};

// TapHold is my own implementation of the `LT` macro. It's processed in `process_record_user()`.
#define TAPHOLD_CONFIG_SIZE 5
taphold_t taphold_config[TAPHOLD_CONFIG_SIZE] = {
    {.key=KC_ALPHA, .mode=TAPHOLD_LAYER, .shortAction=KC_ESC, .longAction=_ALPHA},
    {.key=KC_BETA, .mode=TAPHOLD_LAYER, .shortAction=KC_EQL, .longAction=_BETA},
    {.key=KC_LCTRL, .mode=TAPHOLD_MOD, .shortAction=KC_MINS, .longAction=KC_LCTRL},
    {.key=KC_V, .mode=TAPHOLD_MOD, .shortAction=KC_V, .longAction=KC_LALT},
    {.key=KC_B, .mode=TAPHOLD_MOD, .shortAction=KC_B, .longAction=KC_LGUI}
};
uint16_t taphold_config_size = TAPHOLD_CONFIG_SIZE;
uint32_t taphold_timeout = 90;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Main layer
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃ TAB ┃  Q  │  W  │  E  │  R  │  T  ┃  '  ┃  Y  │  U  │  I  │  O  │  P  ┃ BSP ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────╊━━━━━╉─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃α/ESC┃  A  │  S  │  D  │  F  │  G  ┃ β/= ┃  H  │  J  │  K  │  L  │  ;  ┃ RET ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────╊━━━━━╉─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃SHIFT┃  Z  │  X  │  C  │ALT/V│MOD/B┃SPACE┃  N  │  M  │  ,  │  .  │  /  ┃CTL/-┃
       ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛
       */
    LAYOUT_kc(
        TAB,  Q,    W,    E,    R,    T,    QUOT, Y,    U,    I,    O,    P,    BSPC,
        ALPHA,A,    S,    D,    F,    G,    BETA, H,    J,    K,    L,    SCLN, ENT,
        LSFT, Z,    X,    C,    V,    B,    SPC,  N,    M,    COMM, DOT,  SLSH, LCTRL
    ),
    /* Alpha layer
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃     ┃PREV │PLAY │NEXT │     │     ┃  |  ┃     │ ^^^ │  ^  │ vvv │  ~  ┃ DEL ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────╊━━━━━╉─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃     │VOL -│VOL +│     │     ┃     ┃HOME │ <-- │  v  │ --> │  `  ┃  \  ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────╊━━━━━╉─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃     │     │     │     │     ┃     ┃ END │ INS │  [  │  ]  │  (  ┃  )  ┃
       ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛
       */
    LAYOUT_kc(
        _____,MPRV, MPLY, MNXT, _____,_____,SCLN, _____,PGUP, UP,   PGDN, TILD, DEL,
        _____,_____,_____,_____,_____,_____,BETA, HOME, LEFT, DOWN, RIGHT,SCLN, BSLS,
        _____,_____,_____,_____,_____,_____,SPC,  END,  INS,  LBRC, RBRC, SH(9),SH(0)
    ),
    /* Beta layer
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃     ┃  1  │  2  │  3  │  4  │  5  ┃     ┃  6  │  7  │  8  │  9  │  0  ┃ DEL ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────╊━━━━━╉─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃ F1  │ F2  │ F3  │ F4  │ F5  ┃     ┃ F6  │ F7  │ F8  │ F9  │ F10 ┃  |  ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────╊━━━━━╉─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃RESET│DEBUG│     │ F11 │ F12 ┃     ┃     │     │  {  │  }  │  -  ┃  =  ┃
       ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛
       */
    LAYOUT_kc(
        _____,K1,   K2,   K3,   K4,   K5,   _____,K6,   K7,   K8,   K9,   K0,   _____,
        _____,F1,   F2,   F3,   F4,   F5,   _____,F6,   F7,   F8,   F9,   F10,  _____,
        _____,RESET,DEBUG,_____,F11,  F12,  _____,_____,_____,LCBR, RCBR, _____,_____
    )
};

void matrix_init_user(void) {
}

void matrix_scan_user(void) {
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    return taphold_process(keycode, record);
}
