; Buzzer driver functions built on top of Tivaware
; Lab 4, ECE 266, fall 2018
; Created by Zhao Zhang

; To include names declared in C
                .cdecls "stdint.h", "stdbool.h", "stdio.h", "inc/hw_memmap.h", "driverlib/pin_map.h", "driverlib/gpio.h", "driverlib/sysctl.h", "launchpad.h", "buzzer.h"

                .text
                .global buzzerOn
                .global buzzerOff

; GPIO peripheral, port base and pin mask value for the buzzer
; The buzzer is assumed to be connected to J17, and then the buzzer's
; signal pin is connected to PC4.
BUZZER_PERIPH   .field  SYSCTL_PERIPH_GPIOC
BUZZER_PORT     .field  GPIO_PORTC_BASE
BUZZER_PIN      .equ    GPIO_PIN_4

;
; void buzzInit(): Initialze the buzz peripheral, port, and pin direction
;
buzzerInit      PUSH    {LR}

                ; Initialize the GPIO peripheral for the port that the buzzer uses:
                ;   Call SysCtlPeripheralEnable(BUZZER_PERIPH)
                LDR     r0, BUZZER_PERIPH
                BL      SysCtlPeripheralEnable

                ; Set the GPIO pin for the buzzer as output type:
                ;   Call GPIOPinTypeGPIOOutput(BUZZER_PORT, BUZZER_PIN)
                LDR     r0, BUZZER_PORT
                MOV     r1, #BUZZER_PIN
                BL      GPIOPinTypeGPIOOutput

                POP     {PC}

;
; void buzzOn(): Turn on the buzzer. It calls GPIOPinWrite() to write 1 to the signal pin.
;
buzzerOn        PUSH    {LR}

                ; Write 1 to the GPIO pin that the buzzer uses:
                ;   Call GPIOPinWrite(BUZZ_PORT, BUZZ_PIN, BUZZ_PIN)
                LDR     r0, BUZZER_PORT
                MOV     r1, #BUZZER_PIN
                MOV     r2, #BUZZER_PIN
                BL      GPIOPinWrite

                POP     {PC}

;
; void buzzOff(): Turn off the buzzer. It alls GPIOPinWrite() to write 0 to the pin
;
buzzerOff       PUSH    {LR}

                ; Write 0 to the GPIO pin that the buzzer uses
                ;   Call GPIOPinWrite(BUZZ_PORT, BUZZ_PIN, 0)
                LDR     r0, BUZZER_PORT
                MOV     r1, #BUZZER_PIN
                MOV     r2, #0
                BL      GPIOPinWrite

                POP     {PC}
