#ifndef GLIDER_H
#define GLIDER_H

#include "quantum.h"

/*
#define LAYOUT( \
    K00, K01, K02, K03, K04, K05, K06, K30, K31, K32, K33, K34, K35, \
    K10, K11, K12, K13, K14, K15, K16, K40, K41, K42, K43, K44, K45, \
    K20, K21, K22, K23, K24, K25, K26, K50, K51, K52, K53, K54, K55 \
) { \
    { K00, K01, K02, K03, K04, K05, K06 }, \
    { K10, K11, K12, K13, K14, K15, K16 }, \
    { K20, K21, K22, K23, K24, K25, K26 }, \
    { K30, K31, K32, K33, K34, K35, K36 }, \
    { K40, K41, K42, K43, K44, K45, K46 }, \
    { K50, K51, K52, K53, K54, K55, K56 }  \
}
*/
#define LAYOUT( \
    K00, K01, K02, K03, K04, K05, K06, K07, K08, K09, K0A, K0B, K0C, \
    K10, K11, K12, K13, K14, K15, K16, K17, K18, K19, K1A, K1B, K1C, \
    K20, K21, K22, K23, K24, K25, K26, K27, K28, K29, K2A, K2B, K2C \
) { \
    { K00, K01, K02, K03, K04, K05, K06, K07, K08, K09, K0A, K0B, K0C }, \
    { K10, K11, K12, K13, K14, K15, K16, K17, K18, K19, K1A, K1B, K1C }, \
    { K20, K21, K22, K23, K24, K25, K26, K27, K28, K29, K2A, K2B, K2C } \
}

// Used to create a keymap using only KC_ prefixed keys
#define LAYOUT_kc( \
    L00, L01, L02, L03, L04, L05, L06, L07, L08, L09, L0A, L0B, L0C, \
    L10, L11, L12, L13, L14, L15, L16, L17, L18, L19, L1A, L1B, L1C, \
    L20, L21, L22, L23, L24, L25, L26, L27, L28, L29, L2A, L2B, L2C \
    ) \
    LAYOUT( \
        KC_##L00, KC_##L01, KC_##L02, KC_##L03, KC_##L04, KC_##L05, KC_##L06, KC_##L07, KC_##L08, KC_##L09, KC_##L0A, KC_##L0B, KC_##L0C, \
        KC_##L10, KC_##L11, KC_##L12, KC_##L13, KC_##L14, KC_##L15, KC_##L16, KC_##L17, KC_##L18, KC_##L19, KC_##L1A, KC_##L1B, KC_##L1C, \
        KC_##L20, KC_##L21, KC_##L22, KC_##L23, KC_##L24, KC_##L25, KC_##L26, KC_##L27, KC_##L28, KC_##L29, KC_##L2A, KC_##L2B, KC_##L2C \
    )

#endif
