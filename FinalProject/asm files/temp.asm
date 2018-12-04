
; To include names declared in C
 .cdecls "stdint.h", "stdbool.h", "stdio.h", "driverlib/pin_map.h", "inc/hw_memmap.h", "driverlib/sysctl.h", "launchpad.h", "ranger.h","driverlib/gpio.h", "driverlib/timer.h"
					.text
TEMP_GPIO         .field  SYSCTL_PERIPH_GPIOB
TEMP_TIMER        .field  SYSCTL_PERIPH_TIMER3
TEMP_PORT			.field	GPIO_PORTB_BASE
TEMP_PIN			.field	GPIO_PIN_3
TIME_BASE      	 	.field	TIMER3_BASE
TIME_PAIR			.field  TIMER_CFG_SPLIT_PAIR
TIME_UP				.field  TIMER_CFG_B_CAP_TIME_UP
TIME_B				.field	TIMER_B
TIME_BOTH_EDGES		.field	TIMER_EVENT_BOTH_EDGES
BASE_TIMER			.field	GPIO_PB3_T3CCP1
TIME_EVENT			.field  TIMER_CAPB_EVENT


tempInit				 PUSH 	{LR}
						 LDR	r0,	TEMP_GPIO
						 BL		SysCtlPeripheralEnable

						 LDR    r0, TEMP_TIMER
						 BL		SysCtlPeripheralEnable

						 LDR    r0, TEMP_PORT
						 BL		SysCtlPeripheralEnable

						 LDR	r0,	TIME_BASE
						 LDR	r1, TIME_PAIR
						 LDR	r2,	TIME_UP
						 ORR	r1, r1, r2
						 BL		TimerConfigure

						 LDR	r0,	TIME_BASE
						 LDR	r1,	TIME_B
						 LDR	r2,	TIME_BOTH_EDGES
						 BL		TimerControlEvent

						 LDR	r0,	TIME_BASE
						 LDR	r1, TIME_B
						 BL		TimerEnable
                		 POP   {PC}

TempDetect       	     PUSH 	{LR}
						 LDR	r0,	TIMER_PORT
						 MOV	r1, #GPIO_PIN_3
						 BL		GPIOPinTypeGPIOOutput

						 LDR	r0, TIMER_PORT
						 MOV	r1, #GPIO_PIN_3
						 MOV	r2, #GPIO_PIN_3
						 BL		GPIOPinWrite

						 MOV	r0, #20
						 BL		waitUs

						 LDR	r0, TIMER_PORT
						 MOV	r1, #GPIO_PIN_3
						 MOV	r2, #0
						 BL		GPIOPinWrite

						 MOV	r0, #1.5
						 BL		waitMs

						 LDR	r0, TIMER_PORT
						 MOV	r1, #GPIO_PIN_3
						 MOV	r2, #GPIO_PIN_3
						 BL		GPIOPinWrite

						 MOV	r0, #30
						 BL		waitUs


						 LDR	r0, TIMER_PORT
						 MOV	r1, #GPIO_PIN_3
						 BL		GPIOPinTypeTimer

						 LDR	r0,	BASE_TIMER
						 BL		GPIOPinConfigure

						 LDR	r0, TIME_BASE
						 LDR	r1, TIME_EVENT
						 BL		TimerIntClear


while_loop				 LDR  r0, TIME_BASE
						 MOV  r1, #0
						 BL   TimerIntStatus
						 CMP  r0, #1
						 BEQ  while_loop

						 LDR  r0, TIME_BASE
						 LDR  r1, TIME_EVENT
						 BL	  TimerIntClear

while_loop1				 LDR  r0, TIME_BASE
						 MOV  r1, #0
						 BL   TimerIntStatus
						 CMP  r0, #1
						 BEQ  while_loop1

						 LDR  r0, TIME_BASE
						 LDR  r1, TIME_EVENT
						 BL	  TimerIntClear


while_loop2				 LDR  r0, TIME_BASE
						 MOV  r1, #0
						 BL   TimerIntStatus
						 CMP  r0, #1
						 BEQ  while_loop2

						 LDR 	r0, TIME_BASE
						 LDR	r1, TIME_B
						 BL		TimerValueGet
						 PUSH	{r0}

						 LDR  r0, TIME_BASE
						 LDR  r1, TIME_EVENT
						 BL	  TimerIntClear

while_loop3				 LDR  r0, TIME_BASE
						 MOV  r1, #0
						 BL   TimerIntStatus
						 CMP  r0, #1
						 BEQ  while_loop3

						 LDR 	r0, TIME_BASE
						 LDR	r1, TIME_B
						 BL		TimerValueGet
						 PUSH	{r1}

						 LDR  r0, TIME_BASE
						 LDR  r1, TIME_EVENT
						 BL	  TimerIntClear

						 POP {r1}
						 POP {r0}
						 SUB r0, r0, r1


						 MOV r2, #170
						 MUL r0, r0, r2
						 MOV r3, #50000
						 UDIV r0, r0, r3

						 POP {PC}

