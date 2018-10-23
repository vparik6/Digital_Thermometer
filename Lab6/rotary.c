/*
 * rotary.c
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
#include "rotary.h"
#include <driverlib/adc.h>



void adcInit(){
    //Enables ADC0 module
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    //Wait for the ADC0 module to be ready
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)){

    }
    //Enables the first sample sequencer

    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH2);
    ADCSequenceEnable(ADC0_BASE, 0);


    //This part of the code is identical to the one above, except now we are using the second adc module avaible on the tiva-c board
    //Enables ADC1 module
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);

    //Wait for the ADC1 module to be ready
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC1)){

    }
    //Enables the first sample sequencer

    ADCSequenceConfigure(ADC1_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceEnable(ADC1_BASE, 0);
    ADCSequenceStepConfigure(ADC1_BASE, 0, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH4);

}

uint32_t adcVal(){

    uint32_t ui32value;

    //Trigger the sample sequence.
    ADCProcessorTrigger(ADC0_BASE, 0);

    //Wait until sample sequence is complete
    while(!ADCIntStatus(ADC0_BASE, 0, false)){
    }
    //Read value from ADC
    ADCSequenceDataGet(ADC0_BASE, 0 , &ui32value);
    ADCIntClear(ADC0_BASE,0);
    return (uint32_t)ui32value;

}

uint32_t adcVal2(){

    uint32_t ui32value;

    //Trigger the sample sequence.
    ADCProcessorTrigger(ADC1_BASE, 0);

    //Wait until sample sequence is complete
    while(!ADCIntStatus(ADC1_BASE, 0, false)){
    }
    //Read value from ADC
    ADCSequenceDataGet(ADC1_BASE, 0, &ui32value);
    ADCIntClear(ADC1_BASE,0);
    return (uint32_t)ui32value;
}


