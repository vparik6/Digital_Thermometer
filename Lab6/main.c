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
#include "launchpad.h"
#include "seg7.h"
#include "seg7digit.h"
#include "rotary.h"
#include <driverlib/adc.c>
#include <driverlib/adc.h>
#include <math.h>

int degrees;

// The running state of the stopwatch system
enum {Percentile, Raw}  sysState = Percentile;

// The initial state of the 7-segment display: "00:00" with colon on
 seg7Display_t seg7Display = {0, 0, 0, 0, 0};




void
modedisplay(uint32_t time)
{
    if (sysState == Percentile)
    {
        degrees = adcVal();
        degrees = degrees*100/4095;

        seg7Display.d4 = 10;
        seg7Display.d1 = degrees % 10;

        if(degrees < 10)  //erases the value of d3
               seg7Display.d2 =10;
        else{
            seg7Display.d2 = (degrees/10) %10;
        }
        if (degrees< 100)
            seg7Display.d3 =10;
        else {
            seg7Display.d3 = (degrees/100) %10;
        }

}
    else if (sysState == Raw){

        degrees = adcVal();


        seg7Display.d1 = degrees % 10;

        if(degrees < 10)  //erases the value of d3
               seg7Display.d2 =10;
        else{
            seg7Display.d2 = (degrees/10) %10;
        }
        if (degrees< 100)
            seg7Display.d3 =10;
        else {
            seg7Display.d3 = (degrees/100) %10;
        }
        if (degrees < 1000)
            seg7Display.d4 = 10;
        else{
            seg7Display.d4 = (degrees/1000) %10;
        }
    }

    seg7DigitUpdate(&seg7Display);
    schdCallback(modedisplay, time + 200);
}

// Callback function for checking push button. SW1 is the RESET button, and
// SW2 is the START/PAUSE/RESUME button.
void
checkPushButton(uint32_t time)
{
    int code = pbRead();
    uint32_t delay;

    switch (code) {
    case 1:                         // SW1 is the Reset button, only when the stopwatch is paused
        sysState = Percentile;
        delay = 250;                // software debouncing
        break;

    case 2:                         // SW2 is the Start/Pause/Resume button

        sysState = Raw;
        delay = 250;                // software debouncing
        break;

    default:
        delay = 10;
    }

    schdCallback(checkPushButton, time + delay);
}

int main(void)
{
    lpInit();
    seg7Init();
    adcInit();

    uprintf("%d\n\r", adcVal());
    uprintf("%s\n\r", "Lab 6: ADC");


    // Schedule the first callback events for LED flashing and push button checking.
    // Those trigger callback chains. The time unit is millisecond.
    schdCallback(checkPushButton, 1000);
    schdCallback(modedisplay, 1000);

    // Run the event scheduler to process callback events
    while (true) {
        schdExecute();
    }
}
