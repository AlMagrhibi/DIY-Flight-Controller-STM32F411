#include "mixer.h"

#include <stddef.h>

#define PID_MIX_SCALE 0.5f

static bool armed = false;

static float clampf(float value, float min_value, float max_value) {
    if (value < min_value) {
        return min_value;
    }
    if (value > max_value) {
        return max_value;
    }
    return value;
}

void mixer_init(void) {
    armed = false;
}

void mixer_update(const mixer_input_t* input, mixer_output_t* output) {
    if (input == NULL || output == NULL) {
        return;
    }

    if (!armed) {
        mixer_disarm(output);
        return;
    }

    const float throttle = clampf(input->throttle, 0.0f, 1.0f);
    const float roll = clampf(input->roll, -1.0f, 1.0f) * PID_MIX_SCALE;
    const float pitch = clampf(input->pitch, -1.0f, 1.0f) * PID_MIX_SCALE;
    const float yaw = clampf(input->yaw, -1.0f, 1.0f) * PID_MIX_SCALE;

    float m0 = throttle + pitch + roll - yaw;
    float m1 = throttle + pitch - roll + yaw;
    float m2 = throttle - pitch - roll - yaw;
    float m3 = throttle - pitch + roll + yaw;

    float max_value = m0;
    if (m1 > max_value) {
        max_value = m1;
    }
    if (m2 > max_value) {
        max_value = m2;
    }
    if (m3 > max_value) {
        max_value = m3;
    }

    if (max_value > 1.0f) {
        const float scale = 1.0f / max_value;
        m0 *= scale;
        m1 *= scale;
        m2 *= scale;
        m3 *= scale;
    }

    output->motor[0] = clampf(m0, 0.0f, 1.0f);
    output->motor[1] = clampf(m1, 0.0f, 1.0f);
    output->motor[2] = clampf(m2, 0.0f, 1.0f);
    output->motor[3] = clampf(m3, 0.0f, 1.0f);
}

void mixer_disarm(mixer_output_t* output) {
    if (output == NULL) {
        return;
    }

    for (uint8_t i = 0U; i < MOTOR_COUNT; i++) {
        output->motor[i] = 0.0f;
    }
}

void mixer_set_armed(bool state) {
    armed = state;
}

bool mixer_is_armed(void) {
    return armed;
}
