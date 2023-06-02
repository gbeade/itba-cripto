#include <stdio.h>

#include "include/cuTest.h"
#include "include/polynomialTest.h"
#include "include/shadowTest.h"
#include "include/steganographyTest.h"
#include <stdlib.h>

CuSuite * getPolynomialSuiteTest(void) {
	CuSuite *const suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, testCongMacro);
	SUITE_ADD_TEST(suite, testCreateLinearPolynomial);
	SUITE_ADD_TEST(suite, testCreateModuloPolynomial);
	SUITE_ADD_TEST(suite, testCreatePolyFromBytes);
	SUITE_ADD_TEST(suite, testCreateNonpositiveDegreePolynomial);
	SUITE_ADD_TEST(suite, testPolynomialEvaluation);
	SUITE_ADD_TEST(suite, testPolynomialInterpolation);

	return suite;
}

CuSuite * getSteganographySuiteTest(void) {
	CuSuite *const suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, testHideAndShowlsb2);
	SUITE_ADD_TEST(suite, testHideAndShowlsb2new);
	SUITE_ADD_TEST(suite, testHideAndShowlsb4);
	SUITE_ADD_TEST(suite, testHidelsb2);
	SUITE_ADD_TEST(suite, testHidelsb2new);
	SUITE_ADD_TEST(suite, testHidelsb2New2);
	SUITE_ADD_TEST(suite, testHidelsb4);
	SUITE_ADD_TEST(suite, testShowlsb4);

	return suite;
}

CuSuite * getShadowSuiteTest(void) {
	CuSuite *const suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, testShadowGeneration);
	SUITE_ADD_TEST(suite, testShadowGenerationMod);


	return suite;
}

void runAllTests(void) {
	CuString *output = CuStringNew();
	CuSuite *suite = CuSuiteNew();

	CuSuite * polynomialSuite = getPolynomialSuiteTest(); 
	CuSuite * steganographySuite = getSteganographySuiteTest(); 
	CuSuite * shadowSuite = getShadowSuiteTest(); 

	CuSuiteAddSuite(suite, polynomialSuite);
	CuSuiteAddSuite(suite, steganographySuite);
	// CuSuiteAddSuite(suite, shadowSuite); TODO arreglar el test


	CuSuiteRun(suite);

	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);

	printf("%s\n", output->buffer);

	CuStringDelete(output); 
	CuSuiteDelete(steganographySuite);
	CuSuiteDelete(shadowSuite);
	CuSuiteDelete(polynomialSuite);
	free(suite); 
}

int main(void) {
	runAllTests();
	return 0;
}
