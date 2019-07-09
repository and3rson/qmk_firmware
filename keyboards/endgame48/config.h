#ifndef CONFIG_H
#define CONFIG_H

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x6060
#define DEVICE_VER      0x0001
#define MANUFACTURER    Andrew Dunai
#define PRODUCT         Custom 49
#define DESCRIPTION     A 12x4 split ortholinear keyboard

/* key matrix size */
#define MATRIX_ROWS 8
#define MATRIX_COLS 6

/* key matrix pins */
#define MATRIX_ROW_PINS { \
    B6, B2, B3, B1, F7, F6, F5, F4 \
}

#define MATRIX_COL_PINS { \
    B5, B4, E6, D7, C6, D4 \
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
#define DEBOUNCING_DELAY 5

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
#define RGBLED_NUM 14
#define RGBLIGHT_HUE_STEP 8
#define RGBLIGHT_SAT_STEP 8
#define RGBLIGHT_VAL_STEP 8
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

#define NUMBER_OF_ENCODERS 1
#define ENCODERS_PAD_A { D0 }
#define ENCODERS_PAD_B { D1 }
#define ENCODER_RESOLUTION 4

#define OLED_FONT_H "glcdfont_49.c"

#endif
