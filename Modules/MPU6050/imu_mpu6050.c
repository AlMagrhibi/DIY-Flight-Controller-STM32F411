#include "imu_mpu6050.h"

#include <stddef.h>

#include "stm32f4xx_hal.h"
#include "system_time.h"

extern I2C_HandleTypeDef hi2c1;

#define I2C_TIMEOUT_MS 10U

static bool mpu_write(uint8_t reg, uint8_t value) {
    uint8_t buffer[2] = {reg, value};
    return HAL_I2C_Master_Transmit(&hi2c1,
                                   (uint16_t)(MPU6050_ADDR << 1),
                                   buffer,
                                   sizeof(buffer),
                                   I2C_TIMEOUT_MS) == HAL_OK;
}

static bool mpu_read(uint8_t reg, uint8_t* buffer, uint8_t length) {
    if (buffer == NULL || length == 0U) {
        return false;
    }

    if (HAL_I2C_Master_Transmit(&hi2c1,
                                (uint16_t)(MPU6050_ADDR << 1),
                                &reg,
                                1U,
                                I2C_TIMEOUT_MS) != HAL_OK) {
        return false;
    }

    return HAL_I2C_Master_Receive(&hi2c1,
                                  (uint16_t)(MPU6050_ADDR << 1),
                                  buffer,
                                  length,
                                  I2C_TIMEOUT_MS) == HAL_OK;
}

bool mpu6050_init(void) {
    uint8_t who_am_i = 0U;
    if (!mpu_read(MPU6050_REG_WHO_AM_I, &who_am_i, 1U)) {
        return false;
    }
    if (who_am_i != MPU6050_ADDR) {
        return false;
    }

    if (!mpu_write(MPU6050_REG_PWR_MGMT_1, 0x01U)) {
        return false;
    }
    HAL_Delay(10U);

    if (!mpu_write(MPU6050_REG_SMPLRT_DIV, 0x00U)) {
        return false;
    }
    if (!mpu_write(MPU6050_REG_CONFIG, 0x03U)) {
        return false;
    }
    if (!mpu_write(MPU6050_REG_GYRO_CONFIG, 0x00U)) {
        return false;
    }
    if (!mpu_write(MPU6050_REG_ACCEL_CONFIG, 0x00U)) {
        return false;
    }

    return true;
}

bool mpu6050_read(imu_data_t* data) {
    if (data == NULL) {
        return false;
    }

    uint8_t buffer[14];
    if (!mpu_read(MPU6050_REG_ACCEL_XOUT_H, buffer, sizeof(buffer))) {
        return false;
    }

    const int16_t ax = (int16_t)((buffer[0] << 8) | buffer[1]);
    const int16_t ay = (int16_t)((buffer[2] << 8) | buffer[3]);
    const int16_t az = (int16_t)((buffer[4] << 8) | buffer[5]);
    const int16_t temp = (int16_t)((buffer[6] << 8) | buffer[7]);
    const int16_t gx = (int16_t)((buffer[8] << 8) | buffer[9]);
    const int16_t gy = (int16_t)((buffer[10] << 8) | buffer[11]);
    const int16_t gz = (int16_t)((buffer[12] << 8) | buffer[13]);

    data->accel_x = (float)ax / MPU6050_ACCEL_SCALE_2G;
    data->accel_y = (float)ay / MPU6050_ACCEL_SCALE_2G;
    data->accel_z = (float)az / MPU6050_ACCEL_SCALE_2G;
    data->gyro_x = (float)gx / MPU6050_GYRO_SCALE_250;
    data->gyro_y = (float)gy / MPU6050_GYRO_SCALE_250;
    data->gyro_z = (float)gz / MPU6050_GYRO_SCALE_250;
    data->temperature_c = ((float)temp / 340.0f) + 36.53f;
    data->timestamp_us = micros();

    return true;
}
