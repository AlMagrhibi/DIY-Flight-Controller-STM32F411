#ifndef PID_H
#define PID_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    float kp;
    float ki;
    float kd;
    float integral;
    float prev_error;
    float prev_derivative;
    float integral_limit;
    float output_limit;
    float derivative_alpha;
    float last_P;
    float last_I;
    float last_D;
    float output;
    bool initialized;
    bool first_run;
} pid_t;

void pid_init(pid_t* pid,
              float kp,
              float ki,
              float kd,
              float integral_limit,
              float output_limit,
              float derivative_alpha);

float pid_update(pid_t* pid, float setpoint, float measurement, float dt);
void pid_reset(pid_t* pid);
void pid_set_gains(pid_t* pid, float kp, float ki, float kd);
void pid_set_limits(pid_t* pid, float integral_limit, float output_limit);

#endif /* PID_H */
