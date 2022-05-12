#include "symbols.h"

const char PROGMEM layers[5][26] = {
    {
        ' ', ' ', ' ', ' ', ' ',
        0x20, 0x80, 0x81, 0x82, 0x20,
        0x20, 0xa0, 0xa1, 0xa2, 0x20,
        0x20, 0xc0, 0xc1, 0xc2, 0x20,
        ' ', ' ', ' ', ' ', ' ', 0
    },
    {
        ' ', ' ', ' ', ' ', ' ',
        0x20, 0x83, 0x84, 0x85, 0x20,
        0x20, 0xa3, 0xa4, 0xa5, 0x20,
        0x20, 0xc3, 0xc4, 0xc5, 0x20,
        ' ', ' ', ' ', ' ', ' ', 0
    },
    {
        ' ', ' ', ' ', ' ', ' ',
        0x20, 0x86, 0x87, 0x88, 0x20,
        0x20, 0xa6, 0xa7, 0xa8, 0x20,
        0x20, 0xc6, 0xc7, 0xc8, 0x20,
        ' ', ' ', ' ', ' ', ' ', 0
    },
    {
        ' ', ' ', ' ', ' ', ' ',
        0x20, 0x89, 0x8a, 0x8b, 0x20,
        0x20, 0xa9, 0xaa, 0xab, 0x20,
        0x20, 0xc9, 0xca, 0xcb, 0x20,
        ' ', ' ', ' ', ' ', ' ', 0
    },
    {
        ' ', ' ', ' ', ' ', ' ',
        ' ', 0x8c, 0x8d, 0x8e, ' ',
        ' ', 0xac, 0xad, 0xae, ' ',
        ' ', 0xcc, 0xcd, 0xce, ' ',
        ' ', ' ', ' ', ' ', ' ', 0
    }
};

const char PROGMEM logo[12] = {
    0x8f, 0x90, 0x91, 0x92, 0x93,
    0xaf, 0xb0, 0xb1, 0xb2, 0xb3,
    /* '4', '8', ' ', 'v', '2', */
    '\n',
    0
};

const char PROGMEM mods[4][3] = {
    {0x94, 0x95},
    {0x96, 0x97},
    {0x98, 0x99},
    {0x9a, 0x9b},
};
