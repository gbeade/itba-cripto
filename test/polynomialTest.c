#include <stdio.h>
#include "include/cuTest.h"
#include "../include/polynomial.h"


void testCreateLinearPolynomial(CuTest *const cuTest) {
    Polynomial * poly1 = polyFromCoefficients(2, 1, 1); 
    Polynomial * poly2 = polyFromCoefficients(2, 1, 252); 
    Polynomial * poly3 = polyFromCoefficients(2, 0, 0); 
    Polynomial * poly4 = polyFromCoefficients(2, 1, 2); 

    CuAssertIntEquals(cuTest, poly1->coefficients[0], 1);
    CuAssertIntEquals(cuTest, poly1->coefficients[1], 1);

    CuAssertIntEquals(cuTest, poly2->coefficients[0], 1);
    CuAssertIntEquals(cuTest, poly2->coefficients[1], 1);

    CuAssertIntEquals(cuTest, poly3->coefficients[0], 0);
    CuAssertIntEquals(cuTest, poly3->coefficients[1], 0);

    CuAssertIntEquals(cuTest, poly4->coefficients[0], 1);
    CuAssertIntEquals(cuTest, poly4->coefficients[1], 2);
}

void testCreateModuloPolynomial(CuTest *const cuTest) {
    // Polynomial * poly1 = polyFromCoefficients(3, -1, 255, 0); 
    // Polynomial * poly2 = polyFromCoefficients(2, -255, -2); 
    Polynomial * poly3 = polyFromCoefficients(1, 0); 
    // Polynomial * poly4 = polyFromCoefficients(6, 251, 500, 1, 2, 3, -1000); 

    // CuAssertIntEquals(cuTest, poly1->coefficients[0], 250);
    // CuAssertIntEquals(cuTest, poly1->coefficients[1], 4);
    // CuAssertIntEquals(cuTest, poly1->coefficients[2], 0);

    // CuAssertIntEquals(cuTest, poly2->coefficients[0], 247);
    // CuAssertIntEquals(cuTest, poly2->coefficients[1], 249);

    CuAssertIntEquals(cuTest, poly3->coefficients[0], 0);

    // CuAssertIntEquals(cuTest, poly4->coefficients[0], 0);
    // CuAssertIntEquals(cuTest, poly4->coefficients[1], 249);
    // CuAssertIntEquals(cuTest, poly4->coefficients[2], 1);    
    // CuAssertIntEquals(cuTest, poly4->coefficients[3], 2);
    // CuAssertIntEquals(cuTest, poly4->coefficients[4], 3);
    // CuAssertIntEquals(cuTest, poly4->coefficients[5], 4);

}
