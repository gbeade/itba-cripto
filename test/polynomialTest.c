#include <stdio.h>
#include "include/cuTest.h"
#include "../include/polynomial.h"

void testCongMacro(CuTest *const cuTest) {
    CuAssertIntEquals(cuTest, CONG(1), 1);
    CuAssertIntEquals(cuTest, CONG(-1), 250);
    CuAssertIntEquals(cuTest, CONG(250), 250);
    CuAssertIntEquals(cuTest, CONG(-250), 1);
    CuAssertIntEquals(cuTest, CONG(251), 0);
    CuAssertIntEquals(cuTest, CONG(0), 0);
    CuAssertIntEquals(cuTest, CONG(1000000), 16);
    CuAssertIntEquals(cuTest, CONG(-1000000), 235);
}


void testCreateLinearPolynomial(CuTest *const cuTest) {
    Polynomial * poly1 = polyFromCoefficients(2, 1, 1); 
    Polynomial * poly2 = polyFromCoefficients(2, 1, 252); 
    Polynomial * poly3 = polyFromCoefficients(2, 0, 0); 
    Polynomial * poly4 = polyFromCoefficients(2, 1, 2);

    CuAssertPtrNotNull(cuTest, poly1); 
    CuAssertPtrNotNull(cuTest, poly2); 
    CuAssertPtrNotNull(cuTest, poly3); 
    CuAssertPtrNotNull(cuTest, poly4);  

    CuAssertIntEquals(cuTest, poly1->coefficients[0], 1);
    CuAssertIntEquals(cuTest, poly1->coefficients[1], 1);

    CuAssertIntEquals(cuTest, poly2->coefficients[0], 1);
    CuAssertIntEquals(cuTest, poly2->coefficients[1], 1);

    CuAssertIntEquals(cuTest, poly3->coefficients[0], 0);
    CuAssertIntEquals(cuTest, poly3->coefficients[1], 0);

    CuAssertIntEquals(cuTest, poly4->coefficients[0], 1);
    CuAssertIntEquals(cuTest, poly4->coefficients[1], 2);

    polyFree(poly1); 
    polyFree(poly2); 
    polyFree(poly3); 
    polyFree(poly4); 
}

void testCreateModuloPolynomial(CuTest *const cuTest) {
    Polynomial * poly1 = polyFromCoefficients(3, -1, 255, 0); 
    Polynomial * poly2 = polyFromCoefficients(2, -255, -2); 
    Polynomial * poly3 = polyFromCoefficients(1, 0); 
    Polynomial * poly4 = polyFromCoefficients(6, 251, 500, 1, 2, 3, -1000); 

    CuAssertPtrNotNull(cuTest, poly1); 
    CuAssertPtrNotNull(cuTest, poly2); 
    CuAssertPtrNotNull(cuTest, poly3); 
    CuAssertPtrNotNull(cuTest, poly4); 

    CuAssertIntEquals(cuTest, poly1->coefficients[0], 250);
    CuAssertIntEquals(cuTest, poly1->coefficients[1], 4);
    CuAssertIntEquals(cuTest, poly1->coefficients[2], 0);

    CuAssertIntEquals(cuTest, poly2->coefficients[0], 247);
    CuAssertIntEquals(cuTest, poly2->coefficients[1], 249);

    CuAssertIntEquals(cuTest, poly3->coefficients[0], 0);

    CuAssertIntEquals(cuTest, poly4->coefficients[0], 0);
    CuAssertIntEquals(cuTest, poly4->coefficients[1], 249);
    CuAssertIntEquals(cuTest, poly4->coefficients[2], 1);    
    CuAssertIntEquals(cuTest, poly4->coefficients[3], 2);
    CuAssertIntEquals(cuTest, poly4->coefficients[4], 3);
    CuAssertIntEquals(cuTest, poly4->coefficients[5], 4);

    polyFree(poly1); 
    polyFree(poly2); 
    polyFree(poly3); 
    polyFree(poly4); 
}

void testCreateNonpositiveDegreePolynomial(CuTest *const cuTest) {
    Polynomial * poly1 = polyFromCoefficients(0);
    Polynomial * poly2 = polyFromCoefficients(-1);
    Polynomial * poly3 = polyFromCoefficients(0, 1, 2);
    Polynomial * poly4 = polyFromCoefficients(-1, 2, 3, 4);

    CuAssertPtrEquals(cuTest, poly1, (Polynomial *)NULL); 
    CuAssertPtrEquals(cuTest, poly2, (Polynomial *)NULL); 
    CuAssertPtrEquals(cuTest, poly3, (Polynomial *)NULL); 
    CuAssertPtrEquals(cuTest, poly4, (Polynomial *)NULL); 
}

void testPolynomialEvaluation(CuTest *const cuTest) {
    Polynomial * poly1 = polyFromCoefficients(3, 1, 1, 1);
    Polynomial * poly2 = polyFromCoefficients(3, 1, -250, 1);  
    Polynomial * poly3 = polyFromCoefficients(1, 5);  

    CuAssertIntEquals(cuTest, polyEvaluate(poly1, 1), 3);
    CuAssertIntEquals(cuTest, polyEvaluate(poly1, 0), 1);
    CuAssertIntEquals(cuTest, polyEvaluate(poly1, -1), 1);
    CuAssertIntEquals(cuTest, polyEvaluate(poly1, 251), 1);
    CuAssertIntEquals(cuTest, polyEvaluate(poly1, -250), 3);
    CuAssertIntEquals(cuTest, polyEvaluate(poly1, -500), 7);

    CuAssertIntEquals(cuTest, polyEvaluate(poly2, 1), 3);
    CuAssertIntEquals(cuTest, polyEvaluate(poly2, 0), 1);
    CuAssertIntEquals(cuTest, polyEvaluate(poly2, -1), 1);
    CuAssertIntEquals(cuTest, polyEvaluate(poly2, 190), 147);
    CuAssertIntEquals(cuTest, polyEvaluate(poly2, 500), 3);

    CuAssertIntEquals(cuTest, polyEvaluate(poly3, -1), 5);
    CuAssertIntEquals(cuTest, polyEvaluate(poly3, 1), 5);
    CuAssertIntEquals(cuTest, polyEvaluate(poly3, 0), 5);

    polyFree(poly1); 
    polyFree(poly2); 
    polyFree(poly3); 
}


static void testQuadraticInterpolation(CuTest *const cuTest, int s1, int s2, int s3, int x1, int x2, int x3, int y1, int y2, int y3) {
    int xs[3] = {x1, x2, x3}; 
    int ys[3] = {y1, y2, y3}; 
    Polynomial * poly1 = polyInterpolate(3, xs, ys); 

    CuAssertIntEquals(cuTest, poly1->coefficients[0], s1);
    CuAssertIntEquals(cuTest, poly1->coefficients[1], s2);
    CuAssertIntEquals(cuTest, poly1->coefficients[2], s3);

    polyFree(poly1); 
}


void testPolynomialInterpolation(CuTest *const cuTest) {

    /* Base case interpolation */
    testQuadraticInterpolation(cuTest, 1, 1, 1, 1, 2, 3, 3, 7, 13);
    testQuadraticInterpolation(cuTest, 250, 250, 3, 1, 2, 3, 1, 9, 23);

    /* Interpolate negative numbers - they circle back to positive numbers in Z251 */
    testQuadraticInterpolation(cuTest, 250, 250, 250, -5, -4, -3, -21, -13, -7);  

    /* We test for quadratic by they are colinear, expect s3=0 */
    testQuadraticInterpolation(cuTest, 0, 1, 0, 1, 2, 3, 1, 2, 3); 

}

