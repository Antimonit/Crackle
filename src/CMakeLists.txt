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

add_library(crackle_lib
        scanner.hpp
        parser.hpp
        Driver.cpp
        sources/SymbolTable.cpp
        headers/SymbolTable.hpp
        sources/node_helpers.cpp
        headers/node_helpers.hpp
        sources/types.cpp
        headers/types.hpp
        sources/AstPrinter.cpp
        headers/AstPrinter.hpp
        sources/Heap.cpp
        headers/Heap.hpp
        ${FLEX_lexer_OUTPUTS}
        ${BISON_parser_OUTPUTS}
        )
target_include_directories(crackle_lib PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(crackle_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#set_property(TARGET crackle_lib PROPERTY CXX_STANDARD 17)
