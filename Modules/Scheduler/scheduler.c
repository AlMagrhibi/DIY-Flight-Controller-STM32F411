#include "scheduler.h"

#include <stddef.h>

#include "system_time.h"

static task_t* task_table[MAX_TASKS];
static uint8_t task_count = 0U;

void scheduler_init(void) {
    task_count = 0U;
    for (uint8_t i = 0U; i < MAX_TASKS; i++) {
        task_table[i] = NULL;
    }
}

int8_t scheduler_add_task(task_t* task) {
    if (task == NULL) {
        return -1;
    }
    if (task_count >= MAX_TASKS) {
        return -2;
    }
    if (task->function == NULL) {
        return -3;
    }
    if (task->period_us == 0U) {
        return -4;
    }

    task->last_run_us = micros();
    task->execution_count = 0U;
    task->missed_deadlines = 0U;
    task->enabled = true;

    task_table[task_count++] = task;
    return 0;
}

void scheduler_run(void) {
    const uint32_t now = micros();

    for (uint8_t i = 0U; i < task_count; i++) {
        task_t* task = task_table[i];
        if (task == NULL || !task->enabled) {
            continue;
        }

        const uint32_t elapsed = now - task->last_run_us;
        if (elapsed >= task->period_us) {
            if (elapsed >= (task->period_us * 2U)) {
                task->missed_deadlines++;
            }

            task->function();
            task->last_run_us += task->period_us;
            task->execution_count++;
        }
    }
}

void scheduler_enable_task(task_t* task) {
    if (task != NULL) {
        task->enabled = true;
        task->last_run_us = micros();
    }
}

void scheduler_disable_task(task_t* task) {
    if (task != NULL) {
        task->enabled = false;
    }
}
