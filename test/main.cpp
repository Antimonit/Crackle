//
// Created by DAVE on 3. 2. 2018.
//
#include <iostream>
#include <sstream>
#include <fstream>
#include "Driver.hpp"

using namespace std;

bool test(const string& testName) {
	MC::Driver::Builder builder;

	string inFileName = "./in/" + testName;
	string outFileName = "./out/" + testName;

	ifstream inFile(inFileName);
	ifstream expectedOutFile(outFileName);
	stringstream actualOut;

	builder.input(inFile);
	builder.output(actualOut);
//	builder.debug();
	builder.heap(1024);

	if (builder.build().parse() != 0) {
		cerr << "Parsing test \"" << testName << "\" failed." << endl;
		return false;
	}

	stringstream expectedOut;
	expectedOut << expectedOutFile.rdbuf();

	string expectedOutString = expectedOut.str();
	string actualOutString = actualOut.str();

	if (expectedOutString == actualOutString) {
		return true;
	} else {
		cerr << "Test \"" << testName << "\" failed." << endl;
		return false;
	}
}

int main(const int argc, const char **argv) {

	cout << "Starting tests." << endl;
	int failed = 0;

	failed += test("iterative_fibonacci.txt") ? 0 : 1;
	failed += test("recursive_fibonacci.txt") ? 0 : 1;
	failed += test("avl.txt") ? 0 : 1;

	if (failed > 0) {
		cout << failed << " tests failed." << endl;
		return (EXIT_FAILURE);
	} else {
		cout << "All tests passed." << endl;
		return (EXIT_SUCCESS);
	}
}
