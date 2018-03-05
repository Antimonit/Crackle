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
		MC::DriverTest::testDEC();

		std::cout << "Starting iterative fibonacci." << std::endl;
		ioTest("iterative_fibonacci.txt");

		std::cout << "Starting recursive fibonacci." << std::endl;
		ioTest("recursive_fibonacci.txt");

		std::cout << "Starting AVL." << std::endl;
		ioTest("avl.txt");

		std::cout << "All tests passed." << std::endl;
		return (EXIT_SUCCESS);

	} catch (const std::string& e) {
		std::cerr << "Test failed. " << e << std::endl;
		return (EXIT_FAILURE);
	} catch (const char* e) {
		std::cerr << "Test failed. " << e << std::endl;
		return (EXIT_FAILURE);
	} catch (...) {
		std::cerr << "Test failed." << std::endl;
		return (EXIT_FAILURE);
	}
}
