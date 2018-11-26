/*
 * main.c for ECE 266 Lab 3, stopwatch
 *
 *  Created on: Aug 23, 2016
 *  Last updated on: Sep 19, 2017
 *      Author: zzhang
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "seg7digit.h"
#include "launchpad.h"
#include "seg7.h"
#include "temp.h"
#include "pwmbuzzer.h"
#include <driverlib/adc.c>
#include <driverlib/adc.h>
#include <math.h>
#include <led.c>

// Buzzer-related constants
#define BUZZER_CHECK_INTERVAL 10
#define BUZZER_ON_TIME  30
#define BUZZER_OFF_TIME (3000 - BUZZER_ON_TIME)
#define BUZZER_MAX_PERIOD ((uint32_t) (50000000 / 261.63))
#define BUZZER_MAX_PULSE_WIDTH 50000
#define BUZZER_MIN_PERIOD ((uint32_t) (50000000 / 525.25))


// The initial state of the 7-segment display: "00:00" with colon on
seg7Display_t seg7Display = {0, 0, 0, 0, 0};

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
typedef struct{
    enum
    {
        Off, On, SwitchOff, SwitchOn
    } state;            // the running state of the buzzer system
    bool buzzing;       // if the buzzer is buzzing
    int32_t timeLeft;   // the time left for the buzzer to buzz or not buzz
    int pwmPulseWidth;
    int pwmPeriod;
} buzzer_t;
static volatile buzzer_t buzzer = { On, false, 0, BUZZER_MAX_PERIOD, 0 };

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
                buzzerPwmSet(0, buzzer.pwmPeriod);
                buzzer.buzzing = false;
                buzzer.timeLeft = BUZZER_OFF_TIME;
            }
        }
        else
        {
            // If the buzzer has been silent for enough time, turn it on
            if ((buzzer.timeLeft -= BUZZER_CHECK_INTERVAL) <= 0)
            {
                buzzerPwmSet(buzzer.pwmPulseWidth, buzzer.pwmPeriod);
                buzzer.buzzing = true;
                buzzer.timeLeft = BUZZER_ON_TIME;
            }
        }
        break;

    case SwitchOff:             // De-activate the buzzer system
        if (buzzer.buzzing)
        buzzerPwmSet(0, buzzer.pwmPeriod);
        buzzer.state = Off;
        buzzer.buzzing = Off;
        break;

    case SwitchOn:              // Activate the buzzer system
        buzzerPwmSet(buzzer.pwmPulseWidth, buzzer.pwmPeriod);
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
//static bool userActivated = false;

// The callback function for checking the pushbuttons
//void checkPushButton(uint32_t time)
//{
//    uint32_t delay = 10;        // the default delay for the next checking
//
//    int code = pbRead();        // read the pushbutton
//
//    switch (code)
//    {
//    case 1:                     // SW1: Turn on the buzzer system
//        userActivated = true;
//        buzzer.state = SwitchOn;
//        delay = 250;
//        uprintf("%s\n\r", "button is on");
//        break;
//
//    case 2:                     // SW2: Turn off the buzzer system
//        userActivated = false;
//        buzzer.state = SwitchOff;
//        delay = 250;
//        uprintf("%s\n\r", "button is off");
//        break;
//    }
//
//    // schedule the next callback
//    schdCallback(checkPushButton, time + delay);
//}



void checkAdc(uint32_t time) {
    uint32_t data[2];

    adcVal(data);

    uint32_t left = 100 - data[0]*100/4096;
    uint32_t right = 100 - data[1]*100/4096;

//    led.maxPulseWidth = LED_MAX_PULSE_WIDTH * left/99;

    buzzer.pwmPulseWidth = BUZZER_MAX_PULSE_WIDTH * left/99;
    buzzer.pwmPeriod = BUZZER_MIN_PERIOD+(BUZZER_MAX_PERIOD - BUZZER_MIN_PERIOD) * (99-right)/99;

    schdCallback(checkAdc, time + 100);
}


int main(void) {
    lpInit();
    seg7Init();
    adcInit();
    buzzerInit();
    ledInit();

//    uprintf("%s\n\r", "Lab 6: ADC");
//    uprintf("%d\n\r", adcVal());

    schdCallback(checkAdc, 1000);
//    schdCallback(checkPushButton, 1005);
    schdCallback(buzzerPlay, 1010);

    // Run the event scheduler to process callback events
    while (true) {
        schdExecute();
    }
}
