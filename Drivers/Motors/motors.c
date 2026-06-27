#include "motors.h"

#include <stddef.h>

#include "mixer.h"

#if MOTOR_DRIVER_TYPE == MOTOR_DRIVER_DRV8833
#include "brushed/drv8833.h"
#define DRIVER_INIT()   drv8833_init()
#define DRIVER_SET(m,t) drv8833_set_pwm((m), (t))
#define DRIVER_STOP()   drv8833_stop_all()
#define DRIVER_TYPE     MOTOR_DRIVER_DRV8833_T
#elif MOTOR_DRIVER_TYPE == MOTOR_DRIVER_ESC_PWM
#include "esc_pwm/esc_pwm.h"
#define DRIVER_INIT()   esc_pwm_init()
#define DRIVER_SET(m,t) esc_pwm_set((m), (t))
#define DRIVER_STOP()   esc_pwm_stop_all()
#define DRIVER_TYPE     MOTOR_DRIVER_ESC_PWM_T
#else
#error "MOTOR_DRIVER_TYPE not defined in config.h"
#endif

static bool driver_ready = false;
static bool armed = false;
static motor_driver_type_t active_driver = MOTOR_DRIVER_NONE;

static float clamp_throttle(float throttle) {
    if (throttle < 0.0f) {
        return 0.0f;
    }
    if (throttle > 1.0f) {
        return 1.0f;
    }
    return throttle;
}

void motors_init(void) {
    driver_ready = DRIVER_INIT();
    active_driver = driver_ready ? DRIVER_TYPE : MOTOR_DRIVER_NONE;
    armed = false;
    mixer_set_armed(false);
    DRIVER_STOP();
}

void motors_arm(void) {
    if (driver_ready) {
        armed = true;
        mixer_set_armed(true);
    }
}

void motors_disarm(void) {
    armed = false;
    mixer_set_armed(false);
    DRIVER_STOP();
}

bool motors_is_armed(void) {
    return armed;
}

void motors_set(motor_id_t motor, float throttle) {
    if (!driver_ready || !armed) {
        return;
    }
    if (motor >= MOTOR_COUNT_ENUM) {
        return;
    }

    DRIVER_SET((uint8_t)motor, clamp_throttle(throttle));
}

void motors_set_all(const float throttle[4]) {
    if (throttle == NULL) {
        return;
    }

    for (uint8_t i = 0U; i < MOTOR_COUNT_ENUM; i++) {
        motors_set((motor_id_t)i, throttle[i]);
    }
}

void motors_stop(void) {
    if (driver_ready) {
        DRIVER_STOP();
    }
}

bool motors_is_initialized(void) {
    return driver_ready;
}

motor_driver_type_t motors_get_driver_type(void) {
    return active_driver;
}
