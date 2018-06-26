#include "glider.h"
#include "taphold.h"

#define KC______ KC_TRNS

#define _MAIN 0
#define _ALPHA 1
#define _BETA 2

enum custom_keycodes {
  KC_MAIN = SAFE_RANGE,
  KC_ALPHA,
  KC_BETA
};

#define TAPHOLD_CONFIG_SIZE 3
taphold_t taphold_config[TAPHOLD_CONFIG_SIZE] = {
    {.key=KC_ALPHA, .mode=TAPHOLD_LAYER, .shortAction=KC_ESC, .longAction=_ALPHA},
    {.key=KC_BETA, .mode=TAPHOLD_LAYER, .shortAction=KC_EQL, .longAction=_BETA},
    {.key=KC_LCTRL, .mode=TAPHOLD_MOD, .shortAction=KC_MINS, .longAction=KC_LCTRL}
};
uint16_t taphold_config_size = TAPHOLD_CONFIG_SIZE;
uint32_t taphold_timeout = 90;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Main layer
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃ TAB ┃  Q  │  W  │  E  │  R  │  T  │  '  │  Y  │  U  │  I  │  O  │  P  ┃ BSP ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃α/ESC┃  A  │  S  │  D  │  F  │  G  │ β/= │  H  │  J  │  K  │  L  │  ;  ┃ RET ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃SHIFT┃  Z  │  X  │  C  │  V  │  B  │SPACE│  N  │  M  │  ,  │  .  │  /  ┃CTL/-┃
       ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛
       */
    LAYOUT_kc(
        TAB,  Q,    W,    E,    R,    T,    SCLN, Y,    U,    I,    O,    P,    BSPC,
        ALPHA,A,    S,    D,    F,    G,    BETA, H,    J,    K,    L,    SCLN, ENT,
        LSFT, Z,    X,    C,    V,    B,    SPC,  N,    M,    COMM, DOT,  SLSH, LCTRL
    )
};

void matrix_init_user(void) {
}

void matrix_scan_user(void) {
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    return taphold_process(keycode, record);
}
