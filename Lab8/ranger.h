/*
 * ranger.h
 *
 *  Created on: Nov 6, 2018
 *      Author: jniero2
 */

#ifndef RANGER_H_
#define RANGER_H_

#include <stdint.h>

void rangerInit();
static void sendStartPulse();
uint32_t rangerDetect();

#endif /* RANGER_H_ */
