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
        debug.h
        interpreter.h
        symbol_table.h symbol_table.c
        function_symbol_table.h function_symbol_table.c
        types.h types.c
        lexer.l
        parser.y
        ${FLEX_Scanner_OUTPUTS}
        ${BISON_Parser_OUTPUTS}
        cmake-build-debug/node_helpers.h
        parser.h)
