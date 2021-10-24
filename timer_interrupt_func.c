#include "main_structure.h"
#include "gpio_init.h"
#include "seg7_display.h"


/**
 * Interrupt function which adds functionalities of the calculator.
 */
void hwTimerISR(void *CallbackRef)
{
	displayDigit();
	// taking a reading of the switches
	slideSwitchIn = XGpio_DiscreteRead(&SLIDE_SWITCHES, 1);
	/**
	 * if the button to use the previous calculation is selected we will indicate it by turning all the
	 * LEDs of NUMBER_1 as this number will be changed with the previous calculation value
	 */
	if (use_value_next_calculation_flag != 0){
		slideSwitchIn = slideSwitchIn | CHAIN_OPERATION;
	}
	/**
	 * if the SHIFT button has been pressed, then the opcode switches LEDs will turn on to indicate that the
	 * SHIFT mode is ON
	 */
	if (shift_flag == 1){
		slideSwitchIn = slideSwitchIn | SHIFT_ON;
	}
	XGpio_DiscreteWrite(&LED_OUT, 1, slideSwitchIn);

	/**
	 * The following code is used to keep displaying the result or the memory till there is a change
	 * in the inputs
	 */
	if (showing_result == 1 ){
		if (previous_num2 != get_number_2() || previous_num1 != get_number_1()) {
			showing_result = 0;
		}
	}
	if (showing_memory == 1){
		if (previous_num2 != get_number_2() || previous_num1 != get_number_1()) {
			showing_memory = 0;
		}
	}

	/**
	 * The following code is used to store the last input when the input is bein displayed. In this way,
	 * the user can press the memory button and unless a change is done in the input, the user can check this value
	 */
	if (get_opcode() == 12 && showing_result != 1 && showing_memory != 1){
		counter = get_number_2();
		previous_num2 = counter;
	}
	else if (get_opcode() == 8 && showing_result != 1 && showing_memory != 1){
		counter = get_number_1();
		previous_num1 = counter;
	}
}
