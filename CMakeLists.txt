cmake_minimum_required(VERSION 3.8)
project(crackle_interpreter CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17")

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(parser
        parser.yy
        ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.cc
#        VERBOSE ${CMAKE_CURRENT_BINARY_DIR}/parser.output
#        COMPILE_FLAGS --debug
        )
FLEX_TARGET(lexer
        scanner.ll
        ${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.cc)
ADD_FLEX_BISON_DEPENDENCY(lexer parser)

add_executable(Crackle
        scanner.hpp
        main.cpp
        Driver.cpp
        parser.hpp
        sources/SymbolTable.cpp
        headers/SymbolTable.hpp
        sources/node_helpers.cpp
        headers/node_helpers.hpp
        sources/types.cpp
        headers/types.hpp
        sources/AstPrinter.cpp
        headers/AstPrinter.hpp
        ${FLEX_lexer_OUTPUTS}
        ${BISON_parser_OUTPUTS}
        sources/Heap.cpp headers/Heap.hpp)

target_include_directories(Crackle
        PRIVATE
        .
        ${CMAKE_CURRENT_BINARY_DIR})

set_property(TARGET Crackle
             PROPERTY CXX_STANDARD 17)
