/*
 * pwdled.h: for Lab 8, Fall 2018
 *
 *  Created by Zhao Zhang
 */

#ifndef PWMLED_H_
#define PWMLED_H_

#include <stdint.h>

// Pack PWM parameters in 32 bits, to reduce the # of parameters of ledPwmSet()
typedef struct {
	uint16_t pulseWidth;       // PWM pulse width
	uint16_t period;            // PWM period in cycles
} pwm_t;

/*
 * Initialize the timer PWM functions connected to the three sub-LEDs.
 */
void ledPwmInit();

/*
 * Set the LED parameters for the three sub-LEDs
 */
void ledPwmSet(pwm_t red, pwm_t green, pwm_t blue);

#endif /* PWDLED_H_ */
