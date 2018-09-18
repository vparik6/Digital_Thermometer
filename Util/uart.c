/*
 * uart.c
 *
 * UART (serial) functions for Tiva C LaunchPad
 *
 *  Created on: Jul 21, 2016
 *      Author: zzhang
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/uart.h>

#define LINE_SIZE 	80		// Line size limit for uprintf

// Declare GPIO_PA0_U0RX and GPIO_PA1_U0TX if they are not declared yet
// For some reason, those are not declared in TivaWare for device LM4F120H5QR
#ifndef GPIO_PA0_U0RX
#define GPIO_PA0_U0RX           0x00000001
#endif
#ifndef GPIO_PA1_U0TX
#define GPIO_PA1_U0TX           0x00000401
#endif

// Enable UART0 with baud rate of 115,200 bps, and 8-N-1 frame
void uartInit()
{
	// Enable UART0 and GPIO Port A as peripherals
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	// Connect UART0 RX and TX pins
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	// Configure baud rate and frame format
	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
			            UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
}

// Send a character throught UART0
void uartPutChar(char ch)
{
  UARTCharPut(UART0_BASE, ch);
}

// Receive a character through UART0
char uartGetChar()
{
	char ch = UARTCharGet(UART0_BASE);
	return ch;
}

int uartPutString(char *buffer)
{
	char ch;
	int n = 0;

	// Repeat sending the char at the head until the NULL is seen
	while ((ch = *buffer++) != '\0') {
		uartPutChar(ch);
		n++;
	}

	return n;
}

// Print a string through UART0 using an internal buffer of 80 characters.
// Return the number of characters printed.
int uprintf(char* fmt, ...)
{
	char buffer[LINE_SIZE+1];	// an internal buffer of 81 characters
	va_list argptr;		// variable arguments list

	// Create a va_list, call vsnprintf function to format the output
	va_start(argptr, fmt);
	vsnprintf(buffer, LINE_SIZE+1, fmt, argptr);
	va_end(argptr);

	// Send the formatted string to UART
	return uartPutString(buffer);
}

