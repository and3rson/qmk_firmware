#ifndef CONFIG_H
#define CONFIG_H

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x6060
#define DEVICE_VER      0x0001
#define MANUFACTURER    Andrew Dunai
#define PRODUCT         D48
#define DESCRIPTION     A 12x4 split ortholinear keyboard with 2 encoders & 128x32 OLED display

/* key matrix size */
#define MATRIX_ROWS 4
#define MATRIX_COLS 12

/* key matrix pins */
#define MATRIX_ROW_PINS { \
    A4, B1, B2, B4 \
}

#define MATRIX_COL_PINS { \
    A2, B0, A7, A8, \
    A13, A14, B12, B11,  \
    B10, B15, B14, B13 \
}
#define UNUSED_PINS { B3 }

/* COL2ROW or ROW2COL */
//#define DIODE_DIRECTION ROW2COL
#define DIODE_DIRECTION COL2ROW

/* number of backlight levels */
//#define BACKLIGHT_PIN B5
//#define BACKLIGHT_PIN D2
//#ifdef BACKLIGHT_PIN
//#define BACKLIGHT_LEVELS 3
//#endif

/* Set 0 if debouncing isn't needed */
//#define DEBOUNCE 5

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
//#define LOCKING_SUPPORT_ENABLE

/* Locking resynchronize hack */
//#define LOCKING_RESYNC_ENABLE

/* prevent stuck modifiers */
//#define PREVENT_STUCK_MODIFIERS

#ifdef RGBLIGHT_ENABLE
#define RGB_DI_PIN D3
#ifdef RGB_DI_PIN
#define RGBLED_NUM 14
#define RGBLIGHT_HUE_STEP 8
#define RGBLIGHT_SAT_STEP 8
#define RGBLIGHT_VAL_STEP 8
#endif
#endif

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

//#define NUMBER_OF_ENCODERS 1
#define ENCODERS_PAD_A { A5, A0 }
#define ENCODERS_PAD_B { A6, A1 }
//#define ENCODER_RESOLUTION 4

#define OLED_FONT_H "glcdfont_d48.c"

// Taps (encoder tap_code)
#define TAP_CODE_DELAY 10

#endif
