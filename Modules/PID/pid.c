#include "pid.h"

#include <stddef.h>

static float absf_local(float value) {
    return (value < 0.0f) ? -value : value;
}

static float clampf(float value, float min_value, float max_value) {
    if (value < min_value) {
        return min_value;
    }
    if (value > max_value) {
        return max_value;
    }
    return value;
}

void pid_init(pid_t* pid, float kp, float ki, float kd,
              float integral_limit, float output_limit,
              float derivative_alpha) {
    if (pid == NULL) {
        return;
    }

    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->integral_limit = absf_local(integral_limit);
    pid->output_limit = absf_local(output_limit);
    pid->derivative_alpha = clampf(derivative_alpha, 0.0f, 1.0f);
    pid->integral = 0.0f;
    pid->prev_error = 0.0f;
    pid->prev_derivative = 0.0f;
    pid->last_P = 0.0f;
    pid->last_I = 0.0f;
    pid->last_D = 0.0f;
    pid->output = 0.0f;
    pid->initialized = true;
    pid->first_run = true;
}

float pid_update(pid_t* pid, float setpoint, float measurement, float dt) {
    if (pid == NULL || !pid->initialized || dt <= 0.0001f) {
        return 0.0f;
    }

    const float error = setpoint - measurement;

    if (pid->first_run) {
        pid->prev_error = error;
        pid->first_run = false;
    }

    const float p_term = pid->kp * error;

    pid->integral += error * dt;
    if (pid->integral_limit > 0.0f) {
        pid->integral = clampf(pid->integral,
                               -pid->integral_limit,
                               pid->integral_limit);
    }
    const float i_term = pid->ki * pid->integral;

    const float derivative = (error - pid->prev_error) / dt;
    const float filtered_derivative =
        (pid->derivative_alpha * derivative) +
        ((1.0f - pid->derivative_alpha) * pid->prev_derivative);
    const float d_term = pid->kd * filtered_derivative;

    pid->prev_error = error;
    pid->prev_derivative = filtered_derivative;

    float output = p_term + i_term + d_term;
    if (pid->output_limit > 0.0f) {
        output = clampf(output, -pid->output_limit, pid->output_limit);
    }

    pid->last_P = p_term;
    pid->last_I = i_term;
    pid->last_D = d_term;
    pid->output = output;

    return output;
}

void pid_reset(pid_t* pid) {
    if (pid == NULL) {
        return;
    }

    pid->integral = 0.0f;
    pid->prev_error = 0.0f;
    pid->prev_derivative = 0.0f;
    pid->last_P = 0.0f;
    pid->last_I = 0.0f;
    pid->last_D = 0.0f;
    pid->output = 0.0f;
    pid->first_run = true;
}

void pid_set_gains(pid_t* pid, float kp, float ki, float kd) {
    if (pid == NULL) {
        return;
    }

    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
}

void pid_set_limits(pid_t* pid, float integral_limit, float output_limit) {
    if (pid == NULL) {
        return;
    }

    pid->integral_limit = absf_local(integral_limit);
    pid->output_limit = absf_local(output_limit);
}
