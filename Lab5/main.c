/*
 * main.c: ECE 266 Lab 5, Fall 2018, Motion Detection Alarm (interrupt-based)
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/interrupt.h>
#include <driverlib/systick.h>
#include "launchpad.h"
#include "buzzer.h"
#include "motion.h"

// Buzzer-related constants
#define BUZZER_CHECK_INTERVAL 30
#define BUZZER_ON_TIME  30
#define BUZZER_OFF_TIME (3000 - BUZZER_ON_TIME)


/*
 * The task for playing the buzzer.
 *
 * The buzzer state and callback function. The buzzer system can be in four states:
 *   Off: The system is turned off. The buzzer is silent.
 *   On: The buzzer system is turned on. The buzzer will buzz periodically.
 *   SwitchOn: The buzzer system is to be turned on.
 *   SwitchOff: The buzzer system is to be turned off.
 */

// The buzzer object
typedef struct
{
    enum
    {
        Off, On, SwitchOff, SwitchOn
    } state;            // the running state of the buzzer system
    bool buzzing;       // if the buzzer is buzzing
    int32_t timeLeft;   // the time left for the buzzer to buzz or not buzz
} buzzer_t;
static volatile buzzer_t buzzer = { Off, false, 0 };

// The last motion detected time
static volatile uint32_t lastMotionDetectedTime = 0;

// The buzzer play callback function
void buzzerPlay(uint32_t time)
{
    uint32_t delay = BUZZER_CHECK_INTERVAL;     // the delay for next callback

    switch (buzzer.state)
    {
    case Off:           // the buzzer system is turned off
        break;

    case On:            // the buzzer system is active, turn buzzer on and off
        if (buzzer.buzzing)
        {
            // If the buzzer has been buzzing for enough time, turn it off
            if ((buzzer.timeLeft -= BUZZER_CHECK_INTERVAL) <= 0)
            {
                buzzerOff();
                buzzer.buzzing = false;
                buzzer.timeLeft = BUZZER_OFF_TIME;
            }
        }
        else
        {
            // If the buzzer has been silent for enough time, turn it on
            if ((buzzer.timeLeft -= BUZZER_CHECK_INTERVAL) <= 0)
            {
                buzzerOn();
                buzzer.buzzing = true;
                buzzer.timeLeft = BUZZER_ON_TIME;
            }
        }
        break;

    case SwitchOff:             // De-activate the buzzer system
        if (buzzer.buzzing)
            buzzerOff();
        buzzer.state = Off;
        buzzer.buzzing = Off;
        break;

    case SwitchOn:              // Activate the buzzer system
        buzzerOn();
        buzzer.state = On;
        buzzer.buzzing = true;
        buzzer.timeLeft = BUZZER_ON_TIME;
        break;
    }

    // schedule the next callback
    schdCallback(buzzerPlay, time + delay);
}

/*
 * The task for checking push button, implemented as interrupt handler
 */

// If the user has activated the buzzer system or not
static volatile bool userActivated = false;

// The interrupt handler for checking the pushbuttons
void pbIntrHandler()
{
    // Last time pushbutton was pushed
    static uint32_t lastTime = 0;

    // IMPORTANT: Clear interrupt, otherwise the interrupt handler will be executed forever
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);

    // Read the pushbutton
    int code = pbRead();

    // Get the current system time
    uint32_t time = sysTimeGet();

    // De-bouncing: If a key was pushed within 150 ms, ignore this push
    if (time < lastTime + 150)
        return;

    // Process the pushbutton input
    switch (code)
    {
    case 1:                     // SW1: Turn on the buzzer system
        userActivated = true;
        buzzer.state = SwitchOn;
        break;

    case 2:                     // SW2: Turn off the buzzer system
        userActivated = false;
        buzzer.state = SwitchOff;
        break;
    }

    // record the time to check for de-bouncing next time
    lastTime = time;
}

/*
 * Select a set of interrupts that can wake up the LaunchPad
 */
void setInterrupts()
{
    // Set interrupt on Port F, pin 0 (SW1) and pin 4 (SW2)
    GPIOIntRegister(GPIO_PORTF_BASE, pbIntrHandler); // register the interrupt handler
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, // interrupt on falling edge, note that SW1 and SW2 are active low
                   GPIO_FALLING_EDGE);
    IntPrioritySet(INT_GPIOF, 0); // set interrupt level to 0 (0 is the highest for programmable interrupts)
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4); // enable interrupts on SW1 and SW2 input
}

/*
 * The main function
 */
int main(void)
{
    lpInit();
    buzzerInit();
    pirInit();
    setInterrupts();

    // Print out a start message
    uprintf("%s\n\r", "Lab 5 starts");

    // Schedule the first callback events
    schdCallback(buzzerPlay, 200);

    // Run the callback scheduler
    while (true)
    {
        schdExecute();

        // Put Tiva C into sleep until the next interrupt happens
        __asm("    wfi");
    }
}
