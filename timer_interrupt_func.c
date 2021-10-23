#include "helper_functions.h"
#include "gpio_init.h"

int get_number_1();
int get_number_2();
int get_opcode();

void hwTimerISR(void *CallbackRef)
{
	displayDigit();
	slideSwitchIn = XGpio_DiscreteRead(&SLIDE_SWITCHES, 1);
	if (use_value_next_calculation_flag == 0){
		XGpio_DiscreteWrite(&LED_OUT, 1, slideSwitchIn);
	} else {
		slideSwitchIn = slideSwitchIn | 0b0000111111000000;
		XGpio_DiscreteWrite(&LED_OUT, 1, slideSwitchIn);
	}
	if (shift_flag == 1){
		slideSwitchIn = slideSwitchIn | 0b1111000000000000;
		XGpio_DiscreteWrite(&LED_OUT, 1, slideSwitchIn);
	}

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

	if (get_opcode() == 12 && showing_result != 1 && showing_memory != 1){
		counter = get_number_2();
		previous_num2 = counter;
	}
	else if (get_opcode() == 8 && showing_result != 1 && showing_memory != 1){
		counter = get_number_1();
		previous_num1 = counter;
	}
}
