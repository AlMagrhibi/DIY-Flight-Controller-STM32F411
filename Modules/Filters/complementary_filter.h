#ifndef COMPLEMENTARY_FILTER_H
#define COMPLEMENTARY_FILTER_H

#include <stdbool.h>

typedef struct {
    float roll;
    float pitch;
    float yaw;
    float alpha;
    bool initialized;
} complementary_filter_t;

void complementary_filter_init(complementary_filter_t* filter, float alpha);
void complementary_filter_update(complementary_filter_t* filter,
                                 float gyro_x,
                                 float gyro_y,
                                 float gyro_z,
                                 float accel_x,
                                 float accel_y,
                                 float accel_z,
                                 float dt);

#endif /* COMPLEMENTARY_FILTER_H */
