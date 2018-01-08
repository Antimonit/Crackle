cmake_minimum_required(VERSION 3.8)
project(crackle_interpreter)

set(CMAKE_C_STANDARD 11)

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
        headers/symbol_table.h
        sources/symbol_table.c
        headers/function_symbol_table.h
        sources/function_symbol_table.c
        headers/types.h
        sources/types.cpp
        ast_printer.h
        ast_printer.cpp
        lexer.l
        parser.y
        parser.h
        ${FLEX_Scanner_OUTPUTS}
        ${BISON_Parser_OUTPUTS}
        headers/node_helpers.h
        sources/node_helpers.c
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
