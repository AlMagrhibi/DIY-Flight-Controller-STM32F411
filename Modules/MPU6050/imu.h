#ifndef IMU_H
#define IMU_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    float gyro_x;
    float gyro_y;
    float gyro_z;
    float accel_x;
    float accel_y;
    float accel_z;
    float temperature_c;
    uint32_t timestamp_us;
} imu_data_t;

typedef struct {
    float gyro_offset_x;
    float gyro_offset_y;
    float gyro_offset_z;
    float accel_offset_x;
    float accel_offset_y;
    float accel_offset_z;
    bool is_calibrated;
} imu_calibration_t;

bool imu_init(void);
bool imu_read(imu_data_t* data);
void imu_calibrate(void);
bool imu_is_ready(void);
const imu_calibration_t* imu_get_calibration(void);

#endif /* IMU_H */
