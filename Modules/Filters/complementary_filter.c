#include "complementary_filter.h"

#include <math.h>
#include <stddef.h>

#define RAD_TO_DEG 57.2957795f

static float clamp_alpha(float alpha) {
    if (alpha < 0.0f) {
        return 0.0f;
    }
    if (alpha > 1.0f) {
        return 1.0f;
    }
    return alpha;
}

void complementary_filter_init(complementary_filter_t* filter, float alpha) {
    if (filter == NULL) {
        return;
    }

    filter->alpha = clamp_alpha(alpha);
    filter->roll = 0.0f;
    filter->pitch = 0.0f;
    filter->yaw = 0.0f;
    filter->initialized = false;
}

void complementary_filter_update(complementary_filter_t* filter,
                                 float gyro_x,
                                 float gyro_y,
                                 float gyro_z,
                                 float accel_x,
                                 float accel_y,
                                 float accel_z,
                                 float dt) {
    if (filter == NULL || dt <= 0.0f) {
        return;
    }

    const float accel_roll = atan2f(accel_y, accel_z) * RAD_TO_DEG;
    const float accel_pitch = atan2f(accel_x,
                                     sqrtf((accel_y * accel_y) +
                                           (accel_z * accel_z))) * RAD_TO_DEG;

    if (!filter->initialized) {
        filter->roll = accel_roll;
        filter->pitch = accel_pitch;
        filter->yaw = 0.0f;
        filter->initialized = true;
        return;
    }

    const float gyro_roll = filter->roll + (gyro_x * dt);
    const float gyro_pitch = filter->pitch + (gyro_y * dt);
    const float gyro_yaw = filter->yaw + (gyro_z * dt);

    filter->roll = (filter->alpha * gyro_roll) +
                   ((1.0f - filter->alpha) * accel_roll);
    filter->pitch = (filter->alpha * gyro_pitch) +
                    ((1.0f - filter->alpha) * accel_pitch);
    filter->yaw = gyro_yaw;

    while (filter->yaw > 180.0f) {
        filter->yaw -= 360.0f;
    }
    while (filter->yaw < -180.0f) {
        filter->yaw += 360.0f;
    }
}
