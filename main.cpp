#include <iostream>
#include <cstdlib>
#include <cstring>

#include "driver.hpp"

int main(const int argc, const char **argv) {

	MC::Driver driver;
	driver.parse(std::cin);
	driver.print(std::cout) << "\n";

	// arg 0 -> executable file name
	// arg 1 -> custom param
//	if (argc == 2) {
//		MC::MC_Driver driver;
//		if (std::strncmp(argv[1], "-o", 2) == 0) {
//			driver.parse(std::cin);
//		} else if (std::strncmp(argv[1], "-h", 2) == 0) {
//			std::cout << "use -o for pipe to std::cin\n";
//			std::cout << "just give a filename to count from a file\n";
//			std::cout << "use -h to get this menu\n";
//			return (EXIT_SUCCESS);
//		} else {
//			/** example reading input from a file **/
//			/** assume file, prod code, use stat to check **/
//			driver.parse(argv[1]);
//		}
//		driver.print(std::cout) << "\n";
//	} else {
//		std::cout << "Invalid arguments" << std::endl;
//
//		/** exit with failure condition **/
//		return (EXIT_FAILURE);
//	}
	return EXIT_SUCCESS;
}
