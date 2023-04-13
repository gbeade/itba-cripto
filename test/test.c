#include <stdio.h>

#include "include/cuTest.h"
#include "include/polynomialTest.h"


CuSuite * getPolynomialSuiteTest(void) {
	CuSuite *const suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, testCongMacro);
	SUITE_ADD_TEST(suite, testCreateLinearPolynomial);
	SUITE_ADD_TEST(suite, testCreateModuloPolynomial);
	SUITE_ADD_TEST(suite, testCreateNonpositiveDegreePolynomial);
	SUITE_ADD_TEST(suite, testPolynomialEvaluation);

	return suite;
}

void runAllTests(void) {
	CuString *output = CuStringNew();
	CuSuite *suite = CuSuiteNew();

	CuSuiteAddSuite(suite, getPolynomialSuiteTest());

	CuSuiteRun(suite);

	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);

	printf("%s\n", output->buffer);
}

int main(void) {
	runAllTests();
	return 0;
}