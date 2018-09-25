/*
 * seg7digit.h
 *
 *  Created on: Sep 13, 2016
 *  Last updated on: Sep 18, 2017
 *      Author: zzhang
 */

#include <stdint.h>
#include <stdbool.h>

#ifndef SEG7DIGIT_H_
#define SEG7DIGIT_H_

// The structure of the state of the 4-digit 7-segment display
typedef struct {
    uint8_t d1;                 // digit 1, rightmost
    uint8_t d2;                 // digit 2
    uint8_t d3;                 // digit 3
    uint8_t d4;                 // digit 4, leftmost
    uint8_t colon;              // the status of the colon, 0 -- off, 1 -- on
} seg7Display_t;

// Update the 4-digit 7-segment display with digit numbers (not 7-segment display pattern)
void seg7DigitUpdate(seg7Display_t *seg7Display);

#endif /* SEG7DIGIT_H_ */
