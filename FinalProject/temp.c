/*
 * temp.h
 *
 *  Created on: Oct 16, 2018
 *      Author: jniero2, vparik6
 */

#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include "launchpad.h"
#include "temp.h"
#include <driverlib/adc.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/timer.h>


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
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3);
    waitUs(20);
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0);
    waitMs(1.5);                                 //milliseconds as according to the pdf
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3);
    waitUs(30);
}


uint32_t tempDetect() {

     uint32_t val1 = 0;
     uint32_t val2 = 0;
     uint32_t difference = 0;
     int i = 0;
     int value = 0;


     sendStartPulse();
     GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_3);
     GPIOPinConfigure(GPIO_PB3_T3CCP1);

     TimerIntClear(TIMER3_BASE, TIMER_CAPB_EVENT); // these two clear the two events that are not data bits
     while(!TimerIntStatus(TIMER3_BASE, false)){

     }
     TimerIntClear(TIMER3_BASE, TIMER_CAPB_EVENT);

     while(!TimerIntStatus(TIMER3_BASE, false)){

     }
     TimerIntClear(TIMER3_BASE, TIMER_CAPB_EVENT);


     for (i = 0; i < 32; i++) {

         while(!TimerIntStatus(TIMER3_BASE, false)){

         }
         val1 = TimerValueGet(TIMER3_BASE, TIMER_B);

         TimerIntClear(TIMER3_BASE, TIMER_CAPB_EVENT);

         while(!TimerIntStatus(TIMER3_BASE, false)){

         }
         val2 = TimerValueGet(TIMER3_BASE, TIMER_B);

         TimerIntClear(TIMER3_BASE, TIMER_CAPB_EVENT);
         difference = val2 - val1;
         // this part of the code will determine if the bit is either 1 or 0
         //1 will be represented by the time difference being greater than 120 uS
         //0 will be represented by the time difference being less than 80 uS

         if (difference <= 30*50) {
             value = value << 1 | 0;
         }
         else if (difference >= 68*50) {
             value = value << 1 | 1;
         }
     }

//     uprintf("value is %u\n\r", value);
     return value;
}
