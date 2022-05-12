#include "endgame48_v2.h"
#include "config.h"
#include "taphold.h"
#include "rgblight.h"
#include "oled/oled_driver.h"
#include "smoothled.h"
#include "stdio.h"
#include "string.h"
#include "symbols.h"

#define ________ KC_TRNS

#define _MAIN 0
#define ALPHA 1
#define BETA 2
#define SIGMA 3

#define F_DASH "\x9c\x9d"

uint32_t layer_colors[4] = {
    0x4020FF,
    /*[ALPHA] = 0xFF0040,*/
    0xFF0010,
    0x20FF00,
    0x40FFFF
};

enum custom_keycodes {
  KC_MAIN = SAFE_RANGE,
  KC_ALPHA,
  KC_BETA,
  KC_SIGMA
};

// TapHold is my own implementation of the `LT` macro. It's processed in `process_record_user()`.
uint16_t taphold_config_size = 4;
taphold_t taphold_config[] = {
    {.key=KC_ALPHA, .mode=TAPHOLD_LAYER, .shortAction=KC_ESC, .longAction=ALPHA},
    {.key=KC_BETA, .mode=TAPHOLD_LAYER, .shortAction=KC_EQL, .longAction=BETA},
    {.key=KC_SIGMA, .mode=TAPHOLD_LAYER, .shortAction=KC_MUTE, .longAction=SIGMA},
    {.key=KC_LCTRL, .mode=TAPHOLD_MOD, .shortAction=KC_MINS, .longAction=KC_LCTRL},
    /*{.key=KC_V, .mode=TAPHOLD_MOD, .shortAction=KC_V, .longAction=KC_LALT},*/
    /*{.key=KC_B, .mode=TAPHOLD_MOD, .shortAction=KC_B, .longAction=KC_LGUI}*/
};
uint32_t taphold_timeout = 100;

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
    [_MAIN] = LAYOUT( \
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,             KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC, \
        KC_ALPHA,KC_A,    KC_S,    KC_D,    KC_F,    KC_G,             KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_ENT,  \
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,             KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_LCTRL,\
        KC_LCTRL,________,________,KC_LALT, KC_LGUI, KC_SPC,  KC_SIGMA,KC_SPC,  KC_BETA, KC_QUOT, ________,________,________\
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
    [ALPHA] = LAYOUT( \
        ________,KC_MPRV, KC_MPLY, KC_MNXT, ________,________,         KC_MINS, KC_PGUP, KC_UP,   KC_PGDN, KC_TILD, KC_DEL,  \
        ________,________,KC_VOLD, KC_VOLU, ________,________,         KC_HOME, KC_LEFT, KC_DOWN, KC_RIGHT,KC_GRV,  KC_BSLS, \
        ________,________,________,________,________,________,         KC_END,  KC_EQL,  KC_LBRC, KC_RBRC, LSFT(KC_9),LSFT(KC_0),\
        ________,________,________,________,________,________,________,________,________,________,________,________,________\
    ),

    /* 𝛽 layer
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┓┏┳━┳┓┏━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃ RGB ┃  1  │  2  │  3  │  4  │  5  ┃┃┃ ┃┃┃  6  │  7  │  8  │  9  │  0  ┃ F12 ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨┃┃ ┃┃┣─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃ F1  │ F2  │ F3  │ F4  │ F5  ┃┃┃ ┃┃┃ F6  │ F7  │ F8  │ F9  │ F10 ┃ F11 ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨┗┻━┻┛┣─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃RESET│DEBUG│SLEEP│WAKE │     ┃     ┃     │     │  {  │  }  │     ┃     ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨     ┣─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃     │     │     │     │     ┃ (o) ┃PTSCR│     │     │     │     ┃     ┃
       ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┛     ┗━━━━━┷━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛
       */
    [BETA] = LAYOUT( \
        RGB_TOG, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,             KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_F12,  \
        ________,KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,            KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  \
        ________,RESET,   DEBUG,   KC_PWR,  KC_WAKE, ________,         ________,________,KC_LCBR, KC_RCBR, ________,________,\
        ________,________,________,________,________,________,________,KC_PSCR, ________,________,________,________,________\
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
    [SIGMA] = LAYOUT( \
        ________,________,________,________,________,________,         ________,________,________,________,________,________,\
        ________,________,________,________,________,________,         ________,________,________,________,________,________,\
        ________,________,________,________,________,________,         ________,________,________,________,________,________,\
        ________,________,________,________,________,________,________,________,________,________,________,________,________\
    ),
};

static bool alpha_pressed = false;
static bool beta_pressed = false;
static bool sigma_pressed = false;

static bool ctrl_pressed = false;
static bool alt_pressed = false;
static bool shift_pressed = false;
static bool gui_pressed = false;

static volatile uint32_t last_scan;
static volatile uint32_t scan_interval;

static uint32_t reset_requested = 0;

uint32_t default_color(void) {
    return layer_colors[0];
}

void keyboard_post_init_user() {
    /* debug_enable = true; */
    /* debug_matrix = true; */
}

void matrix_init_user(void) {
    smoothled_set(layer_colors[0], 1500);
    // rxtx_init();
}

void matrix_scan_user(void) {
    uint32_t now = timer_read32();
    scan_interval = now - last_scan;
    last_scan = now;
    if (reset_requested && timer_read32() - reset_requested > 100) {
        rgblight_setrgb((DEBUG_COLOR >> 16) & 0xFF, (DEBUG_COLOR >> 8) & 0xFF, DEBUG_COLOR & 0xFF);
        reset_keyboard();
    }
}

layer_state_t layer_state_set_user(layer_state_t state) {
    state = update_tri_layer_state(state, ALPHA, BETA, SIGMA);
    uint8_t layer = biton32(state);
    smoothled_set(layer_colors[layer], 150);
    return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (reset_requested) {
        return false;
    }

    if (keycode == KC_LCTRL) ctrl_pressed = record->event.pressed;
    if (keycode == KC_LALT) alt_pressed = record->event.pressed;
    if (keycode == KC_LSFT) shift_pressed = record->event.pressed;
    if (keycode == KC_LGUI) gui_pressed = record->event.pressed;

    if (keycode == RESET) {
        reset_requested = timer_read32();
        return false;
    }

    if (keycode == KC_ALPHA) alpha_pressed = record->event.pressed;
    if (keycode == KC_BETA) beta_pressed = record->event.pressed;
    if (keycode == KC_SIGMA) sigma_pressed = record->event.pressed;

    return taphold_process(keycode, record);
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
                tap_code(KC_PGUP);
            } else {
                tap_code(KC_PGDN);
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

static bool blink = false;

void render_frame(void) {
    /* oled_write_P(PSTR("\n\nEG 48\n\n"), false); */
    oled_write_P(logo, false);
    // Host Keyboard Layer Status
    uint8_t current_layer = biton32(layer_state);

    if (reset_requested) {
        oled_invert(true);
        oled_write_P(layers[4], false);
    } else {
        oled_write_P(layers[current_layer], current_layer != _MAIN && (blink = !blink)); // && is_inverted);
    }

    oled_write_P(PSTR("\n\n"), false);

    char mod_data[12] = F_DASH " " F_DASH F_DASH " " F_DASH "\n\0";
    /* char mod_data[6] = "\x07\x07\x07\x07\x07\0"; */
    /* if (ctrl_pressed) mod_data[0] = 'C'; */
    /* if (alt_pressed) mod_data[1] = 'A'; */
    /* if (shift_pressed) mod_data[3] = 'S'; */
    /* if (gui_pressed) mod_data[4] = 'G'; */
    if (ctrl_pressed) memcpy(mod_data, mods[0], 2);
    if (alt_pressed) memcpy(mod_data +3, mods[1], 2);
    if (shift_pressed) memcpy(mod_data + 5, mods[2], 2);
    if (gui_pressed) memcpy(mod_data + 8, mods[3], 2);
    oled_write(mod_data, false);

    char buf[10] = "     ";
    buf[sprintf(buf, "%2lums", scan_interval ? scan_interval : 1L)] = ' ';
    buf[5] = 0;
    oled_write(buf, false);
}

static uint32_t last_render;

bool oled_task_user(void) {
    if (timer_elapsed32(last_render) > 50) {
        render_frame();
        last_render = timer_read32();
    }
    return false;
}
