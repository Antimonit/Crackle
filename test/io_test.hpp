//
// Created by DAVE on 3. 2. 2018.
//
#ifndef __IO_TEST__HPP__
#define __IO_TEST__HPP__

#include <iostream>
#include <sstream>
#include <fstream>
#include "Driver.hpp"

using namespace std;


bool ioTest(const string& testName) {
	string inFileName = "./in/" + testName;
	string outFileName = "./out/" + testName;

	ifstream inFile(inFileName);
	ifstream expectedOutFile(outFileName);
	stringstream actualOut;

	MC::Driver driver = MC::Driver::Builder()
			.input(inFile)
			.output(actualOut)
			.heap(1024)
			.build();

	if (driver.parse() != 0) {
		throw "Parsing test \"" + testName + "\" failed.";
	}

	stringstream expectedOut;
	expectedOut << expectedOutFile.rdbuf();

	string expectedOutString = expectedOut.str();
	string actualOutString = actualOut.str();

	if (expectedOutString == actualOutString) {
		return true;
	} else {
		throw "Test \"" + testName + "\" failed.\n" +
				"Expected: " + expectedOutString +
				"Actual: " + actualOutString;
	}
}

#endif