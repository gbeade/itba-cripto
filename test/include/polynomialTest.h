#ifndef POLYNOMIAL_TEST_H
#define POLYNOMIAL_TEST_H

#include "cuTest.h"
#include "../../include/polynomial.h"

/* Test CONG macro*/
void testCongMacro(CuTest *const cuTest); 

/* Test polynomial creation in modulo 251 */
void testCreateLinearPolynomial(CuTest *const cuTest);
void testCreateModuloPolynomial(CuTest *const cuTest);
void testCreateNonpositiveDegreePolynomial(CuTest *const cuTest);


/* Test polynomial evaluation */
void testPolynomialEvaluation(CuTest *const cuTest);




#endif /* POLYNOMIAL_TEST_H */
