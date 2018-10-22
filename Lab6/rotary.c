/*
 * rotary.c
 *
 *  Created on: Oct 16, 2018
 *      Author: jniero2
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
#include "rotary.h"
#include <driverlib/adc.h>

void adcInit() {
// this code is pulled from page 46 of peripheral driver library under the BB resource tab
// Enable the ADC0 module.
//
SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
//
// Wait for the ADC0 module to be ready.
//
while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)) {
}
//
// Enable the first sample sequencer to capture the value of channel 0 when the processor trigger occurs.
//
ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH0);
//ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH4);
ADCSequenceEnable(ADC0_BASE, 0);
}

uint16_t adcVal() {
    
    
uint32_t ui32Value;
//
// Trigger the sample sequence.
//
ADCProcessorTrigger(ADC0_BASE, 0);
//
// Wait until the sample sequence has completed.
//
while(!ADCIntStatus(ADC0_BASE, 0, false))
{
}
//
// Read the value from the ADC.
//
ADCSequenceDataGet(ADC0_BASE, 0, &ui32Value);
//ADCIntClear(ACD0_BASE, 0); // This line might need to go somewhere else
return ui32Value;

}
