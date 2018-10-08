/*
 * ECE 266, Fall 2018, Lab 4 Buzzer
 * main.c
 */

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "launchpad.h"
#include "buzzer.h"
#include "motion.h"

// Buzzer-related constants
#define BUZZER_CHECK_INTERVAL 10
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
static buzzer_t buzzer = { Off, false, 0 };

// The buzzer play callback function
void buzzerPlay(uint32_t time)
{
    uint32_t delay = BUZZER_CHECK_INTERVAL;     // the delay for next callback

    switch (buzzer.state)
    {
    case Off:           // the buzzer system is turned off, do nothing
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
 * The task for checking push button
 */

// If the user has activated the buzzer system or not
bool userActivated = false;

// The callback function for checking the pushbuttons
void checkPushButton(uint32_t time)
{
    uint32_t delay = 10;        // the default delay for the next checking

    int code = pbRead();        // read the pushbutton

    switch (code)
    {
    case 1:                     // SW1: Turn on the buzzer system
        userActivated = true;
        buzzer.state = SwitchOn;
        delay = 250;
        uprintf("%s\n\r", "button is on");
        break;

    case 2:                     // SW2: Turn off the buzzer system
        userActivated = false;
        buzzer.state = SwitchOff;
        delay = 250;
        uprintf("%s\n\r", "button is off");
        break;
    }

    // schedule the next callback
    schdCallback(checkPushButton, time + delay);
}


bool motionDetected = false;
///This is the code added by self for debugging purposes
void motionSensor(uint32_t time) {

    uint32_t delay = 10;
        if (userActivated = true) {
            if (pirDetect()) {
                uprintf("%s\n\r", "motion detected");
                motionDetected = true;
                
            }
            else {
                uprintf("%s\n\r", "No motion ");
                
            }
        }
    schdCallback(motionSensor, time + delay);
}

int main(void)
{
    lpInit();
    buzzerInit();
    pirInit();

    uprintf("%s\n\r", "Lab 4 starts");


    // Schedule the first callback events
    schdCallback(buzzerPlay, 1000);
    schdCallback(checkPushButton, 1005);
    schdCallback(motionSensor, 1010);

    // Run the callback scheduler
    while (true)
    {
        schdExecute();
    }
}
