; pwmled.asm: Assembly PWM-LED functions
; Lab 7, ECE 266, Fall 2018
; Created by Zhao Zhang

; Include C header files
                .cdecls "stdint.h","stdbool.h","inc/hw_memmap.h","driverlib/sysctl.h","driverlib/gpio.h","driverlib/pin_map.h","driverlib/timer.h","pwmbuzzer.h"

                .text

; Field and any other declarations
TIMER0_PERIPH   .field  SYSCTL_PERIPH_TIMER0

TIMER0          .field  TIMER0_BASE

PORTC           .field  GPIO_PORTC_BASE

PIN_ROUTE_4     .field  PIO_PC4_WT0CCP0

                .asmfunc
buzzerInit      PUSH    {lr}

                LDR     r0, TIMER0_PERIPH
                BL      SysCtlPeripheralEnable

                LDR     r0, PORTC
                MOV     r1, #(GPIO_PIN_4)
                BL      GPIOPinTypeTimer

                LDR     r0, PIN_ROUTE_4
                BL      GPIOPinConfigure

                LDR     r0, TIMER0
                MOV     r1, #TIMER_CFG_SPLIT_PAIR
                ORR     r1, #TIMER_CFG_B_PWM
                BL      TimerConfigure

                LDR     r0, TIMER0
                MOV     r1, #TIMER_A
                MOV     r2, #true
                BL      TimerControlLevel

                LDR     r0, TIMER0
                MOV     r1, #TIMER_A
                MOV     r2, #200
                BL      TimerLoadSet

                LDR     r0, TIMER0
                MOV     r1, #TIMER_A
                MOV     r2, #0
                BL      TimerMatchSet

                LDR     r0, TIMER0,
                MOV     r1, #TIMER_A
                BL      TimerEnable


                POP     {pc}
                .endasmfunc


                .asmfunc
buzzerPwmSet       PUSH    {r0}            ; Push r0, r1 and r2 for later function calls

                ; Set the period and duty cycle length for the read sub-LED
                ;   call TimerLoadSet(TIMER0, TIMER_A, red.period)
                ;   call TimerMatchSet(TIMER0, TIMER_A, red.pulse_width)
                ;   Note that r0, r1, r2 each holds a pwm_t type, with
                MOV     r2, r0, LSR #16             ; r2 = red.period
                LDR     r0, TIMER0
                MOV     r1, #TIMER_A
                BL      TimerLoadSet

                LDR     r0, TIMER0
                MOV     r1, #TIMER_A
                LDR     r2, [sp, #0]                ; r2 = pushed r0 (red)
                BFC     r2, #16, #16                ; clear bits 31-16, i.e. r2 = red.pulse_width
                BL      TimerMatchSet

                ; Set the period and duty cycle length for the read sub-LED
                ;   call TimerLoadSet(TIMER1, TIMER_A, blue.period)
                ;   call TimerMatchSet(TIMER1, TIMER_A, blue.pulse_width)

                ADD     sp, #12                     ; release the stack space for r2, r1, r0
                POP     {pc}
                .endasmfunc
