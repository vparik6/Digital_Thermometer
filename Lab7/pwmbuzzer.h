/*
 * pwdled.h: for Lab 8, Fall 2018
 *
 *  Created by Zhao Zhang
 */

#ifndef PWMBUZZER_H_
#define PWMBUZZER_H_

#include <stdint.h>

// Pack PWM parameters in 32 bits, to reduce the # of parameters of ledPwmSet()
typedef struct {
    uint16_t pulseWidth;       // PWM pulse width
    uint16_t period;            // PWM period in cycles
} pwmbz_t;

/*
 * Initialize the timer PWM functions connected to the three sub-LEDs.
 */
void buzzerInit();
void buzzerPwmSet(pwmbz_t buzzer);

#endif /* PWDBUZZER_H_ */
