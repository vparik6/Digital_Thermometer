; Assembly file for seg7DigitUpdate() function
; Lab 3, ECE 266, fall 2017
; Created by Zhao Zhang in summer 2016 for ECE 367
; Last updated on Sep 19, 2017 for ECE 266

; The 7-segment coding table
					.data
g_seg7Coding			.byte 	00111111b			; digit 0
					.byte 	00000110b 			; digit 1
					.byte 	01011011b			; digit 2
					.byte	01001111b			; digit 3
					.byte	01100110b			; digit 4
					.byte	01101101b			; digit 5
					.byte	01111101b			; digit 6
					.byte	00000111b			; digit 7
					.byte	01111111b			; digit 8
					.byte	01101111b			; digit 9

; Output code, equivalent to declare "uint8_t code[4]" as global variable
					.bss 	g_outputBuffer, 4

; Function prototype: void seg7DigitUpdate(seg7Display_t *pSeg7Display)
; The function encoded the incoming digits and colon status into 7-segment display patterns,
; and then calls seg7Update() to display the digits on the 4-digit 7-segment display.
					.text
					.global seg7DigitUpdate		; make this function global
					.global seg7Update			; to use this function

; TI assembler requires that symbols in data/bss sections be re-declared in code section before use
seg7Coding			.field	g_seg7Coding
outputBuffer			.field  g_outputBuffer

; register usage:
;	r0 - seg7Display
;	r1 - pointer to 7-segment conding table
;	r2 - pointer to output buffer
;   r3 - various
;   r9 - the colon status
seg7DigitUpdate		PUSH		{lr}					; save the return address

					; Load the address of the coding table
					LDR		r1, seg7Coding

					; Load the address of the code output buffer
					LDR		r2, outputBuffer

					; Load the colon status, convert it to either 0b00000000 (off) or 0b10000000 (on)
					; Note that bit 7 is callded the colon bit (in this program)
					LDRB		r9, [r0, #4]			; load colon status
					CMP		r9, #0				; compare r9 and 0
					ITE		NE					; If-then condition: r9 != 0
					MOVNE	r9, #10000000b		; Then part: r9 = 0b10000000
					MOVEQ	r9, #00000000b		; Else part: r9 = 0b00000000

					; Load the coding for d1, d2, d3, d4, set their colon bit, and then store to
					; the output buffer
					LDRB		r3, [r0]				; load d0
					LDRB		r3, [r1, r3]			; load the 7-segemtn pattern for d0
					ORR		r3, r9				; set the colon bit
					STRB		r3, [r2]				; store to the output buffer

					LDRB		r3, [r0, #1]			; load d1
					LDRB		r3, [r1, r3]			; load the 7-segemtn pattern for d1
					ORR		r3, r9				; set the colon bit
					STRB		r3, [r2, #1]			; store to the output buffer

					LDRB		r3, [r0, #2]			; load d2
					LDRB		r3, [r1, r3]			; load the 7-segemtn pattern for d2
					ORR		r3, r9				; set the colon bit
					STRB		r3, [r2, #2]			; store to the output buffer

					LDRB		r3, [r0, #3]			; load d3
					LDRB		r3, [r1, r3]			; load the 7-segemtn pattern for d3
					ORR		r3, r9				; set the colon bit
					STRB		r3, [r2, #3]			; store to the output buffer

					; call the seg7Update function
					MOV		r0, r2				; r0 = outputBuffer
					BL		seg7Update

					; return
					POP		{PC}
