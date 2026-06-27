#include "lowpass_filter.h"

#include <stddef.h>

static float clamp_alpha(float alpha) {
    if (alpha < 0.0f) {
        return 0.0f;
    }
    if (alpha > 1.0f) {
        return 1.0f;
    }
    return alpha;
}

void lpf_init(lpf_t* filter, float alpha) {
    if (filter == NULL) {
        return;
    }

    filter->alpha = clamp_alpha(alpha);
    filter->output = 0.0f;
}

float lpf_update(lpf_t* filter, float input) {
    if (filter == NULL) {
        return input;
    }

    filter->output = (filter->alpha * input) +
                     ((1.0f - filter->alpha) * filter->output);
    return filter->output;
}
