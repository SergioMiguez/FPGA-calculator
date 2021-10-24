#ifndef __SEG7_DISPLAY_H_
#define __SEG7_DISPLAY_H_

#include "xgpio.h"		// Added for xgpio object definitions
#include "gpio_init.h"
#include "main_structure.h"
#include <stdio.h>
#include "xil_types.h"		// Added for integer type definitions
#include "math.h"

// Definitions for 7-segment BCD codes
#define DIGIT_BLANK		0xFF
#define DIGIT_ZERO 		0xC0
#define DIGIT_ONE  		0xF9
#define DIGIT_TWO  		0xA4
#define DIGIT_THREE  	0xB0
#define DIGIT_FOUR  	0x99
#define DIGIT_FIVE  	0x92
#define DIGIT_SIX  		0x82
#define DIGIT_SEVEN  	0xF8
#define DIGIT_EIGHT  	0x80
#define DIGIT_NINE  	0x90
#define DIGIT_DASH  	0xBF
#define DIGIT_DOT		0x7F

// Definitions for 7-segment BCD codes for values with dot included
#define DIGIT_ZERO_DOT	0x40
#define DIGIT_ONE_DOT	0x79
#define DIGIT_TWO_DOT	0x24
#define DIGIT_THREE_DOT	0x30
#define DIGIT_FOUR_DOT	0x19
#define DIGIT_FIVE_DOT	0x12
#define DIGIT_SIX_DOT	0x02
#define DIGIT_SEVEN_DOT	0x78
#define DIGIT_EIGHT_DOT	0x00
#define DIGIT_NINE_DOT	0x10


#define NUMBER_BLANK  	10 	// Note: since 10 cannot be a digit,
 						   	//       it is used to represent a blank digit
#define NUMBER_DASH  	11 	// Note: since 11 cannot be a digit,
 						   	//       it is used to represent "dash"

// Definitions for digit selection codes
#define EN_FIRST_SEG	0b0111
#define EN_SECOND_SEG	0b1011
#define EN_THIRD_SEG	0b1101
#define EN_FOURTH_SEG	0b1110

// Definitions to turn on LEDs on SHIFT or CHAIN mode
#define CHAIN_OPERATION 0b0000111111000000
#define SHIFT_ON 0b1111000000000000

/**
 * Prototype function to print strings.
 */
void print(char *str);
/**
 * Prototype function from xinterruptES3.c.
 */
int setUpInterruptSystem();
/**
 * Prototype function from timer_interrupt_func.c.
 */
void hwTimerISR(void *CallbackRef);
/**
 * Function used to handle and try to display the given number.
 */
void displayNumber(float number);
/**
 * Function used to calculate the digits that must be displayed individually.
 */
void calculateDigits(float number);
/**
 * Function to finally display the previously calculated digits into the display.
 */
void displayDigit();
/**
 * Function to calculate the number of digits that value has.
 */
int number_of_figures(int);
/**
 * Function to crop and display correctly the numbers depending on their properties
 */
void display_figure(float);
/**
 * Function to crop the number with the correct digits to be displayed
 */
float float_crop_value(float);
/**
 * Function to crop the number with the correct digits to be displayed.
 */
int int_crop_value(float);

#endif
