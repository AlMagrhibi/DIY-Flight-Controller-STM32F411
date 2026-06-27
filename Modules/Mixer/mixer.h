#ifndef MIXER_H
#define MIXER_H

#include <stdbool.h>
#include <stdint.h>

#define MOTOR_COUNT 4U

typedef struct {
    float throttle;
    float roll;
    float pitch;
    float yaw;
} mixer_input_t;

typedef struct {
    float motor[MOTOR_COUNT];
} mixer_output_t;

void mixer_init(void);
void mixer_update(const mixer_input_t* input, mixer_output_t* output);
void mixer_disarm(mixer_output_t* output);
void mixer_set_armed(bool state);
bool mixer_is_armed(void);

#endif /* MIXER_H */
