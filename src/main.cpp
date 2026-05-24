#include <Arduino.h>
#include "scheduler.h"
#include "tasks.h"

Task tasks[MAX_TASKS] = {
    {30, 0, task_heartbeat, 1},
    {250, 0, task_logger, 1},
    {0, 0, nullptr, 0}
};

void setup()
{
    Serial.begin(9600);

    scheduler_init();
}

void loop()
{
    scheduler_run();
}