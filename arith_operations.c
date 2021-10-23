#include "xil_types.h"
#include "stdio.h"
#include "math.h"

double exp (double);
float fmodf(float, float);

int get_number_1();
int get_number_2();
int get_opcode();

float adder (float augend, float addend) {
	float sum;
	sum = augend + addend;
	return sum;
}

float subtractor(float minuend, float subtrahend) {
	float difference;
	difference = minuend - subtrahend;
	return difference;
}

float multiplicator (float multiplicand, float multiplier){
	float multiplication;
	multiplication = multiplicand * multiplier;
	return multiplication;
}

float divider (float dividend, float divisor){
	float division;
	if (divisor == 0){
		return -9999;
	} else {
		division = dividend / divisor;
	}

	return division;
}

float modulator (float dividend, float divisor) {
	float module;
	module = fmodf(dividend, divisor);
	return module;
}

float power (float base, float exponent) {
	float sol;
	sol = pow(base, exponent);
	return sol;
}

float exponential (float exponent) {
	float sol;
	sol = exp(exponent);
	return sol;
}

float calculate(int opcode, float num1, float num2){
	float result = 0;
	switch(opcode) {
		case 0 :
			result = -9999;
			xil_printf("The printing inputs\n");
			break;
		case 8 :
			result = get_number_1();
			break;
		case 12 :
			result = get_number_2();
			break;
		case 1 :
			result = -9999;
			xil_printf("Change buttons and shift to sides\n");
			break;
		case 2 :
			result = adder(num1, num2);
			//xil_printf(" The addition result is: %d\n\r", result);
			break;
		case 3 :
			result = subtractor(num1, num2);
			//xil_printf(" The subtraction result is: %d\n\r", result);
			break;
		case 4 :
			result = multiplicator(num1, num2);
			//xil_printf(" The multiplication result is: %d\n\r", result);
			break;
		case 5 :
			result = divider(num1, num2);
			//xil_printf(" The division result is: %d\n\r", result);
			break;
		case 6 :
			result = modulator(num1, num2);
			//xil_printf(" The modulator result is: %d\n\r", result);
			break;
		case 7 :
			result = power(num1, num2);
			//xil_printf(" The power result is: %d\n\r", result);
			break;
		case 9 :
			result = exponential(num1);
			//xil_printf(" The exponential result is: %d\n\r", result);
			break;
		case 10 :
			result = sqrt(num1);
			break;
		case 11 :
			result = sin(num1);
			break;
		case 13 :
			result = cos(num1);
			break;
		case 14 :
			result = tan(num1);
			break;
		case 15 :
			result = asin(num1);
			break;
		case 16 :
			result = acos(num1);
			break;
		case 17 :
			result = atan(num1);
			break;
		case 18 :
			result = log2f(num1);
			break;
		case 19 :
			result = log10(num1);
			break;
		case 20 :
			result = log(num1);
			break;
		case 21 :
			result = power(num1, 2);
			break;
		case 22 :
			result = power(num1, 3);
			break;
		case 23 :
			result = power(10, num1);
			break;
		case 24 :
			result = sinh(num1);
			break;
		case 25 :
			result = cosh(num1);
			break;
		case 26 :
			result = tanh(num1);
			break;
		case 27 :
			result = asinh(num1);
			break;
		case 28 :
			result = acosh(num1);
			break;
		case 29 :
			result = atanh(num1);
			break;
		default:
			//xil_printf("Error! The operation type (%c) is wrong!\n\r", opcode);
			break;
	}
	return result;
}

