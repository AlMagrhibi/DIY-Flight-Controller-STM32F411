#ifndef MOTORS_H
#define MOTORS_H

#include <stdbool.h>
#include <stdint.h>

#include "config.h"

typedef enum {
    MOTOR_DRIVER_NONE = 0,
    MOTOR_DRIVER_DRV8833_T,
    MOTOR_DRIVER_ESC_PWM_T,
    MOTOR_DRIVER_DSHOT_T
} motor_driver_type_t;

typedef enum {
    MOTOR_FRONT_LEFT = 0,
    MOTOR_FRONT_RIGHT = 1,
    MOTOR_REAR_RIGHT = 2,
    MOTOR_REAR_LEFT = 3,
    MOTOR_COUNT_ENUM = 4
} motor_id_t;

void motors_init(void);
void motors_arm(void);
void motors_disarm(void);
bool motors_is_armed(void);
void motors_set(motor_id_t motor, float throttle);
void motors_set_all(const float throttle[4]);
void motors_stop(void);
bool motors_is_initialized(void);
motor_driver_type_t motors_get_driver_type(void);

#endif /* MOTORS_H */
