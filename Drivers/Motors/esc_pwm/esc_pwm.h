#ifndef ESC_PWM_H
#define ESC_PWM_H

#include <stdbool.h>
#include <stdint.h>

bool esc_pwm_init(void);
void esc_pwm_set(uint8_t motor, float throttle);
void esc_pwm_stop_all(void);

#endif /* ESC_PWM_H */
