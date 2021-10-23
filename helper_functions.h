#include "xil_types.h"
#include "seg7_display.h"
#include "math.h"


float counter;
volatile int previous_num1;
volatile int previous_num2;
u8 shift_flag;
volatile u8 showing_memory;
volatile u8 showing_result;
u16 slideSwitchIn;
u8 use_value_next_calculation_flag;

u8 pushBtnLeftIn;
u8 pushBtnRightIn;
u8 pushBtnUpIn;
u8 pushBtnDownIn;

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
