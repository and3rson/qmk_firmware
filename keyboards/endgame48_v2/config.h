#ifndef CONFIG_H
#define CONFIG_H

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x6060
#define DEVICE_VER      0x0001
#define MANUFACTURER    Andrew Dunai
#define PRODUCT         Endgame 48 (v2)
#define DESCRIPTION     A 12x4 split ortholinear keyboard with encoder

/* key matrix size */
#define MATRIX_ROWS 8
#define MATRIX_COLS 7

/* key matrix pins */
#define MATRIX_ROW_PINS { \
    D4, B5, D5, B7, \
    F4, B6, F1, F0 \
}

#define MATRIX_COL_PINS { \
    F5, F6, F7, B1, B3, B2, \
    D7 \
}
#define UNUSED_PINS

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION ROW2COL

/* number of backlight levels */
//#define BACKLIGHT_PIN B5
//#define BACKLIGHT_PIN D2
//#ifdef BACKLIGHT_PIN
//#define BACKLIGHT_LEVELS 3
//#endif

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE 20

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE

/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

/* key combination for command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) \
)

/* prevent stuck modifiers */
#define PREVENT_STUCK_MODIFIERS

#define RGB_DI_PIN D3
#ifdef RGB_DI_PIN
#define RGBLED_NUM 12
#define RGBLIGHT_HUE_STEP 8
#define RGBLIGHT_SAT_STEP 8
#define RGBLIGHT_VAL_STEP 8
/* #define RGBLIGHT_SLEEP */

#define RGBLIGHT_DEFAULT_VAL 0
#endif

// #undef RGBLIGHT_ANIMATIONS
/*
#define B6_AUDIO

#ifdef AUDIO_ENABLE
#define STARTUP_SONG_DOOM SONG(E1M1_DOOM)
#define STARTUP_SONG SONG( \
    E__NOTE(_E6), \
    E__NOTE(_A6), \
    Q__NOTE(_E7), \
    E__NOTE(_E6), \
    E__NOTE(_E7) \
)
#endif
*/

#define ENCODERS_PAD_A { E6 }
#define ENCODERS_PAD_B { B4 }
#define ENCODER_RESOLUTION 4

#define TAPPING_TERM 90
#define PERMISSIVE_HOLD
#define TAPPING_FORCE_HOLD

// Reduce firmware size by only using uprintf

// Compat with -flto
#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION

#define NO_ACTION_ONESHOT

#define OLED_FONT_H "glcdfont_48_v2.c"

#endif
