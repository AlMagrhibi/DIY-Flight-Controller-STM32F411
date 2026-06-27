#include "imu.h"

#include "imu_mpu6050.h"
#include "stm32f4xx_hal.h"

#define CALIBRATION_SAMPLES 1000

static imu_calibration_t calibration = {0};
static bool initialized = false;

bool imu_init(void) {
    initialized = mpu6050_init();
    return initialized;
}

bool imu_read(imu_data_t* data) {
    if (!initialized || data == NULL) {
        return false;
    }
    if (!mpu6050_read(data)) {
        return false;
    }

    if (calibration.is_calibrated) {
        data->gyro_x -= calibration.gyro_offset_x;
        data->gyro_y -= calibration.gyro_offset_y;
        data->gyro_z -= calibration.gyro_offset_z;
        data->accel_x -= calibration.accel_offset_x;
        data->accel_y -= calibration.accel_offset_y;
        data->accel_z -= calibration.accel_offset_z;
    }

    return true;
}

void imu_calibrate(void) {
    float gyro_x = 0.0f;
    float gyro_y = 0.0f;
    float gyro_z = 0.0f;
    float accel_x = 0.0f;
    float accel_y = 0.0f;
    float accel_z = 0.0f;
    uint16_t valid_samples = 0U;
    imu_data_t sample;

    for (uint16_t i = 0U; i < CALIBRATION_SAMPLES; i++) {
        if (mpu6050_read(&sample)) {
            gyro_x += sample.gyro_x;
            gyro_y += sample.gyro_y;
            gyro_z += sample.gyro_z;
            accel_x += sample.accel_x;
            accel_y += sample.accel_y;
            accel_z += sample.accel_z;
            valid_samples++;
        }
        HAL_Delay(1U);
    }

    if (valid_samples == 0U) {
        calibration.is_calibrated = false;
        return;
    }

    calibration.gyro_offset_x = gyro_x / (float)valid_samples;
    calibration.gyro_offset_y = gyro_y / (float)valid_samples;
    calibration.gyro_offset_z = gyro_z / (float)valid_samples;
    calibration.accel_offset_x = accel_x / (float)valid_samples;
    calibration.accel_offset_y = accel_y / (float)valid_samples;
    calibration.accel_offset_z = (accel_z / (float)valid_samples) - 1.0f;
    calibration.is_calibrated = true;
}

bool imu_is_ready(void) {
    return initialized;
}

const imu_calibration_t* imu_get_calibration(void) {
    return &calibration;
}
