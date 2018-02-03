//
// Created by DAVE on 3. 2. 2018.
//
#include <c++/iostream>
#include <fstream>
#include <c++/cstring>
#include <c++/sstream>
#include "Driver.hpp"

bool test(const std::string& testName) {
	MC::Driver::Builder builder;

	std::string folder = std::string("./");
	std::string prefix = std::string("test_");
	std::string in_postfix = std::string("_in.txt");
	std::string out_postfix = std::string("_out.txt");

	std::string inFileName = folder + prefix + testName + in_postfix;
	std::string outFileName = folder + prefix + testName + out_postfix;

	std::ifstream inFile(inFileName);
	std::ifstream expectedOutFile(outFileName);
	std::stringstream actualOut;

	builder.input(inFile);
	builder.output(actualOut);
//	builder.debug();
	builder.heap(1024);

	if (builder.build().parse() != 0) {
		std::cerr << "Parsing failed!" << std::endl;
		return false;
	}

	std::stringstream expectedOut;
	expectedOut << expectedOutFile.rdbuf();

	std::string expectedOutString = expectedOut.str();
	std::string actualOutString = actualOut.str();

//	std::cout << '"' << expectedOutString << '"' << std::endl;
//	std::cout << '"' << actualOutString << '"' << std::endl;

	return expectedOutString == actualOutString;
}

int main(const int argc, const char **argv) {

	std::cout << "Starting tests." << std::endl;
	int failed = 0;

	failed += test("iterative_fibonacci") ? 0 : 1;

	if (failed > 0) {
		std::cout << "Tests failed." << std::endl;
		return (EXIT_FAILURE);
	} else {
		std::cout << "All tests passed." << std::endl;
		return (EXIT_SUCCESS);
	}
}
