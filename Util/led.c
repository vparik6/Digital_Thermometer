/**
 * \file led.c
 *
 * \brief LED driver functions.
 *
 * \author Zhao Zhang
 *
 * \date Summer 2013
 */

#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>

/**
 * LaunchPad LED Pins. Note the color sequence is RBG, not the common RGB
 *
 * 	    RED	    => Port F, pin 1
 *      BLUE	=> Port F, pin 2
 *      GREEN	=> Port F, pin 3
 */

// this module initialized?
static bool initialized = false;

/**
 * Initialize LED in forced mode
 */
void ledInitForced()
{
    /// Enable the GPIO used by LED (Port F)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    /// Enable the three pins used by LED
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1); // RED
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2); // GREEN
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3); // BLUE
}

/**
 * Initialize LED in non-forced mode
 */
void ledInit()
{
    /// If the LED module has been initialized, stop
    if (initialized)
        return;

    /// Initialize the LED
    ledInitForced();
    initialized = true;
}

/**
 * Turn on or off of each LED color
 * @param red   Red on/off
 * @param green Green on/off
 * @param blue  Blue on/off
 */
void ledTurnOnOff(bool red, bool blue, bool green)
{
    uint8_t pinValue = 0;
    if (red)
        pinValue |= (1 << 1);
    if (blue)
        pinValue |= (1 << 2);
    if (green)
        pinValue |= (1 << 3);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,
            pinValue);
}

