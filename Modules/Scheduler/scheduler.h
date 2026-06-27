#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdbool.h>
#include <stdint.h>

#include "config.h"

typedef void (*TaskFunction)(void);

typedef struct {
    TaskFunction function;
    uint32_t period_us;
    uint32_t last_run_us;
    uint32_t execution_count;
    uint32_t missed_deadlines;
    bool enabled;
    const char* name;
} task_t;

void scheduler_init(void);
void scheduler_run(void);
int8_t scheduler_add_task(task_t* task);
void scheduler_enable_task(task_t* task);
void scheduler_disable_task(task_t* task);

#endif /* SCHEDULER_H */
