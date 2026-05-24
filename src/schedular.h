#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

#define MAX_TASKS 3

typedef struct {
    uint32_t period;
    uint32_t last_run;
    void (*function)();
    uint8_t enabled;
} Task;

extern volatile uint32_t tick;
extern Task tasks[MAX_TASKS];

void scheduler_init();
void scheduler_run();

#endif