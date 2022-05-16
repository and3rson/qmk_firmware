#include "smoothled.h"
#include "math.h"
#include "string.h"

static color_t  source[RGBLED_NUM]  = {};
static color_t  current[RGBLED_NUM] = {};
static color_t  target[RGBLED_NUM]  = {};
static uint32_t duration            = 1500;
static int32_t  smoothledTimer      = -1;

const float RGBLED_STEP = 1.0f / ((float)RGBLED_NUM);

void smoothled_set(uint32_t color, uint32_t newDuration) {
    smoothledTimer = timer_read32();
    for (uint8_t p = 0; p < RGBLED_NUM; p++) {
        source[p] = current[p];
        memcpy(&target[p], &color, sizeof(color));
    }
    duration = newDuration;
}

color_t interp(color_t a, color_t b, float k, uint8_t x) {
    k = pow(k, 2);
    // Center
    x = fabs(5.5 - ((float)x));
    // Time range for current LED
    float start = RGBLED_STEP * x;
    float end = RGBLED_STEP * (x + 1);
    if (k <= start) {
        k = 0;
    } else if (k >= end) {
        k = 1;
    } else {
        k = (k - start) / RGBLED_STEP;
    }
    return (color_t) {
        .r = (1 - k) * a.r + k * b.r,
        .g = (1 - k) * a.g + k * b.g,
        .b = (1 - k) * a.b + k * b.b
    };
}

void smoothled_process(void) {
    if (smoothledTimer == -1) {
        return;
    }
    smoothled_render();
}

void smoothled_render(void) {
    uint32_t passed = timer_elapsed32(smoothledTimer);
    /* uint32_t ta = duration - tb; */
    if (passed > duration) {
        passed = duration;
        smoothledTimer = -1;
    }
    float k = ((float)passed) / duration;
    for (uint8_t p = 0; p < RGBLED_NUM; p++) {
        current[p] = interp(source[p], target[p], k, p);
        /* current[p] = color; */
        rgblight_setrgb_at(current[p].r, current[p].g, current[p].b, p);
    }
}
