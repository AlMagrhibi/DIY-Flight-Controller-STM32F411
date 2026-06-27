#ifndef CONFIG_H
#define CONFIG_H

/* Timing Configuration */
#define IMU_UPDATE_RATE_HZ        1000
#define PID_UPDATE_RATE_HZ        500
#define RECEIVER_UPDATE_RATE_HZ   50
#define TELEMETRY_UPDATE_RATE_HZ  20
#define BATTERY_UPDATE_RATE_HZ    10
#define LED_UPDATE_RATE_HZ        10

/* Scheduler */
#define MAX_TASKS                 16

/* Motor Driver Selection */
#define MOTOR_DRIVER_DRV8833      1
#define MOTOR_DRIVER_ESC_PWM      2
#define MOTOR_DRIVER_TYPE         MOTOR_DRIVER_DRV8833

/* Motor Configuration */
#define MOTOR_MIN_PWM             1000
#define MOTOR_MAX_PWM             2000
#define MOTOR_IDLE_PWM            1100

/* Battery */
#define LOW_BATTERY_VOLTAGE       3.5f
#define CRITICAL_BATTERY_VOLTAGE  3.3f
#define BATTERY_CELL_COUNT        2

/* Safety */
#define FAILSAFE_TIMEOUT_MS       300
#define ARM_MAX_THROTTLE          0.05f
#define ARM_MAX_ANGLE_DEG         15.0f

/* Radio */
#define RADIO_TIMEOUT_MS          500

#endif /* CONFIG_H */
