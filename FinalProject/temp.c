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

#define TIMER3          TIMER3_BASE
#define PORTB           GPIO_PORTB_BASE

void tempInit(){
    //function is all good
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
    TimerConfigure(TIMER3_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_CAP_TIME_UP));
    TimerControlEvent(TIMER3_BASE, TIMER_B, TIMER_EVENT_BOTH_EDGES);
    TimerEnable(TIMER3_BASE, TIMER_B);
}

void sendStartPulse() {
    //all good
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_3);
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0);
    waitUs(20);
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3);
    waitUs(30);
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0);

}

//unsigned short ReadByte(){
//  unsigned short num = 0, t;
//  DataDir = 1;
//  for (i=0; i&lt;8; i++){
//   while(!Data);
//   TMR2 = 0;
//   T2CON.TMR2ON = 1;  // Start TMR2 from 0 when a low to high data pulse
//   while(Data);       // is detected, and wait until it falls low again.
//   T2CON.TMR2ON = 0;  // Stop the TMR2 when the data pulse falls low.
//   if(TMR2 &gt; 40) num |= 1&lt;&lt;(7-i);  // If time &gt; 40us, Data is 1
//  }
//  return num;
//}


uint32_t tempDetect() {

     uint32_t val1;
     uint32_t val2;
     uint32_t distance;


     sendStartPulse();
     GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_3);
     GPIOPinConfigure(GPIO_PB3_T3CCP1);


     TimerIntClear(TIMER3_BASE, TIMER_CAPB_EVENT);


     while(!TimerIntStatus(TIMER3_BASE, false)){

     }

     val1 = TimerValueGet(TIMER3_BASE, TIMER_B);

     TimerIntClear(TIMER3_BASE, TIMER_CAPB_EVENT);
     while(!TimerIntStatus(TIMER3_BASE, false)){
     }
     val2 = TimerValueGet(TIMER3_BASE, TIMER_B);

     TimerIntClear(TIMER3_BASE, TIMER_CAPB_EVENT);

//     uprintf("%u value 1 is\n\r", val1);
//     uprintf("%u value 2 is\n\r", val2);

    // TimerIntClear(TIMER3_BASE, TIMER_CAPB_EVENT);

     distance =  (val2-val1)*170/50000; // this result gives us distance in millimeters. 100mm = 10 cm
     uprintf("distance is %u\n\r", distance);
     return distance;
}




//void adcInit(){
//    //Enables ADC0 module
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
//
//    //Wait for the ADC0 module to be ready
//    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)){}
//
//    //Enables the first sample sequencer
//    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
//    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH2);
//    ADCSequenceEnable(ADC0_BASE, 0);
//
//}
//
//void adcVal(uint32_t data[]){
//
//    //uint32_t ui32value;
//
//    //Trigger the sample sequence.
//    ADCProcessorTrigger(ADC0_BASE, 0);
//
//    //Wait until sample sequence is complete
//    while(!ADCIntStatus(ADC0_BASE, 0, false)){
//    }
//    //Read value from ADC
//    ADCIntClear(ADC0_BASE,0);
//    ADCSequenceDataGet(ADC0_BASE, 0 , data);
//
//}


