#include <Arduino.h>
#include "tasks.h"

void task_heartbeat()
{
    Serial.println("heartbeat");
}

void task_logger()
{
    Serial.println("log");
}