#ifndef DRV8833_H
#define DRV8833_H

#include <stdbool.h>
#include <stdint.h>

bool drv8833_init(void);
void drv8833_set_pwm(uint8_t motor, float throttle);
void drv8833_stop_all(void);

#endif /* DRV8833_H */
