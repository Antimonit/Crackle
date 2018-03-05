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
        sources/node_types.cpp
        headers/types.hpp
        sources/AstPrinter.cpp
        headers/AstPrinter.hpp
        sources/Heap.cpp
        headers/Heap.hpp
        ${FLEX_lexer_OUTPUTS}
        ${BISON_parser_OUTPUTS}
        nodes/node_types.h
        nodes/DataType.h            nodes/DataType.cpp
        nodes/xNode.cpp             nodes/xNode.h
        nodes/xConstantNode.cpp     nodes/xConstantNode.h
        nodes/xVariableDefNode.cpp  nodes/xVariableDefNode.h
        nodes/xVariableNode.cpp     nodes/xVariableNode.h
        nodes/xFunctionDefNode.cpp  nodes/xFunctionDefNode.h
        nodes/xFunctionNode.cpp     nodes/xFunctionNode.h
        nodes/xObjectDefNode.cpp    nodes/xObjectDefNode.h
        nodes/xObjectNode.cpp       nodes/xObjectNode.h
        nodes/xOperatorNode.cpp     nodes/xOperatorNode.h
        nodes/xReturnNode.cpp       nodes/xReturnNode.h
        nodes/xEmptyNode.cpp        nodes/xEmptyNode.h
        nodes/constant/xConstantIntNode.cpp nodes/constant/xConstantIntNode.h nodes/constant/xConstantObjectNode.cpp nodes/constant/xConstantObjectNode.h nodes/constant/xConstantVoidNode.cpp nodes/constant/xConstantVoidNode.h nodes/constant/xConstantDoubleNode.cpp nodes/constant/xConstantDoubleNode.h nodes/constant/xConstantStringNode.cpp nodes/constant/xConstantStringNode.h nodes/constant/xConstantBoolNode.cpp nodes/constant/xConstantBoolNode.h)
target_include_directories(crackle_lib PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(crackle_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#set_property(TARGET crackle_lib PROPERTY CXX_STANDARD 17)
