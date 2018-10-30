/*
 * pwdled.h: for Lab 8, Fall 2018
 *
 *  Created by Zhao Zhang
 */

#ifndef PWMBUZZER_H_
#define PWMBUZZER_H_

#include <stdint.h>

// Pack PWM parameters in 32 bits, to reduce the # of parameters of ledPwmSet(
/*
 * Initialize the timer PWM functions connected to the three sub-LEDs.
 */
void buzzerInit();
void buzzerPwmSet(uint32_t pulse, uint32_t period);

#endif /* PWDBUZZER_H_ */
