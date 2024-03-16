#ifndef ARITH_OPERATIONS_H_
#define ARITH_OPERATIONS_H_

#include "main_structure.h"
#include "xil_types.h"
#include "stdio.h"
#include "math.h"

/**
 * Prototype of functions used from arith_operations.c
 */
/**
 * Function to calculate the sum between two numbers
 */
float adder (float, float);
/**
 * Function to calculate the subtraction between two numbers
 */
float subtractor(float, float);
/**
 * Function to calculate the multiplication between two numbers
 */
float multiplicator (float, float);
/**
 * Function to calculate the division between two numbers
 */
float divider (float, float);
/**
 * Function to calculate the modulo between two numbers
 */
float modulator (float, float);
/**
 * Function to calculate the power between two numbers
 */
float power (float, float);
/**
 * Function to calculate the square root of a number
 */
float square_root(float);
/**
 * Function to calculate the exponential of a value
 */
float exponential (float);
/**
 * Function to select the operation to be done depending on the value of the opcode.
 * Case 1 is save to active arrow buttons and cases 8 and 12 are saved to display input figures.
 */
float calculate(int, float, float);
/**
 * Function to convert the input value into 2s complements to let the user input
 * negative and positive values.
 */
int convert_to_twos_complement(unsigned int);

#endif /* ARITH_OPERATIONS_H_ */
