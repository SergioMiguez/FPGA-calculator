#ifndef MAIN_STRUCTURE__H_
#define MAIN_STRUCTURE__H_

#include "xil_types.h"
#include "seg7_display.h"
#include "math.h"
#include "gpio_init.h"
#include "arith_operations.h"

/**
 * Declaration of button variables
 */
u8 pushBtnLeftIn;
u8 pushBtnRightIn;
u8 pushBtnUpIn;
u8 pushBtnDownIn;

/**
 * Declaration of other used variables
 */
// Variable used to store values in a "Memory A".
float mem_A;
// Variable used to store values in a "Memory B".
float mem_B;
// Variable used to indicate where the information must be stored Mem A = 0 - Mem B = 1 or retrieved.
u8 last_mem_used;
/**
 *  (In a chain calculation)
 *  Variable used to store the previous value which will be used in the next calculation.
 */
float use_value_next_calculation;
// Variable used to indicate if the use_value_next_calculation must save a new value.
u8 use_value_next_calculation_flag;
// Variable used to indicate if the calculator is in SHIFT mode or not.
u8 shift_flag;
/**
 * Variable used to keep track of the reference position of the displayed numbers.
 * It is used when a figure has more than 4 digits and we want to show figures out of the screen.
 */
int digit_displayed_reference;
// Variable which stores the number of decimal figures from a given input.
int max_number;
// Variable used to indicate if the Left Button has been pressed.
u8 pushBtnLeftIn;
// Variable used to indicate if the Right Button has been pressed.
u8 pushBtnRightIn;
// Variable used to indicate if the Up Button has been pressed.
u8 pushBtnUpIn;
// Variable used to indicate if the Down Button has been pressed.
u8 pushBtnDownIn;
// Variable to store the values input in all the switches.
u16 slideSwitchIn;
// Variable used to display numbers.
float counter;
/**
 * Volatile variable used to indicate if there has been a change in the input,
 * and therefore, it must be displayed.
 */
volatile u8 input_changed;
// Volatile variable used to indicate if a result is being shown.
volatile u8 showing_result;
// Volatile variable used to store the previous value of number_1 to check if the input has changed.
volatile int previous_num1;
// Volatile variable used to store the previous value of number_2 to check if the input has changed.
volatile int previous_num2;
// Volatile variable used to indicate if a memory value is being displayed.
volatile u8 showing_memory;

/**
 * Declaration of function prototypes contained in main_structure.c
 */
/**
 * Function which has the main skeleton of the calculator and depending on the pressed buttons,
 * different operations will take place.
 */
void button_pressed(void);
/**
 * Function to set the last used memory/result to be used it in the following calculation.
 */
void set_use_value_next_calculation(void);
/**
 * Function called when the LEFT button is pressed.
 * Depending on the opcode, it has 2 different functionalities.
 * If the opcode is 1, it will act as an arrow key to move left and display further digits.
 * Elsewhere, it will be used to swap between memories, MEM_A & MEM_B.
 */
void left_button_pressed(void);
/**
 * Function called when the RIGHT button is pressed. Depending on the opcode, it has 2 different
 * functionalities. If the opcode is 1, it will act as an arrow key to move right and display further
 * digits. Elsewhere, it will be used to turn on the SHIFT mode and providing further
 * functionalities/operations to the calculator, giving the chance for more opcode combinations.
 * While ON, the LEDs of the opcode switches will be all ON.
 */
void right_button_pressed(void);
/**
 * Function called when the UP button is pressed. It will be use to select the last result or last
 * selected memory, and substitute NUMBER_1 with this value. This allows the user to make chained calculations.
 * While using this mode, all the LEDs of NUMBER_1 will be ON.
 */
void up_button_pressed(void);
/**
 * Function called when the DOWN button is pressed. The button has the equals "=" function.
 * When pressed, a calculation will take place and a result will be displayed.
 */
void down_button_pressed(void);
/**
 * Function used to extract the value of the opcode from the input switches.
 * This value will be determined by the first 4 switches starting from the left.
 * Switches 0 – 3.
 */
int get_opcode(void);
/**
 * Function used to extract the value of the first input number in the switches.
 * This value will be in determined by the switches 4 to 10 starting from the left.
 * Switches 4 – 10.
 */
int get_number_1(void);
/**
 * Function used to extract the value of the second input number from the switches.
 * This value will be determined by the last 6 switches on the right.
 * Switches 11 – 15.
 */
int get_number_2(void);

#endif /* MAIN_STRUCTURE__H_ */
