#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "scheduler.h"

volatile uint32_t tick = 0;

ISR(TIMER1_COMPA_vect)
{
    tick++;
}

void scheduler_init()
{
    cli();

    TCCR1A = 0;
    TCCR1B = 0;

    OCR1A = 2500;

    TCCR1B |= (1 << WGM12);

    TIMSK1 |= (1 << OCIE1A);

    TCCR1B |= (1 << CS11) | (1 << CS10);

    sei();
}

void scheduler_run()
{
    uint32_t current_tick;

    cli();
    current_tick = tick;
    sei();

    for (int i = 0; i < MAX_TASKS; i++)
    {
        if (tasks[i].enabled && tasks[i].function != nullptr)
        {
            if ((current_tick - tasks[i].last_run) >= tasks[i].period)
            {
                tasks[i].last_run += tasks[i].period;

                tasks[i].function();
            }
        }
    }
}