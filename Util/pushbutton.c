/*
 * \file pushbutton.c
 *
 * \brief Push button functions for the LaunchPad
 *
 * \author Zhao Zhang
 * \date Summer 2013
 *
 * TODO Improve debouncing (wait for the key to be up for a while)
 */

#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <inc/hw_gpio.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/rom_map.h>
#include <driverlib/rom.h>
#include "launchpad.h"


/*
 * PIN usage:
 *     SW1 => PF4
 *     SW2 => PF0
 */

// Push button enum type, SW means soft switch
typedef enum {
    SW1 = GPIO_PIN_4, SW2 = GPIO_PIN_0
} PushButton_t;


// debouncing delay in milliseconds
#define INERTIA		50		

// this module initialized?
static bool initialized = false;

/*
 * Initialize the push button in forced mode
 */
void pbInitForced()
{
    /// Enable PF and configure PF0 and PF4 to output
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);

    /// The following comment and code are copied from buttons.c
    /// from TI code
    //
    // Unlock PF0 so we can change it to a GPIO input
    // Once we have enabled (unlocked) the commit register then re-lock it
    // to prevent further changes.  PF0 is muxed with NMI thus a special case.
    //
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;

    //
    // Set the pad(s) for open-drain operation with a weak pull-up.
    //
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4,
            GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}

/*
 * Initialize the push button, non-forced mode
 */
void pbInit()
{
    if (initialized)
        return;
    pbInitForced();
    initialized = true;
}

/*
 * Read push buttons input, return 1 if SW1 is pushed or both SW1 and SW2 are pushed, 2 if SW
 * is pushed, and 0 if none is pushed.
 */
int
pbRead()
{
    uint8_t pinValue;

    // SW1 and SW2 are active low, so invert the reading
    pinValue = ~GPIOPinRead(GPIO_PORTF_BASE, SW1 | SW2);

    if (pinValue & SW1)
    	    return 1;
    else if (pinValue & SW2)
    	    return 2;
    else
    	    return 0;
}

