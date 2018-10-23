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

// The initial state of the 7-segment display: "00:00" with colon on
seg7Display_t seg7Display = {0, 0, 0, 0, 0};


void displayValuesR(uint32_t time) {
int degrees = 0;
    degrees = adcVal();
    degrees = degrees*100/4096;

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

void displayValuesL(uint32_t time) {
int degrees2 = 0;
    degrees2 = adcVal2();
    degrees2 = degrees2*100/4096;

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

int main(void) {
    lpInit();
    seg7Init();
    adcInit();

    uprintf("%s\n\r", "Lab 6: ADC");
    uprintf("%d\n\r", adcVal());
    uprintf("%d\n\r", adcVal2());

    schdCallback(displayValuesR, 1000);
    schdCallback(displayValuesL, 1000);

    // Run the event scheduler to process callback events
    while (true) {
        schdExecute();
    }
}
