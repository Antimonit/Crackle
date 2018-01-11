cmake_minimum_required(VERSION 3.8)
project(crackle_interpreter)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(BISON)
find_package(FLEX)

bison_target(Parser ${CMAKE_CURRENT_SOURCE_DIR}/parser.y ${CMAKE_CURRENT_BINARY_DIR}/y.tab.c)
flex_target(Scanner ${CMAKE_CURRENT_SOURCE_DIR}/lexer.l  ${CMAKE_CURRENT_BINARY_DIR}/lexer.c)
add_flex_bison_dependency(Scanner Parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(Crackle
        headers/debug.h
        headers/interpreter.h
        sources/interpreter.c
        headers/symbol_table.hpp
        sources/symbol_table.cpp
        headers/types.hpp
        sources/types.cpp
        headers/ast_printer.pph
        sources/ast_printer.cpp
        lexer.l
        parser.y
        parser.h
        ${FLEX_Scanner_OUTPUTS}
        ${BISON_Parser_OUTPUTS}
        headers/node_helpers.hpp
        sources/node_helpers.cpp
        )

# original makefile:

#all: y.tab.c y.tab.h lex.yy.c
#$(info ************  GCC  ************)
#gcc lex.yy.c y.tab.c symbols.c -o homework.exe
#
#y.tab.c: homework.y
#$(info ************ BISON ************)
#bison -dy homework.y -o y.tab.c
#
#lex.yy.c: homework.l
#$(info ************ FLEX  ************)
#flex homework.l
#
#bison:
#$(info ************ BISON ************)
#bison -dyv homework.y -o y.tab.c
