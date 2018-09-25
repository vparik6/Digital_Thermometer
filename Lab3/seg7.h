/*
 * seg7.h
 *
 *  Created on: Sep 4, 2016
 *      Author: zzhang
 */

#ifndef SEG7_H_
#define SEG7_H_

// Initialize the port connection to TiM1637 and the 7-segment display
void seg7Init();

// Update all 4 digits of the 7-segment displays
void seg7Update(uint8_t code[]);

#endif /* SEG7_H_ */
