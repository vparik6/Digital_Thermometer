/*
 * ranger.c
 *
 *  Created on: Nov 6, 2018
 *      Author: jniero2
 */

#include <stdint.h>
#include <stdbool.h>
#include "launchpad.h"
#include <inc/hw_memmap.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/timer.h>
#include "ranger.h"

#define TIMER3          TIMER3_BASE
#define PORTB           GPIO_PORTB_BASE

void rangerInit(){
    //function is all good
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
    TimerConfigure(TIMER3, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_CAP_TIME_UP));
    TimerControlEvent(TIMER3_BASE, TIMER_B, TIMER_EVENT_BOTH_EDGES);
    TimerEnable(TIMER3, TIMER_B);
}

static void sendStartPulse() {
    //all good
    GPIOPinConfigure(PORTB);
    GPIOPinWrite(PORTB, GPIO_PIN_3, 0);
    waitUs(2);
    GPIOPinWrite(PORTB, GPIO_PIN_3, GPIO_PIN_3);
    waitUs(5);
    GPIOPinWrite(PORTB, GPIO_PIN_3, 0);

}

uint32_t rangerDetect() {

     uint32_t val1 = 0;
     uint32_t val2 = 0;
     uint32_t distance = 0;

     sendStartPulse();

     GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_3);
     GPIOPinConfigure(GPIO_PB3_T3CCP1);

     TimerIntClear(TIMER3_BASE, TIMER_CAPB_EVENT);
     while(!TimerIntStatus(TIMER3_BASE, false)){
     TimerIntClear(TIMER3_BASE, TIMER_CAPB_EVENT);}

     val1 = TimerValueGet(TIMER3, TIMER_B);
     uprintf("%d\n", val1);

     TimerIntClear(TIMER3_BASE, TIMER_CAPB_EVENT);
     while(!TimerIntStatus(TIMER3_BASE, false)){
     TimerIntClear(TIMER3_BASE, TIMER_CAPB_EVENT);}

     val2 = TimerValueGet(TIMER3, TIMER_B);
     uprintf("%d\n", val2);

     TimerIntClear(TIMER3_BASE, TIMER_CAPB_EVENT);

     distance =  (val2-val1)*340/2;
     return distance;
}
