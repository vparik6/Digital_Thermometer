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


uint32_t ui32Value;
void adcRead(uint32_t data[]){ // data[] has space for two
    ADCProcessorTrigger(ADC0_BASE, 0); // Trigger ADC sample sequence
    while(!ADCIntStatus(ADC0_BASE, 0, false)){ // Wait until the sample sequence has completed

}

ADCSequenceDataGet(ADC0_BASE, 0, &ui32Value); // Read ADC Sequence Data

ADCIntClear( … ); // Clear interrupt status
}
