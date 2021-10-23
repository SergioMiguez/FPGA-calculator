#include "xil_types.h"
#include "seg7_display.h"
#include "math.h"

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
// Variable used to store values in a "Memory A"
float mem_A;
// Variable used to store values in a "Memory B"
float mem_B;
// Variable used to indicate where the information must be stored Mem A = 0 - Mem B = 1 or retrieved
u8 last_mem_used;
/**
 *  (In a chain calculation)
 *  Variable used to store the previous value which will be used in the next calculation.
 */
float use_value_next_calculation;
// Variable used to indicate if the use_value_next_calculation must save a new value
u8 use_value_next_calculation_flag; // used to determine if
// Variable used to indicate if the calculator is in SHIFT mode or not
u8 shift_flag;
/**
 * Variable used to keep track of the reference position of the numbers displayed
 * Used when a figure is more than 4 digits and we want to show figures out of the screen
 */
int digit_displayed_reference;
// Variable which stores the number of decimal figures in a given input
int max_number;
// Variable used to indicate if the Left Button has been pressed
u8 pushBtnLeftIn;
// Variable used to indicate if the Right Button has been pressed
u8 pushBtnRightIn;
// Variable used to indicate if the Up Button has been pressed
u8 pushBtnUpIn;
// Variable used to indicate if the Down Button has been pressed
u8 pushBtnDownIn;
// Variable to store the values inputed in all the switches
u16 slideSwitchIn;
// Variable used to display the numbers
float counter;
// Volatile variable used to indicate if the input has changed and needs to be displayed
volatile u8 input_changed;
// Volatile variable used to indicate if a result is being shown
volatile u8 showing_result;
// Volatile variable used to store the previous number_1 to check if the input has changed
volatile int previous_num1;
// Volatile variable used to store the previous number_2 to check if the input has changed
volatile int previous_num2;
// Volatile variable used to indicate if a memory value is being shown
volatile u8 showing_memory;

/**
 * Declaration of function prototypes contained in helper_functions.c
 */
void button_pressed();
void set_use_value_next_calculation();
int get_opcode();
int get_number_1();
int get_number_2();
int convert_to_twos_complement(unsigned int);
void left_button_pressed();
void right_button_pressed();
void up_button_pressed();
void down_button_pressed();
int number_of_figures(int);
void display_figure(float);
float float_crop_value(float);
int int_crop_value(float);

/**
 * Declaration of function prototypes contained in arith_operations.c
 */
float calculate(int, float, float);
