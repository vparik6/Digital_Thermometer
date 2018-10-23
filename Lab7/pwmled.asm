; pwmled.asm: Assembly PWM-LED functions
; Lab 7, ECE 266, Fall 2018
; Created by Zhao Zhang

; Include C header files
                .cdecls "stdint.h","stdbool.h","inc/hw_memmap.h","driverlib/sysctl.h","driverlib/gpio.h","driverlib/pin_map.h","driverlib/timer.h","pwmled.h"

                .text

; Field and any other declarations
TIMER0_PERIPH   .field  SYSCTL_PERIPH_TIMER0
TIMER1_PERIPH   .field  SYSCTL_PERIPH_TIMER1
TIMER0          .field  TIMER0_BASE
TIMER1          .field  TIMER1_BASE
PORTF           .field  GPIO_PORTF_BASE
PIN_ROUTE_1     .field  GPIO_PF1_T0CCP1
PIN_ROUTE_2     .field  GPIO_PF2_T1CCP0
PIN_ROUTE_3     .field  GPIO_PF3_T1CCP1

;
; void pwmledInit()
; Initialize the timer PWM functions connected to the three sub-LEDs.
;    Red:      PF1, T0CCP1, Timer 0-B
;    Blue:     PF2, T1CCP0, Timer 1-A
;    Green:    PF3, T1CCP1, Timer 1-B
;
                .asmfunc
ledPwmInit      PUSH    {lr}

                ; Enable Timer 0 and Timer 1, note that PORT F has already be enabled
                ;   call SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0)
                ;   call SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1)
                LDR     r0, TIMER0_PERIPH
                BL      SysCtlPeripheralEnable

                LDR     r0, TIMER1_PERIPH
                BL      SysCtlPeripheralEnable

                ; Connect pins to those timers
                ;   call GPIOPinTypeTimer(PORTF, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
                ;   call GPIOPinConfigure(GPIO_PF1_T0CCP1)
                ;   call GPIOPinConfigure(GPIO_PF2_T1CCP0)
                ;   call GPIOPinConfigure(GPIO_PF3_T1CCP1)
                LDR     r0, PORTF
                MOV     r1, #(GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)
                BL      GPIOPinTypeTimer

                LDR     r0, PIN_ROUTE_1
                BL      GPIOPinConfigure

                LDR     r0, PIN_ROUTE_2
                BL      GPIOPinConfigure

                LDR     r0, PIN_ROUTE_3
                BL      GPIOPinConfigure
                
                ; Select PWM for Timer 0 sub-Timer B, and Timer 1 sub-Timers A & B
                ;   call TimerConfigure(TIMER0, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM))
                ;   call TimerConfigure(TIMER1, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM | TIMER_CFG_B_PWM))
                LDR     r0, TIMER0
                MOV     r1, #TIMER_CFG_SPLIT_PAIR
                ORR     r1, #TIMER_CFG_B_PWM
                BL      TimerConfigure

                LDR     r0, TIMER1
                MOV     r1, #TIMER_CFG_SPLIT_PAIR
                ORR     r1, #TIMER_CFG_A_PWM
                ORR     r1, #TIMER_CFG_B_PWM
                BL      TimerConfigure

                ; Invert the PWM waveform, so that the Match register value is the pulse width.
                ; Otherwise, the pulse width will be (Load value) - (Match value).
                ; The inversion is done by enabling output inversion on the PWM pins.
                ;   call TimerControlLevel(TIMER0, TIMER_B, true /* output inversion */)
                ;   call TimerControlLevel(TIMER1, TIMER_BOTH, true /* output inversion */)
                LDR     r0, TIMER0
                MOV     r1, #TIMER_B
                MOV     r2, #true
                BL      TimerControlLevel

                LDR     r0, TIMER1
                MOV     r1, #TIMER_BOTH
                MOV     r2, #true
                BL      TimerControlLevel

                ; Turn off the LEDs for now
                ;   call TimerLoadSet(TIMER0, TIMER_B, 200)
                ;   call TimerMatchSet(TIMER0, TIMER_B, 0)
                ;   call TimerLoadSet(TIMER1, TIMER_A, 200)
                ;   call TimerMatchSet(TIMER1, TIMER_A, 0)
                ;   call TimerLoadSet(TIMER1, TIMER_B, 200)
                ;   call TimerMatchSet(TIMER1, TIMER_B, 0)
                LDR     r0, TIMER0
                MOV     r1, #TIMER_B
                MOV     r2, #200
                BL      TimerLoadSet

                LDR     r0, TIMER0
                MOV     r1, #TIMER_B
                MOV     r2, #0
                BL      TimerMatchSet

                LDR     r0, TIMER1
                MOV     r1, #TIMER_A
                MOV     r2, #200
                BL      TimerLoadSet

                LDR     r0, TIMER1
                MOV     r1, #TIMER_A
                MOV     r2, #0
                BL      TimerMatchSet

                LDR     r0, TIMER1
                MOV     r1, #TIMER_B
                MOV     r2, #200
                BL      TimerLoadSet

                LDR     r0, TIMER1
                MOV     r1, #TIMER_B
                MOV     r2, #0
                BL      TimerMatchSet

                ; Enable the Timer 0's TimerB and Timer 1's TimerA and TimerB
                ;   call TimerEnable(TIMER0, TIMER_B)
                ;   call TimerEnable(TIMER1, TIMER_A | TIMER_B)
                LDR     r0, TIMER0,
                MOV     r1, #TIMER_B
                BL      TimerEnable

                LDR     r0, TIMER1,
                MOV     r1, #(TIMER_A | TIMER_B)
                BL      TimerEnable

                POP     {pc}
                .endasmfunc

;
; void pwmledSetColor(pwm_t red, pwm_t blue, pwm_t green)
;
; Set color of the on-board LED by setting the intensity of the primary colors
;
                .asmfunc
ledPwmSet       PUSH    {lr, r2, r1, r0}            ; Push r0, r1 and r2 for later function calls

                ; Set the period and duty cycle length for the read sub-LED
                ;   call TimerLoadSet(TIMER0, TIMER_B, red.period)
                ;   call TimerMatchSet(TIMER0, TIMER_B, red.pulse_width)
                ;   Note that r0, r1, r2 each holds a pwm_t type, with
                MOV     r2, r0, LSR #16             ; r2 = red.period
                LDR     r0, TIMER0
                MOV     r1, #TIMER_B
                BL      TimerLoadSet

                LDR     r0, TIMER0
                MOV     r1, #TIMER_B
                LDR     r2, [sp, #0]                ; r2 = pushed r0 (red)
                BFC     r2, #16, #16                ; clear bits 31-16, i.e. r2 = red.pulse_width
                BL      TimerMatchSet

                ; Set the period and duty cycle length for the read sub-LED
                ;   call TimerLoadSet(TIMER1, TIMER_A, blue.period)
                ;   call TimerMatchSet(TIMER1, TIMER_A, blue.pulse_width)
                LDR     r0, TIMER1
                MOV     r1, #TIMER_A
                LDR     r2, [sp, #4]                ; r2 = pushed r1 (blue)
                MOV     r2, r2, LSR #16             ; r2 = blue.period
                BL      TimerLoadSet

                LDR     r0, TIMER1
                MOV     r1, #TIMER_A
                LDR     r2, [sp, #4]                ; r2 = pushed r2 (green)
                BFC     r2, #16, #16                ; clear bits 31-16, i.e. r2 = blue.pulse_width
                BL      TimerMatchSet

                ; Set the period and duty cycle length for the read sub-LED
                ;   call TimerLoadSet(TIMER1, TIMER_B, green.period)
                ;   call TimerMatchSet(TIMER1, TIMER_B, green.pulse_width)
                LDR     r0, TIMER1
                MOV     r1, #TIMER_B
                LDR     r2, [sp, #8]                ; r2 = pushed r1
                MOV     r2, r2, LSR #16             ; r2 = blue.period
                BL      TimerLoadSet

                LDR     r0, TIMER1
                MOV     r1, #TIMER_B
                LDR     r2, [sp, #8]                ; r2 = pushed r1
                BFC     r2, #16, #16                ; clear bits 31-16, i.e. r2 = blue.pulse_width
                BL      TimerMatchSet

                ADD     sp, #12                     ; release the stack space for r2, r1, r0
                POP     {pc}
                .endasmfunc

