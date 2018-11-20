/*
 * temp.h
 *
 *  Created on: Oct 16, 2018
 *      Author: jniero2
 */

#include <stdint.h>
#include <stdbool.h>
#include <driverlib/sysctl.h>
#include <inc/hw_memmap.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include "launchpad.h"
#include "temp.h"
#include <driverlib/adc.h>

void adcInit(){
    //Enables ADC0 module
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    //Wait for the ADC0 module to be ready
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)){}

    //Enables the first sample sequencer
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH2);
    ADCSequenceEnable(ADC0_BASE, 0);

}

void adcVal(uint32_t data[]){

    //uint32_t ui32value;

    //Trigger the sample sequence.
    ADCProcessorTrigger(ADC0_BASE, 0);

    //Wait until sample sequence is complete
    while(!ADCIntStatus(ADC0_BASE, 0, false)){
    }
    //Read value from ADC
    ADCIntClear(ADC0_BASE,0);
    ADCSequenceDataGet(ADC0_BASE, 0 , data);

}


