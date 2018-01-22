#include <iostream>
#include <cstring>

#include "driver.hpp"

int main(const int argc, const char **argv) {

	MC::Driver driver;

	for (int argx = 1; argx < argc; argx++) {
		if (std::strncmp(argv[argx], "-i", 2) == 0) {
			argx++;
			if (argx < argc) {
				driver.input(std::string(argv[argx]));
			} else {
				std::cout << "Unspecified input file name." << std::endl;
				return (EXIT_FAILURE);
			}
		} else if (std::strncmp(argv[argx], "-o", 2) == 0) {
			argx++;
			if (argx < argc) {
				driver.output(std::string(argv[argx]));
			} else {
				std::cout << "Unspecified output file name." << std::endl;
				return (EXIT_FAILURE);
			}
		} else if (std::strncmp(argv[argx], "-d", 2) == 0) {
			argx++;
			if (argx < argc) {
				driver.debug(std::string(argv[argx]));
			} else {
				std::cout << "Unspecified debug file name." << std::endl;
				return (EXIT_FAILURE);
			}
		} else if (std::strncmp(argv[argx], "-h", 2) == 0) {
			std::cout << "Use -i <path_to_file> to specify input file (defaults to standard input)" << std::endl;
			std::cout << "Use -o <path_to_file> to specify output file (defaults to standard output)" << std::endl;
			std::cout << "Use -d <path_to_file> to specify debug file (none by default)" << std::endl;
			std::cout << "Use -h to get help." << std::endl;
			return (EXIT_SUCCESS);
		} else {
			std::cout << "Invalid arguments" << std::endl;
			return (EXIT_FAILURE);
		}
	}

	const int accept(0);
	if (driver.parse() == accept) {
		std::cout << "Parsing successful" << std::endl;
	} else {
		std::cerr << "Parsing failed!" << std::endl;
	}

	return EXIT_SUCCESS;
}
