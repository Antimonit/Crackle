set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-deprecated-register")

project(simple_wc_example CXX)

cmake_minimum_required(VERSION 3.1)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(parser
        parser.yy
        ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.cc)
FLEX_TARGET(lexer
        scanner.ll
        ${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.cc)
ADD_FLEX_BISON_DEPENDENCY(lexer parser)

add_executable(my_wc
        main.cpp
        driver.cpp
        ${FLEX_lexer_OUTPUTS}
        ${BISON_parser_OUTPUTS}
)

target_include_directories(my_wc
        PRIVATE
        .
        ${CMAKE_CURRENT_BINARY_DIR})

set_property(TARGET my_wc
             PROPERTY CXX_STANDARD 14)
