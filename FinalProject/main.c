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
#include "buzzer.h"
#include <driverlib/adc.c>
#include <driverlib/adc.h>
#include <math.h>
#include  <led.c>

// Buzzer-related constants
#define BUZZER_CHECK_INTERVAL 10
#define BUZZER_ON_TIME  30
#define BUZZER_OFF_TIME (3000 - BUZZER_ON_TIME)

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
static bool userActivated = false;

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



void displayValuesR(uint32_t time) {
int degrees = 0;
    degrees = adcVal();
    degrees = 99-degrees*100/4096;

        seg7Display.d1 = degrees % 10;

        if(degrees < 10){
            seg7Display.d2 = 0; // digits 0-9
        }
        else{
            seg7Display.d2 = (degrees/10) % 10; // take the mod because we want the whole number to appear
        }

        seg7DigitUpdate(&seg7Display);
        schdCallback(displayValuesR, time + 200);
}
/*
void displayValuesL(uint32_t time) {
int degrees2 = 0;
    degrees2 = adcVal2();
    degrees2 = 99-degrees2*100/4096;

         seg7Display.d3 = degrees2 % 10;

         if(degrees2 < 10){
             seg7Display.d4 = 0;
         }
         else{
             seg7Display.d4 = (degrees2/10) % 10;
         }

         seg7DigitUpdate(&seg7Display);
         schdCallback(displayValuesL, time + 200);
}
*/
int main(void) {
    lpInit();
    seg7Init();
    adcInit();
    buzzerInit();
    ledInit();

    uprintf("%s\n\r", "Lab 6: ADC");
    uprintf("%d\n\r", adcVal());

    schdCallback(displayValuesR, 1000);
    //schdCallback(displayValuesL, 1000);

    // Run the event scheduler to process callback events
    while (true) {
        schdExecute();
    }
}
