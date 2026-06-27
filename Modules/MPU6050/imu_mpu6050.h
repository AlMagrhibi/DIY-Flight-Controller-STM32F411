#ifndef IMU_MPU6050_H
#define IMU_MPU6050_H

#include <stdbool.h>

#include "imu.h"

#define MPU6050_ADDR              0x68
#define MPU6050_REG_PWR_MGMT_1   0x6B
#define MPU6050_REG_SMPLRT_DIV   0x19
#define MPU6050_REG_CONFIG       0x1A
#define MPU6050_REG_GYRO_CONFIG  0x1B
#define MPU6050_REG_ACCEL_CONFIG 0x1C
#define MPU6050_REG_ACCEL_XOUT_H 0x3B
#define MPU6050_REG_WHO_AM_I     0x75

#define MPU6050_GYRO_SCALE_250   131.0f
#define MPU6050_ACCEL_SCALE_2G   16384.0f

bool mpu6050_init(void);
bool mpu6050_read(imu_data_t* data);

#endif /* IMU_MPU6050_H */
