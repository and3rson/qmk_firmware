#include "gh60.h"
#include "action_layer.h"
#include "rgblight.h"

enum keys {
    TEST = SAFE_RANGE,
    BACKLIT
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* 0: qwerty */
    KEYMAP(
	    KC_ESC,    KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC, \
        KC_TAB,    KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  _______, \
        F(0),      KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,  KC_BSLS,  KC_ENT,  \
        KC_LSFT,   KC_GRV,   KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  _______,  KC_RSFT, \
        KC_LCTL,   KC_LGUI,  KC_LALT,  KC_SPC,                                                     KC_BSLS,  KC_RALT,  KC_DEL,   KC_APP,   KC_RCTL),
    /* 1: fn */
	KEYMAP(
        KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   BL_ON,    \
        _______,  KC_PGUP,  KC_UP,    KC_PGDN,  KC_HOME,  _______,  _______,  KC_PGUP,  KC_UP,    KC_PGDN,  _______,  _______,  _______,  _______,  \
        _______,  KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_END,   _______,  _______,  KC_LEFT,  KC_DOWN,  KC_RIGHT, _______,  _______,  _______,  BACKLIT,  \
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  TEST,     \
        _______,  _______,  _______,                      _______,                                _______,  _______,  _______,  _______,  _______),
    /* 2: arrows */
	KEYMAP(
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  \
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  \
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  \
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_UP,    \
        _______,  _______,  _______,                      _______,                                _______,  _______,  KC_LEFT,  KC_DOWN,  KC_RGHT),
};

};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  // MACRODOWN only works in this function
      switch(id) {
        case 0:
          if (record->event.pressed) {
            register_code(KC_RSFT);
          } else {
            unregister_code(KC_RSFT);
          }
        break;
      }
    return MACRO_NONE;
};

void matrix_scan_user(void) {

//Layer LED indicators
    uint32_t layer = layer_state;

    if (layer & (1<<1)) {
        gh60_wasd_leds_on();
        gh60_fn_led_on();
    } else {
        gh60_wasd_leds_off();
        gh60_fn_led_off();
    }

    if (layer & (1<<2)) {
        gh60_poker_leds_on();
        gh60_esc_led_on();
    } else {
        gh60_poker_leds_off();
        gh60_esc_led_off();
    }

};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TEST:
            if (record->event.pressed) {
              register_code(KC_T);
              unregister_code(KC_T);
              register_code(KC_E);
              unregister_code(KC_E);
              register_code(KC_S);
              unregister_code(KC_S);
              register_code(KC_T);
              unregister_code(KC_T);
            }
            return false;
        case BACKLIT:
            if (record->event.pressed) {
              backlight_step();
            }
            return false;
    }
    return true;
};

