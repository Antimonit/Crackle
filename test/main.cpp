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
		MC::DriverTest::testPLUSIntBoolean();
		MC::DriverTest::testINC();

		ioTest("iterative_fibonacci.txt");
		ioTest("recursive_fibonacci.txt");
		ioTest("avl.txt");

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
