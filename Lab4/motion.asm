;
; motion.asm
;
;  Created on: Oct 2, 2018
;      Author: jniero2
;

 .cdecls "stdint.h", "stdbool.h", "stdio.h", "inc/hw_memmap.h", "driverlib/pin_map.h", "driverlib/gpio.h", "driverlib/sysctl.h", "launchpad.h", "motion.h", "buzzer.h"

                .text
                .global pirDetect

; GPIO peripheral, port base and pin mask value for the buzzer
; The buzzer is assumed to be connected to J17, and then the buzzer's
; signal pin is connected to PC4.
PIR_PERIPH      .field  SYSCTL_PERIPH_GPIOC
PIR_PORT        .field  GPIO_PORTC_BASE
PIR_PIN         .equ    GPIO_PIN_5


pirInit      	PUSH    {LR}

                ; Initialize the GPIO peripheral for the port that the motion sensor uses:
                ;   Call SysCtlPeripheralEnable(BUZZER_PERIPH)
                LDR     r0, PIR_PERIPH
                BL      SysCtlPeripheralEnable

                ; Set the GPIO pin for the buzzer as output type:
                ;   Call GPIOPinTypeGPIOOutput(BUZZER_PORT, BUZZER_PIN)
                LDR     r0, PIR_PORT
                MOV     r1, #PIR_PIN
                BL      GPIOPinTypeGPIOInput
                POP     {PC}

pirDetect       PUSH    {LR}

                ; Write 1 to the GPIO pin that the buzzer uses:
                ;   Call GPIOPinWrite(BUZZ_PORT, BUZZ_PIN, BUZZ_PIN)
                LDR     r0, PIR_PORT
                MOV     r1, #PIR_PIN
                MOV     r2, #PIR_PIN
                BL      GPIOPinRead
                POP     {PC}



