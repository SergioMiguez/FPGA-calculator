#include <stdio.h>
#include "xil_types.h"		// Added for integer type definitions
#include "seg7_display.h"	// Added for 7-segment definitions
#include "gpio_init.h"

// Variable to indicate if a digit has been displayed
u8 digitDisplayed = FALSE;
// Array of the digits that will be displayed
u8 digits[4];
// Variable to indicate the total number of digits that must be displayed
u8 numOfDigits;
// Variable to store the number that is currently being displayed
u8 digitToDisplay;
// Variable to indicate the number that is currently being displayed
u8 digitNumber;

/**
 * Prototype of functions used from math.h
 */
float fmodf(float, float);

/**
 * Initial function used to display a given number into the display
 * This function will handle the calculation of the digits and if it can be displayed
 */
void displayNumber(float number)
{
	u8 count;
	/* Note that 9999 is the maximum number that can be displayed
	 * Therefore, check if the number is less than or equal to 9999
	 * and display the number otherwise, display dashes in all the four segments
	 */

	if (number <= 9999999999 && number >= -999)
	{
		// Call the calculateDigits method to determine the digits of the number
		calculateDigits(number);
		/* Do not display leading zeros in a number,
		 * but if the entire number is a zero, it should be displayed.
		 * By displaying the number from the last digit, it is easier
		 * to avoid displaying leading zeros by using the numOfDigits variable
		 */
		count = 4;
		while (count > 4 - numOfDigits)
		{
			digitToDisplay = digits[count-1];
			digitNumber = count;
			count--;
			/* Wait for timer interrupt to occur and ISR to finish
			 * executing digit display instructions
			 */
			while (digitDisplayed == FALSE);
			digitDisplayed = FALSE;
		}
	}
	else
	{
		// Display "----" to indicate that the number is out of range
		count = 1;
		while (count < 5)
		{
			digitToDisplay = NUMBER_DASH;
			digitNumber = count;
			count++;
			/* Wait for timer interrupt to occur and ISR to finish
			 * executing digit display instructions
			 */
			while (digitDisplayed == FALSE);
			digitDisplayed = FALSE;
		}
	}
}

/**
 * Function used to calculate the digits that must be displayed individually.
 */
void calculateDigits(float number)
{
	u8 fourthDigit;
	u8 thirdDigit;
	u8 secondDigit;
	u8 firstDigit;

	// Check if number is up to four digits
	if (number > 9999){
		numOfDigits = 4;

		//xil_printf("Number: %d\n",number);

		fourthDigit = fmodf(number, 10);
		thirdDigit 	= fmodf(number, 100);
		secondDigit = fmodf(number, 1000);
		firstDigit 	= fmodf(number, 10000);
	}
	else if (number > 999)
	{
		numOfDigits = 4;

		fourthDigit = fmodf(number,10);
		thirdDigit = fmodf(number / 10, 10);
		secondDigit  = fmodf(number / 100, 10);
		firstDigit = number / 1000;

	}
	// Check if number is three-digits long
	else if (number >= 100 && number < 1000)
	{
		if (fmodf(number,1) == 0){
			numOfDigits = 3;

			fourthDigit = fmodf(number,10);
			thirdDigit = fmodf(number / 10, 10);
			secondDigit  = fmodf(number / 100, 10);
			firstDigit = 0;

		} else {
			// if it is a float value
			numOfDigits = 4;

			fourthDigit = fmodf(number * 10, 10);
			thirdDigit = fmodf(number / 1, 10) + 12;
			secondDigit  = fmodf(number / 10, 10);
			firstDigit = number / 100;
		}

	}
	// Check if number is two-digits long
	else if (number >= 10 && number < 100)
	{
		if (fmodf(number,1) == 0){
			numOfDigits = 2;

			fourthDigit = fmodf(number, 10);
			thirdDigit = fmodf(number / 10, 10);
			secondDigit  = 0;
			firstDigit = 0;
		} else {
			// if it is a float value
			numOfDigits = 3;

			fourthDigit = fmodf(number * 10 , 10);
			thirdDigit = fmodf(number / 1, 10) + 12;
			secondDigit  = number / 10;
			firstDigit = 0;
		}
	}
	// Check if number is one-digit long
	else if (number >= 0 && number < 10)
	{
		if (fmodf(number, 1) == 0){
			numOfDigits = 1;

			fourthDigit = fmodf(number,10);
			thirdDigit = 0;
			secondDigit = 0;
			firstDigit = 0;
		} else {
			// if it is a float value
			numOfDigits = 2;

			fourthDigit = fmodf(number * 10 , 10);
			thirdDigit = fmodf(number / 1, 10) + 12;
			secondDigit  = 0;
			firstDigit = 0;
		}

	}

	else if (number < 0 && number > -10){
		if (fmodf(number, 1) == 0){
			numOfDigits = 2;

			fourthDigit = fmodf(-number, 10);
			thirdDigit = NUMBER_DASH;
			secondDigit = 0;
			firstDigit = 0;
		} else {
			// if it is a float value
			numOfDigits = 3;

			fourthDigit = fmodf(-number * 10 , 10);
			thirdDigit = fmodf(-number / 1, 10) + 12;
			secondDigit  = NUMBER_DASH;
			firstDigit = 0;
		}
	}
	else if (number <= -10 && number > -100){
		if (fmodf(number, 1) == 0){
			numOfDigits = 3;

			fourthDigit = fmodf(-number, 10);
			thirdDigit = fmodf(-number / 10, 10);
			secondDigit = NUMBER_DASH;
			firstDigit = 0;
		} else {
			// if it is a float value
			numOfDigits = 4;

			fourthDigit = fmodf(-number * 10 , 10);
			thirdDigit = fmodf(-number / 1, 10) + 12;
			secondDigit = fmodf(-number / 10, 10);
			firstDigit = NUMBER_DASH;
		}
	}
	else if (number <= -100 && number > -1000){
		numOfDigits = 4;

		fourthDigit = fmodf(-number, 10);
		thirdDigit = fmodf(-number / 10, 10);
		secondDigit = fmodf(-number / 100, 10);
		firstDigit = NUMBER_DASH;
	}


	digits[0] = firstDigit;
	digits[1] = secondDigit;
	digits[2] = thirdDigit;
	digits[3] = fourthDigit;

	return;
}

/**
 * Function to finally display the previously calculated digits into the display
 */
void displayDigit()
{
	/*
	 * This timer ISR is used to display the digits
	 */
	switch (digitToDisplay)
	{
		case NUMBER_BLANK :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_BLANK);
			break;
		case 0 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_ZERO);
			break;
		case 1 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_ONE);
			break;
		case 2 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_TWO);
			break;
		case 3 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_THREE);
			break;
		case 4 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_FOUR);
			break;
		case 5 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_FIVE);
			break;
		case 6 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_SIX);
			break;
		case 7 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_SEVEN);
			break;
		case 8 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_EIGHT);
			break;
		case 9 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_NINE);
			break;
		case 12 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_ZERO_DOT);
			break;
		case 13 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_ONE_DOT);
			break;
		case 14 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_TWO_DOT);
			break;
		case 15 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_THREE_DOT);
			break;
		case 16 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_FOUR_DOT);
			break;
		case 17 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_FIVE_DOT);
			break;
		case 18 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_SIX_DOT);
			break;
		case 19 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_SEVEN_DOT);
			break;
		case 20 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_EIGHT_DOT);
			break;
		case 21 :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_NINE_DOT);
			break;
		case NUMBER_DASH :
			XGpio_DiscreteWrite(&SEG7_HEX_OUT, 1, DIGIT_DASH);
			break;
		default:
			break;
	}

	// Select the appropriate digit
	if (digitNumber == 1) {
		XGpio_DiscreteWrite(&SEG7_SEL_OUT, 1, EN_FIRST_SEG);
	}
	else if (digitNumber == 2) {
		XGpio_DiscreteWrite(&SEG7_SEL_OUT, 1, EN_SECOND_SEG);
	}
	else if (digitNumber == 3) {
		XGpio_DiscreteWrite(&SEG7_SEL_OUT, 1, EN_THIRD_SEG);
	}
	else if (digitNumber == 4) {
		XGpio_DiscreteWrite(&SEG7_SEL_OUT, 1, EN_FOURTH_SEG);
	}

	digitDisplayed = TRUE;
	return;
}
