#include "quantum.h"
#include "config.h"
#include "rgblight.h"

typedef struct {
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t padding;
} color_t;

void smoothled_set(uint32_t color, uint32_t newDuration);
void smoothled_process(void);
void smoothled_render(void);
