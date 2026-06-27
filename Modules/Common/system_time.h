#ifndef SYSTEM_TIME_H
#define SYSTEM_TIME_H

#include <stdint.h>

void system_time_init(void);
uint32_t micros(void);
uint32_t millis(void);

#endif /* SYSTEM_TIME_H */
