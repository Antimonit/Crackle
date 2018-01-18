#include <cctype>
#include <fstream>
#include <cassert>
#include <c++/iostream>
#include "parser.tab.hh"
#include "driver.hpp"

MC::Driver::~Driver() {
	delete scanner;
	scanner = nullptr;
	delete parser;
	parser = nullptr;
}

void MC::Driver::parse(const char *const filename) {
	assert(filename != nullptr);
	std::ifstream in_file(filename);
	if (!in_file.good()) {
		exit(EXIT_FAILURE);
	}
	parse_helper(in_file);
}

void MC::Driver::parse(std::istream &stream) {
	if (!stream.good() && stream.eof()) {
		return;
	}
	parse_helper(stream);
}


void MC::Driver::parse_helper(std::istream &stream) {
	delete scanner;
	try {
		scanner = new MC::Scanner(&stream);
	} catch (std::bad_alloc &ba) {
		std::cerr << "Failed to allocate scanner: (" << ba.what() << "), exiting!!\n";
		exit(EXIT_FAILURE);
	}

	delete (parser);
	try {
		parser = new MC::Parser((*scanner) /* scanner */,
								   (*this) /* driver */ );
	} catch (std::bad_alloc &ba) {
		std::cerr << "Failed to allocate parser: (" << ba.what() << "), exiting!!\n";
		exit(EXIT_FAILURE);
	}

	const int accept(0);
	if (parser->parse() != accept) {
		std::cerr << "Parse failed!!\n";
	}
}

void MC::Driver::add_upper() {
	uppercase++;
	chars++;
	words++;
}

void MC::Driver::add_lower() {
	lowercase++;
	chars++;
	words++;
}

void MC::Driver::add_word(const std::string &word) {
	words++;
	chars += word.length();
	for (const char &c : word) {
		if (islower(c)) {
			lowercase++;
		} else if (isupper(c)) {
			uppercase++;
		}
	}
}

void MC::Driver::add_newline() {
	lines++;
	chars++;
}

void MC::Driver::add_char() {
	chars++;
}


std::ostream& MC::Driver::print(std::ostream &stream) {
	stream << "Results: " << "\n";
	stream << "Uppercase: " << uppercase << "\n";
	stream << "Lowercase: " << lowercase << "\n";
	stream << "Lines: " << lines << "\n";
	stream << "Words: " << words << "\n";
	stream << "Characters: " << chars << "\n";
	return (stream);
}
