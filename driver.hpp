#ifndef __MCDRIVER_HPP__
#define __MCDRIVER_HPP__ 1

#include <string>
#include <cstddef>
#include <istream>
#include <cmake-build-debug/parser.tab.hh>

#include "scanner.hpp"
#include "parser.tab.hh"

namespace MC {

	class Driver {
	public:
		Driver() = default;

		virtual ~Driver();

		void parse(const char* filename);

		void parse(std::istream& is);

		void add_upper();

		void add_lower();

		void add_word(const std::string &word);

		void add_newline();

		void add_char();

		std::ostream &print(std::ostream &stream);

	private:

		void parse_helper(std::istream &stream);

		std::size_t chars = 0;
		std::size_t words = 0;
		std::size_t lines = 0;
		std::size_t uppercase = 0;
		std::size_t lowercase = 0;
		MC::Parser *parser = nullptr;
		MC::Scanner *scanner = nullptr;
	};

} /* end namespace MC */
#endif /* END __MCDRIVER_HPP__ */
