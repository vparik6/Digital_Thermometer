
 .cdecls "stdint.h", "stdbool.h", "stdio.h", "inc/hw_memmap.h", "driverlib/pin_map.h", "driverlib/gpio.h", "driverlib/sysctl.h", "driverlib/adc.h", "launchpad.h", "rotary.h"
					.text
ADC_PERIPH			.field	SYSCTL_PERIPH_ADC0
ADC_PORT			.field 	ADC0_BASE
ADC_TRIGGER			.field	ADC_TRIGGER_PROCESSOR ;

;to initialize the adc sensor
adcInit			PUSH 	{LR}
				;   Call SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0)
                LDR   r0, ADC_PERIPH
                BL    SysCtlPeripheralEnable


                ;   Call ADCSequenceConfigure(ADC0_BASE, 0 /* sequencer #0 */, ADC_TRIGGER_PROCESSOR, 0 /* priority */);
                LDR   r0, ADC_PORT
                MOV   r1, #0
                LDR   r2, ADC_TRIGGER
                MOV   r3, #0
                BL    ADCSequenceConfigure

				;ADCSequenceStepConfigure(ADC0_BASE, 0 ,0 , ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH2)
				LDR		r0, ADC_PORT
				MOV		r1, #0
				MOV 	r2, #0
				MOV 	r3, #(ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH2)
				BL    	ADCSequenceStepConfigure

				;ADCSequenceEnable(ADC0_BASE, 0)
				LDR		r0, ADC_PORT
				MOV		r1, #0
				BL		ADCSequenceEnable

                POP   {PC}

;to detect adc sensor
adcVal			PUSH 	{LR}

				;ADCProcessorTrigger(ADC0_BASE, 0);
				LDR		r0, ADC_PORT
				MOV		r1, #0
				BL		ADCProcessorTrigger

				;while (!ADCIntStatus(ADC0_BASE, 0, false))
loop			LDR		r0, ADC_PORT
				MOV		r1, #0
				MOV		r2, #0
				BL		ADCIntStatus
				CMP 	r0, #0
				BEQ		loop

				;ADCSequenceDataGet(ADC0_BASE, 0, &adcReading);
				SUB 	sp, #4
				LDR		r0, ADC_PORT
				MOV		r1, #0
				MOV 	r2, sp
				BL		ADCSequenceDataGet

				POP		{r0, PC}


