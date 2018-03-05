//
// Created by DAVE on 3. 2. 2018.
//

#include <c++/iostream>
#include "io_test.hpp"
#include "DriverTest.hpp"

int main(const int argc, const char **argv) {

	std::cout << "Starting tests." << std::endl;

	try {
		MC::DriverTest::testEQ();
		MC::DriverTest::testPLUSIntInt();
		MC::DriverTest::testPLUSDoubleDouble();
		MC::DriverTest::testPLUSIntDouble();
		std::cout << "Starting tests 2." << std::endl;
		MC::DriverTest::testPLUSIntBoolean();
		std::cout << "Starting tests 3." << std::endl;
		MC::DriverTest::testINC();
		std::cout << "Starting tests 4." << std::endl;

		ioTest("iterative_fibonacci.txt");
		std::cout << "Starting tests." << std::endl;
		ioTest("recursive_fibonacci.txt");
		std::cout << "Starting tests." << std::endl;
		ioTest("avl.txt");
		std::cout << "Starting tests." << std::endl;

	} catch (const std::string& e) {
		std::cerr << e << std::endl;
		return (EXIT_FAILURE);
	} catch (const char* e) {
		std::cerr << e << std::endl;
		return (EXIT_FAILURE);
	} catch (...) {
		std::cerr << "Test failed." << std::endl;
		return (EXIT_FAILURE);
	}

	std::cout << "All tests passed." << std::endl;
	return (EXIT_SUCCESS);
}
