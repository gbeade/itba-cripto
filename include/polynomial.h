#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <stdint.h>
#define MOD 251


typedef struct Polynomial {
    uint8_t* coefficients; 
    uint8_t degree; 
} Polynomial;

/* Creates a new polynomial from a variable number of unsigned integer coefficients. Terms grow from left to right. */
Polynomial* polyFromCoefficients(unsigned int n, ...);

/* Evaluates a polynomial on a parameter x */
uint8_t polyEvaluate(Polynomial* poly, unsigned int x); 

/* Prints a polynomial */
void polyPrint(Polynomial* poly); 

/* Interpolates n given values into a n-1 degree polynomial*/
Polynomial* polyInterpolate(unsigned int n, ...); 


#endif // POLYNOMIAL_H