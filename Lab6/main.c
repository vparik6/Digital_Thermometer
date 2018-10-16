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


//
// Enable the first sample sequencer to capture the value of channel 0 when
// the processor trigger occurs.
//
ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
ADCSequenceStepConfigure(ADC0_BASE, 0, 0,
ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH0);
ADCSequenceEnable(ADC0_BASE, 0);



SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0); // Enable the ADC0 module.

while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)){ // Wait for the ADC0 module to be ready.

}



int main(void)
{
    lpInit();
    seg7Init();

    uprintf("%s\n\r", "Lab 3: Stopwatch");

    // Update the clock display
    //seg7DigitUpdate(&seg7Display);

    // Schedule the first callback events for LED flashing and push button checking.
    // Those trigger callback chains. The time unit is millisecond.
    //schdCallback(stopWatchUpdate, 1000);
    //schdCallback(checkPushButton, 1005);

    // Run the event scheduler to process callback events
    while (true) {
        schdExecute();
    }
}
