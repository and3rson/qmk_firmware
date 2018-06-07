// Six-Seven - minimal layout for Let's Split.
#include "lets_split.h"
#include "action_layer.h"
#include "eeconfig.h"
#include "keys.h"
#include "pro_micro.h"
#include "pointing_device.h"
#include "smoothled.h"
#include "knob_v2.h"

extern keymap_config_t keymap_config;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _MAIN 0
#define _ALPHA 1
#define _BETA 2

enum custom_keycodes {
  KC_MAIN = SAFE_RANGE,
  KC_ALPHA,
  KC_BETA
};

// Dual keys tap/hold timeout.
// If key is tapped for less than this value, send key in addition to primary action after completing the action.
#define DUAL_HOLD_TIMEOUT 85

// TAP shortcut
#define TAP(key) register_code(key); unregister_code(key)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Main layer
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┓      ┏━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃ TAB ┃  Q  │  W  │  E  │  R  │  T  ┃      ┃  Y  │  U  │  I  │  O  │  P  ┃ BSP ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨      ┠─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃α/ESC┃  A  │  S  │  D  │  F  │  G  ┃      ┃  H  │  J  │  K  │  L  │  ;  ┃ RET ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨      ┠─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃SHIFT┃  Z  │  X  │  C  │  V  │  B  ┃      ┃  N  │  M  │  ,  │  .  │  /  ┃CTL/-┃
       ┗━━━━━┻━━━━━┷━━━━━╈━━━━━╈━━━━━╈━━━━━┫      ┣━━━━━╈━━━━━╈━━━━━╈━━━━━┷━━━━━┻━━━━━┛
                         ┃ ALT ┃ MOD ┃SPACE┃      ┃SPACE┃ β/= ┃  '  ┃
                         ┗━━━━━┻━━━━━┻━━━━━┛      ┗━━━━━┻━━━━━┻━━━━━┛
       */
    [_MAIN] = LAYOUT_kc( \
        TAB,  Q,    W,    E,    R,    T,           Y,    U,    I,    O,    P,    BSPC, \
        ALPHA,A,    S,    D,    F,    G,           H,    J,    K,    L,    SCLN, ENT,  \
        LSFT, Z,    X,    C,    V,    B,           N,    M,    COMM, DOT,  SLSH, RCTRL,\
        _____,_____,_____,LALT, LGUI, SPC,         SPC,  BETA, QUOT, _____,_____,_____ \
    ),

    /* Alpha layer (α)
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┓      ┏━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃     ┃PREV │PLAY │NEXT │     │     ┃      ┃     │ ^^^ │  ^  │ vvv │  ~  ┃ DEL ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨      ┠─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃     │VOL -│VOL +│     │     ┃      ┃HOME │ <-- │  v  │ --> │  `  ┃  \  ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨      ┠─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃     │     │     │     │     ┃      ┃ END │ INS │  [  │  ]  │  (  ┃  )  ┃
       ┗━━━━━┻━━━━━┷━━━━━╈━━━━━╈━━━━━╈━━━━━┫      ┠━━━━━╈━━━━━╈━━━━━╈━━━━━┷━━━━━┻━━━━━┛
                         ┃     ┃     ┃     ┃      ┃MOUSL┃MOUSM┃MOUSR┃
                         ┗━━━━━┻━━━━━┻━━━━━┛      ┗━━━━━┻━━━━━┻━━━━━┛
       */
    [_ALPHA] = LAYOUT_kc( \
        _____,MPRV, MPLY, MNXT, _____,_____,       _____,PGUP, UP,   PGDN, TILD, DEL,  \
        _____,_____,VOLD, VOLU, _____,_____,       HOME, LEFT, DOWN, RIGHT,GRV,  BSLS, \
        _____,_____,_____,_____,_____,_____,       END,  INS,  LBRC, RBRC, SH(9),SH(0),\
        _____,_____,_____,_____,_____,_____,       MS_BTN1,MS_BTN3,MS_BTN2,_____,_____,_____ \
    ),

    /* Beta layer (β)
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┓      ┏━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃  `  ┃  1  │  2  │  3  │  4  │  5  ┃      ┃  6  │  7  │  8  │  9  │  0  ┃ del ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨      ┠─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃ F1  │ F2  │ F3  │ F4  │ F5  ┃      ┃ F6  │ F7  │ F8  │ F9  │ F10 ┃  |  ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨      ┠─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃RESET│DEBUG│     │ F11 │ F12 ┃      ┃SND +│SND -│  {  │  }  │  -  ┃  =  ┃
       ┗━━━━━┻━━━━━┷━━━━━╈━━━━━╈━━━━━╈━━━━━┫      ┣━━━━━╈━━━━━╈━━━━━╈━━━━━┷━━━━━┻━━━━━┛
                         ┃     ┃     ┃     ┃      ┃     ┃     ┃     ┃
                         ┗━━━━━┻━━━━━┻━━━━━┛      ┗━━━━━┻━━━━━┻━━━━━┛
       */
    [_BETA] = LAYOUT_kc( \
        _____,K1,   K2,   K3,   K4,   K5,          K6,   K7,   K8,   K9,   K0,   DEL,  \
        _____,F1,   F2,   F3,   F4,   F5,          F6,   F7,   F8,   F9,   F10,  PIPE, \
        _____,RESET,DEBUG,_____,F11,  F12,         AON,  AOFF, LCBR, RCBR, MINS, RCTRL,  \
        _____,_____,_____,_____,_____,_____,       _____,_____,_____,_____,_____,_____ \
    )
};

void matrix_init_user(void) {
    smoothled_set(0, 0, 32);
    knob_init();
    /*rgblight_setrgb(255, 0, 255);*/
}

void matrix_scan_user(void) {
    smoothled_process();

    knob_report_t knob_report = knob_report_read();
    knob_report_reset();
    if (knob_report.phase) { // I check for phase to avoid handling the rotation twice (on 90 and 270 degrees).
        while (knob_report.dir < 0) {
            // CCW
            if (layer_state_is(_BETA)) {
                register_code(KC_VOLD);
                unregister_code(KC_VOLD);
            } else if (layer_state_is(_ALPHA)) {
                register_code(KC_LEFT);
                unregister_code(KC_LEFT);
            } else {
                report_mouse_t report = pointing_device_get_report();
                report.v += 1;
                pointing_device_set_report(report);
                /*register_code(KC_UP);*/
                /*unregister_code(KC_UP);*/
            }
            knob_report.dir++;
        }
        while (knob_report.dir > 0) {
            // CW
            if (layer_state_is(_BETA)) {
                register_code(KC_VOLU);
                unregister_code(KC_VOLU);
            } else if (layer_state_is(_ALPHA)) {
                register_code(KC_RIGHT);
                unregister_code(KC_RIGHT);
            } else {
                report_mouse_t report = pointing_device_get_report();
                report.v -= 1;
                pointing_device_set_report(report);
                /*register_code(KC_DOWN);*/
                /*unregister_code(KC_DOWN);*/
            }
            knob_report.dir--;
        }
    }
}

void matrix_slave_scan_user(void) {
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint16_t alpha_esc_start = 0;
    static uint16_t beta_equal_start = 0;
    static uint16_t ctrl_dash_start = 0;
    /*record->event.pressed ? smoothled_set(255, 0, 16) : smoothled_set(0, 0, 16);*/
    switch (keycode) {
        case KC_ALPHA:
            if (record->event.pressed) {
                layer_on(_ALPHA);
                alpha_esc_start = timer_read();
            } else {
                layer_off(_ALPHA);
                if (timer_elapsed(alpha_esc_start) < DUAL_HOLD_TIMEOUT) {
                    TAP(KC_ESC);
                }
            }
            return false;
        case KC_BETA:
            if (record->event.pressed) {
                layer_on(_BETA);
                beta_equal_start = timer_read();
            } else {
                layer_off(_BETA);
                if (timer_elapsed(beta_equal_start) < DUAL_HOLD_TIMEOUT) {
                    TAP(KC_EQL);
                }
            }
            return false;
        case KC_RCTRL:
            if (record->event.pressed) {
                register_code(KC_RCTL);
                ctrl_dash_start = timer_read();
            } else {
                unregister_code(KC_RCTL);
                if (timer_elapsed(ctrl_dash_start) < DUAL_HOLD_TIMEOUT) {
                    TAP(KC_MINS);
                }
            }
            return false;
        case RESET:
            /*smoothled_set(255, 255, 0);*/
            rgblight_setrgb(255, 255, 0);
            break;
    }
    return true;
}

uint32_t layer_state_set_user(uint32_t state) {
    uint8_t layer = biton32(state);
    switch (layer) {
        case _MAIN:
            smoothled_set(0, 0, 32);
            /*rgblight_setrgb(255, 0, 255);*/
            break;
        case _ALPHA:
            smoothled_set(255, 0, 16);
            /*rgblight_setrgb(255, 0, 0);*/
            break;
        case _BETA:
            smoothled_set(32, 0, 255);
            /*rgblight_setrgb(0, 0, 255);*/
            break;
    }
    return state;
}
