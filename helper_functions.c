#include "helper_functions.h"
#include "gpio_init.h"

/**
 * Definition of variables used
 */
// Variable used to store values in a "Memory A"
float mem_A = 0;
// Variable used to store values in a "Memory B"
float mem_B = 0;
// Variable used to indicate where the information must be stored Mem A = 0 - Mem B = 1 or retrieved
u8 last_mem_used = 0;
/**
 *  (In a chain calculation)
 *  Variable used to store the previous value which will be used in the next calculation.
 */
float use_value_next_calculation = 0;
// Variable used to indicate if the use_value_next_calculation must save a new value
u8 use_value_next_calculation_flag = 0; // used to determine if
// Variable used to indicate if the calculator is in SHIFT mode or not
u8 shift_flag = 0;
/**
 * Variable used to keep track of the reference position of the numbers displayed
 * Used when a figure is more than 4 digits and we want to show figures out of the screen
 */
int digit_displayed_reference = 0;
// Variable which stores the number of decimal figures in a given input
int max_number = 0;
// Variable used to indicate if the Left Button has been pressed
u8 pushBtnLeftIn = 0;
// Variable used to indicate if the Right Button has been pressed
u8 pushBtnRightIn = 0;
// Variable used to indicate if the Up Button has been pressed
u8 pushBtnUpIn = 0;
// Variable used to indicate if the Down Button has been pressed
u8 pushBtnDownIn = 0;
// Variable to store the values inputed in all the switches
u16 slideSwitchIn = 0;
// Variable used to display the numbers
float counter = 0;
// Volatile variable used to indicate if the input has changed and needs to be displayed
volatile u8 input_changed = 0;
// Volatile variable used to indicate if a result is being shown
volatile u8 showing_result = 0;
// Volatile variable used to store the previous number_1 to check if the input has changed
volatile int previous_num1 = 0;
// Volatile variable used to store the previous number_2 to check if the input has changed
volatile int previous_num2 = 0;
// Volatile variable used to indicate if a memory value is being shown
volatile u8 showing_memory = 0;

/**
 * Function which has the main skeleton of the calculator and depending on which buttons are pressed,
 * different operations will take place.
 */
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

/**
 * Function to set the last used memory/result to use it in the following calculation.
 */
void set_use_value_next_calculation(){
	if (last_mem_used == 0){
		use_value_next_calculation = mem_A;
	} else {
		use_value_next_calculation = mem_B;
	}
	max_number = number_of_figures(use_value_next_calculation);
}

/**
 * Function used to extract the value of the opcode from the switches input
 * This value will be in the first 4 switches starting from the left
 * Switches 0 - 3
 */
int get_opcode(){
	int opcode = 0;
	slideSwitchIn = XGpio_DiscreteRead(&SLIDE_SWITCHES, 1);
	opcode = (slideSwitchIn % (int)pow(2,16)) >> 12;
	//xil_printf("%d\n", opcode);
	return opcode;
}

/**
 * Function used to extract the value of the first input number in the switches
 * This value will be in the switches 4 to 10 starting from the left.
 * Switches 4 - 10
 */
int get_number_1(){
	unsigned int num1 = 0;
	slideSwitchIn = XGpio_DiscreteRead(&SLIDE_SWITCHES, 1);
	num1 = (slideSwitchIn % (int) pow(2,12)) >> 6;
	if ((slideSwitchIn & 0b0000100000000000) == 0b0000100000000000){
		num1 = convert_to_twos_complement(num1);
	}
	//xil_printf("%d\n", num1);
	return num1;
}

/**
 * Function used to extract the value of the second input number in the switches
 * This value will be in the last 6 switches on the right.
 * Switches 11 - 15
 */
int get_number_2(){
	int num2 = 0;
	slideSwitchIn = XGpio_DiscreteRead(&SLIDE_SWITCHES, 1);
	num2 = (slideSwitchIn % (int) pow(2,6));
	if ((slideSwitchIn & 0b0000000000100000) == 0b0000000000100000){
		num2 = convert_to_twos_complement(num2);
	}
	//xil_printf("%d\n", num2);
	return num2;
}

/**
 * Function to convert the input value into 2s complements to let the user input
 * negative and positive values.
 */
int convert_to_twos_complement(unsigned int num){
	num = (-pow(2,6)) + ((int)(num << 1) >> 1);
	return num;
}

/**
 * Function called when the LEFT button is pressed.
 * Depending on the opcode will take 2 functionalities
 * if the opcode is 1, it will act as an arrow key to move left and display further digits
 * else, it will be used to swap between memories, MEM_A & MEM_B
 */
void left_button_pressed() {
	if (get_opcode() != 1){
		// swapping the selected memory and displaying it
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
		// using the button as an arrow to see the left side of the number
		max_number = number_of_figures((int)use_value_next_calculation);
		if (max_number - digit_displayed_reference > 4){
			digit_displayed_reference++;
		}
	}

	while(pushBtnLeftIn != 0) {
		pushBtnLeftIn = XGpio_DiscreteRead(&P_BTN_LEFT, 1);
		display_figure(counter);
	}
}

/**
 * Function called when the RIGHT button is pressed.
 * Depending on the opcode will take 2 functionalities
 * if the opcode is 1, it will act as an arrow key to move right and display further digits
 * else, it will be used to turn on the SHIFT mode and give further functionalities/operations
 * to the calculator, giving the chance for more opcode combinations. While ON, the LEDs of the
 * opcode switches will be all ON.
 */
void right_button_pressed() {
	if (get_opcode() != 1){
		if (shift_flag == 0){
			shift_flag = 1;
		} else {
			shift_flag = 0;
		}
	} else {
		max_number = number_of_figures((int)use_value_next_calculation);
		if (digit_displayed_reference > 0){
			digit_displayed_reference--;
		}
	}
	while(pushBtnRightIn != 0) {
		pushBtnRightIn = XGpio_DiscreteRead(&P_BTN_RIGHT, 1);
		display_figure(counter);
	}
}

/**
 * Function called when the UP button is pressed.
 * It will be use to select the last result or last selected memory as NUMBER_1
 * This allows the user to make chained calculations. While using this mode, all
 * the LEDs of the NUMBER_1 will be ON.
 */
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

/**
 * Function called when the DOWN button is pressed.
 * The button has the equals "=" function and when pressed a calculation will
 * take place and a result will be displayed.
 */
void down_button_pressed(){
	set_use_value_next_calculation();
	int opcode = get_opcode();
	digit_displayed_reference = 0;
	if (shift_flag == 1) opcode = opcode + 16;
	// if the opcode is not == to displaying one number
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
		showing_result = 1;
	}

	while(pushBtnDownIn != 0) {
		pushBtnDownIn = XGpio_DiscreteRead(&P_BTN_DOWN, 1);
		display_figure(counter);
	}
	if (shift_flag == 1) shift_flag = 0;
}

/**
 * Function to calculate the number of digits a value has
 */
int number_of_figures (int n) {
    if (n == 0) return 1;
    return floor (log10 ((int)fabsf (n))) + 1;
}

/**
 * Function to crop and display correctly the numbers depending on their properties
 */
void display_figure(float num){
	if (num < 0 || fmodf(num,1) != 0){
		displayNumber(float_crop_value(num));
	} else {
		displayNumber(int_crop_value(num));
	}
}

/**
 * Function to crop the number with the correct digits to be displayed
 */
float float_crop_value(float num){
	return fmodf(num / pow(10, digit_displayed_reference), 10000);
}

/**
 * Function to crop the number with the correct digits to be displayed.
 */
int int_crop_value(float num){
	return (int) (fmodf(num / pow(10, digit_displayed_reference), 10000));
}
