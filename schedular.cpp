#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define MAX_TASKS 3

volatile uint32_t tick = 0;

struct Task {
    uint32_t period;
    uint32_t last_run;
    void (*function)();
    uint8_t enabled;
};

// ================= TASKS =================

void task_heartbeat() {
    Serial.println("heartbeat");
}

void task_logger() {
    Serial.println("log");
}

// =============== TASK TABLE ===============

struct Task tasks[MAX_TASKS] = {
    {30, 0, task_heartbeat, 1},
    {250, 0, task_logger, 1},
    {0, 0, nullptr, 0}
};

// =============== SETUP ====================

void setup() {

    cli();

    Serial.begin(9600);

    // Timer1 CTC mode
    TCCR1A = 0;
    TCCR1B = 0;

    // 16 MHz / 64 prescaler = 250 kHz
    // 2500 counts = 10 ms interrupt
    OCR1A = 2500;

    TCCR1B |= (1 << WGM12);

    // Enable compare interrupt
    TIMSK1 |= (1 << OCIE1A);

    // Prescaler = 64
    TCCR1B |= (1 << CS11) | (1 << CS10);

    sei();
}

// =============== ISR ======================

ISR(TIMER1_COMPA_vect) {
    tick++;
}

// =============== SCHEDULER ================

void loop() {

    uint32_t current_tick;

    cli();
    current_tick = tick;
    sei();

    for (int i = 0; i < MAX_TASKS; i++) {

        if (tasks[i].enabled && tasks[i].function != nullptr) {

            if ((current_tick - tasks[i].last_run) >= tasks[i].period) {

                tasks[i].last_run += tasks[i].period;

                tasks[i].function();
            }
        }
    }
}