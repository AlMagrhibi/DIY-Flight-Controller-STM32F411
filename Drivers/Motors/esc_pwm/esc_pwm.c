#include "esc_pwm.h"

bool esc_pwm_init(void) {
    return false;
}

void esc_pwm_set(uint8_t motor, float throttle) {
    (void)motor;
    (void)throttle;
}

void esc_pwm_stop_all(void) {
}
