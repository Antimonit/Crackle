cmake_minimum_required(VERSION 3.1)
project(pop_interpreter CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(parser
        parser.yy
        ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.cc)
FLEX_TARGET(lexer
        scanner.ll
        ${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.cc)
ADD_FLEX_BISON_DEPENDENCY(lexer parser)

add_executable(Pop
        scanner.hpp
        main.cpp
        driver.cpp
        parser.hpp
        sources/interpreter.cpp
        headers/interpreter.hpp
        sources/symbol_table.cpp
        headers/symbol_table.hpp
        sources/node_helpers.cpp
        headers/node_helpers.hpp
        sources/types.cpp
        headers/types.hpp
        sources/ast_printer.cpp
        headers/ast_printer.hpp
        ${FLEX_lexer_OUTPUTS}
        ${BISON_parser_OUTPUTS}
)

target_include_directories(Pop
        PRIVATE
        .
        ${CMAKE_CURRENT_BINARY_DIR})

set_property(TARGET Pop
             PROPERTY CXX_STANDARD 14)
