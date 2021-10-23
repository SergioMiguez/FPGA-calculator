#include <stdio.h>
#include "platform.h"
#include "xil_types.h"		// Added for integer type definitions
//#include "seg7_display.h"	// Added for 7-segment definitions
#include "gpio_init.h"
#include "helper_functions.h"

/**
 * Prototype of functions
 */
void button_pressed();
void display_figure(float);

/**
 * Main Function which contains the while loop which will run continuously in the calculator
 */
int main()
{
	init_platform();
	int status;

    // Initialize the GPIOs
    status = initGpio();
	if (status != XST_SUCCESS) {
		print("GPIOs initialization failed!\n\r");
		cleanup_platform();
		return 0;
	}

	// Setup the Interrupt System
	status = setUpInterruptSystem();
	if (status != XST_SUCCESS) {
		print("Interrupt system setup failed!\n\r");
		cleanup_platform();
		return 0;
	}

	while (1)
	{
		display_figure(counter);
		// making a reading of each button
		pushBtnLeftIn = XGpio_DiscreteRead(&P_BTN_LEFT, 1);
		pushBtnRightIn = XGpio_DiscreteRead(&P_BTN_RIGHT, 1);
		pushBtnUpIn = XGpio_DiscreteRead(&P_BTN_UP, 1);
		pushBtnDownIn = XGpio_DiscreteRead(&P_BTN_DOWN, 1);

		// state machine function to perform an action
		button_pressed();
	}

    cleanup_platform();
    return 0;
}
