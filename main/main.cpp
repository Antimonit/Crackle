#include <iostream>
#include <cstring>

#include <Driver.hpp>

int main(const int argc, const char **argv) {

	MC::Driver::Builder builder;

	for (int i = 1; i < argc; i++) {
		if (std::strncmp(argv[i], "-i", 2) == 0) {
			i++;
			if (i < argc) {
				builder.input(std::string(argv[i]));
			} else {
				std::cout << "Unspecified input file name." << std::endl;
				return (EXIT_FAILURE);
			}
		} else if (std::strncmp(argv[i], "-o", 2) == 0) {
			i++;
			if (i < argc) {
				builder.output(std::string(argv[i]));
			} else {
				std::cout << "Unspecified output file name." << std::endl;
				return (EXIT_FAILURE);
			}
		} else if (std::strncmp(argv[i], "-d", 2) == 0) {
			i++;
			if (i < argc) {
				builder.debug(std::string(argv[i]));
			} else {
				std::cout << "Unspecified debug file name." << std::endl;
				return (EXIT_FAILURE);
			}
		} else if (std::strncmp(argv[i], "-f", 2) == 0) {
			builder.debug(std::cout);
		} else if (std::strncmp(argv[i], "--heap", 6) == 0) {
			i++;
			if (i < argc) {
				builder.heap(atoi(argv[i]));
			} else {
				std::cout << "Unspecified heap size." << std::endl;
				return (EXIT_FAILURE);
			}
		} else if (std::strncmp(argv[i], "-h", 2) == 0) {
			std::cout << "Use -i <path_to_file> to specify input file (defaults to standard input)" << std::endl;
			std::cout << "Use -o <path_to_file> to specify output file (defaults to standard output)" << std::endl;
			std::cout << "Use -d <path_to_file> to specify debug file (none by default)" << std::endl;
			std::cout << "Use -f to print debug to standard output" << std::endl;
			std::cout << "Use --heap <heap_size> to set the heap size" << std::endl;
			std::cout << "Use -h to get help." << std::endl;
			return (EXIT_SUCCESS);
		} else {
			std::cout << "Invalid arguments" << std::endl;
			return (EXIT_FAILURE);
		}
	}

	MC::Driver driver = builder.build();

	const int accept(0);
	if (driver.parse() == accept) {
		std::cout << "Parsing successful" << std::endl;
	} else {
		std::cerr << "Parsing failed!" << std::endl;
	}

	return EXIT_SUCCESS;
}
