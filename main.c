#include <stdio.h>
#include "platform.h"
#include "xil_types.h"		// Added for integer type definitions
#include "seg7_display.h"	// Added for 7-segment definitions
#include "gpio_init.h"
#include "math.h"

float mem_A = 0;
float mem_B = 0;
float use_value_next_calculation = 0;
int use_value_next_calculation_flag = 0; // used to determine if
int shift_flag = 0;

int digit_displayed_reference = 0;
int max_number = 0;

// Declare and define required variables
u16 pushBtnLeftIn = 0;
u16 pushBtnRightIn = 0;
u16 pushBtnUpIn = 0;
u16 pushBtnDownIn = 0;

u16 slideSwitchIn = 0;


float counter = 0;
volatile int switch_counter = 0;
volatile int input_changed = 0;
volatile int showing_result = 0;
volatile int previous_num1 = 0;
volatile int previous_num2 = 0;
volatile int showing_memory = 0;
uint last_mem_used = 0; // indicates where the information must be stored Mem A = 0 - Mem B = 1

int get_number_1();
int get_number_2();
int twos_complement_to_dec(unsigned int);
int get_opcode();
void set_use_value_next_calculation();
float calculate(int, float, float);
double pow (double, double);
float fabsf (float);
void button_pressed();

void left_button_pressed();
void right_button_pressed();
void up_button_pressed();
void down_button_pressed();

int number_of_figures(int);
void display_figure(float);
float float_crop_value(float);
int int_crop_value(float);

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
		pushBtnLeftIn = XGpio_DiscreteRead(&P_BTN_LEFT, 1);
		pushBtnRightIn = XGpio_DiscreteRead(&P_BTN_RIGHT, 1);
		pushBtnUpIn = XGpio_DiscreteRead(&P_BTN_UP, 1);
		pushBtnDownIn = XGpio_DiscreteRead(&P_BTN_DOWN, 1);

		button_pressed();
	}

    cleanup_platform();
    return 0;
}

void button_pressed(){
	/**
	 * Memory A button pressed - show and select the latest result of this memory
	 */
	if (pushBtnLeftIn == 1){
		left_button_pressed();
	}
	/**
	 * Shift option to increase functionality
	 */
	else if (pushBtnRightIn == 1) {
		right_button_pressed();
	}
	/**
	 * Use Last Result in Next Calculation button pressed
	 * It sets a different instruction model for the next operation where only OPCODE (arithmetic operation)
	 * & Number_2 will be accepted. Number_1 will implicitly be the last result displayed. In the case of 1
	 * number needed in the operation (EXP, SQRT) then only the OPCODE will be accepted.
	 */
	else if (pushBtnUpIn == 1) {
		up_button_pressed();
	}
	/**
	 * EQUALS "=" button pressed - make an operation and save it in the latest selected memory
	 */
	else if (pushBtnDownIn == 1) {
		down_button_pressed();
	}
}

void set_use_value_next_calculation(){
	if (last_mem_used == 0){
		use_value_next_calculation = mem_A;
	} else {
		use_value_next_calculation = mem_B;
	}
	max_number = number_of_figures(use_value_next_calculation);
}

int get_number_1(){
	unsigned int num1 = 0;
	slideSwitchIn = XGpio_DiscreteRead(&SLIDE_SWITCHES, 1);
	num1 = (slideSwitchIn % (int) pow(2,12)) >> 6;
	if ((slideSwitchIn & 0b0000100000000000) == 0b0000100000000000){
		num1 = twos_complement_to_dec(num1);
	}
	//xil_printf("%d\n", num1);
	return num1;
}

int get_number_2(){
	int num2 = 0;
	slideSwitchIn = XGpio_DiscreteRead(&SLIDE_SWITCHES, 1);
	num2 = (slideSwitchIn % (int) pow(2,6));
	if ((slideSwitchIn & 0b0000000000100000) == 0b0000000000100000){
		num2 = twos_complement_to_dec(num2);
	}
	//xil_printf("%d\n", num2);
	return num2;
}

int twos_complement_to_dec(unsigned int num){
	num = (-pow(2,6)) + ((int)(num << 1) >> 1);
	return num;
}

int get_opcode(){
	int opcode = 0;
	slideSwitchIn = XGpio_DiscreteRead(&SLIDE_SWITCHES, 1);
	opcode = (slideSwitchIn % (int)pow(2,16)) >> 12;
	//xil_printf("%d\n", opcode);
	return opcode;
}

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

void left_button_pressed() {
	if (get_opcode() != 1){
		// selecting memory option
		use_value_next_calculation_flag = 0;
		if (showing_memory == 0){
			showing_memory = 1;
		}

		if (last_mem_used != 0){
			counter = mem_A;
			last_mem_used = 0;
		} else {
			counter = mem_B;
			last_mem_used = 1;
		}

		previous_num2 = get_number_2();
		previous_num1 = get_number_1();
		use_value_next_calculation = counter;

	} else {
		// shifting the number to the left
		//xil_printf("Reference: %d\n", digit_displayed_reference);
		max_number = number_of_figures((int)use_value_next_calculation);
		//xil_printf("Max: %d\n", max_number);
		if (max_number - digit_displayed_reference > 4){
			digit_displayed_reference++;
			//xil_printf("New Reference: %d\n", digit_displayed_reference);
		}
	}

	while(pushBtnLeftIn != 0) {
		pushBtnLeftIn = XGpio_DiscreteRead(&P_BTN_LEFT, 1);
		display_figure(counter);
	}
	xil_printf("Crop: %d\n", int_crop_value(counter));
}

void right_button_pressed() {
	if (get_opcode() != 1){
		if (shift_flag == 0){
			shift_flag = 1;
		} else {
			shift_flag = 0;
		}
	} else {
		//xil_printf("Reference: %d\n", use_value_next_calculation);
		max_number = number_of_figures((int)use_value_next_calculation);
		//xil_printf("Max: %d\n", max_number);
		if (digit_displayed_reference > 0){
			digit_displayed_reference--;
			//xil_printf("Reference: %d\n", digit_displayed_reference);
		}
	}
	while(pushBtnRightIn != 0) {
		pushBtnRightIn = XGpio_DiscreteRead(&P_BTN_RIGHT, 1);
		display_figure(counter);
	}
	xil_printf("Crop: %d\n", int_crop_value(counter));

}

void up_button_pressed(){
	if (use_value_next_calculation_flag == 0){
		use_value_next_calculation_flag = 1;
		set_use_value_next_calculation();
	} else {
		use_value_next_calculation_flag = 0;
	}
	while(pushBtnUpIn != 0) {
		pushBtnUpIn = XGpio_DiscreteRead(&P_BTN_UP, 1);
		display_figure(counter);
	}
}

void down_button_pressed(){
	set_use_value_next_calculation();
	int opcode = get_opcode();
	digit_displayed_reference = 0;
	if (shift_flag == 1) opcode = opcode + 16;
	if (opcode != 8 || opcode != 12){
		if (use_value_next_calculation_flag){
			counter = calculate(opcode, use_value_next_calculation, get_number_2());
			previous_num1 = use_value_next_calculation;
			previous_num2 = get_number_2();
		} else {
			counter = calculate(opcode, get_number_1(), get_number_2());
			previous_num1 = get_number_1();
			previous_num2 = get_number_2();
		}

		if (last_mem_used == 0){
			mem_A = counter;
		} else {
			mem_B = counter;
		}
		use_value_next_calculation = counter;
		xil_printf("Result: %d\n",(int) counter);
		showing_result = 1;
	}

	while(pushBtnDownIn != 0) {
		pushBtnDownIn = XGpio_DiscreteRead(&P_BTN_DOWN, 1);
		display_figure(counter);
	}
	if (shift_flag == 1) shift_flag = 0;
}

int number_of_figures (int n) {
    if (n == 0) return 1;
    return floor (log10 ((int)fabsf (n))) + 1;
}

float float_crop_value(float num){
	return fmodf(num / pow(10, digit_displayed_reference), 10000);
}

int int_crop_value(float num){
	return (int) (fmodf(num / pow(10, digit_displayed_reference), 10000));
}

void display_figure(float num){
	if (num < 0 || fmodf(num,1) != 0){
		displayNumber(float_crop_value(num));
	} else {
		displayNumber(int_crop_value(num));
	}
}
