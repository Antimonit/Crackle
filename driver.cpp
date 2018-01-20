#include <cctype>
#include <fstream>
#include <cassert>
#include <c++/iostream>
#include "parser.tab.hh"
#include "driver.hpp"

using token = MC::Parser::token;

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

//void MC::Driver::add_upper() {
//	uppercase++;
//	chars++;
//	words++;
//}
//
//void MC::Driver::add_lower() {
//	lowercase++;
//	chars++;
//	words++;
//}
//
//void MC::Driver::add_word(const std::string &word) {
//	words++;
//	chars += word.length();
//	for (const char &c : word) {
//		if (islower(c)) {
//			lowercase++;
//		} else if (isupper(c)) {
//			uppercase++;
//		}
//	}
//}
//
//void MC::Driver::add_newline() {
//	lines++;
//	chars++;
//}
//
//void MC::Driver::add_char() {
//	chars++;
//}

void MC::Driver::returnx(Node* p, Node* result) {
	Node* value = ex(p->oper.op[0]);

	result->type = typeReturn;
	result->ret.value = value->constant;
}

void MC::Driver::whilex(Node* p, Node* result) {
	while (ex(p->oper.op[0])->constant.intVal) {
		pushSymbolTableScope();
		Node* left = ex(p->oper.op[1]);
		if (left->type == typeReturn) {
			result->type = typeReturn;
			result->ret.value = left->ret.value;
			popSymbolTableScope();
			return;
		}
		popSymbolTableScope();
	}

	result->type = typeOperator;
	result->oper.oper = token::WHILE;
}

void MC::Driver::forx(Node* p, Node* result) {
	for (ex(p->oper.op[0]);
		 ex(p->oper.op[1])->constant.boolVal;
		 ex(p->oper.op[2])) {

		pushSymbolTableScope();
		Node* left = ex(p->oper.op[3]);
		if (left->type == typeReturn) {
			popSymbolTableScope();
			result->type = typeReturn;
			result->ret.value = left->ret.value;
			return;
		}
		popSymbolTableScope();
	}
	result->type = typeOperator;
	result->oper.oper = token::FOR;
}

void MC::Driver::ifx(Node* p, Node* result) {
	pushSymbolTableScope();
	if (ex(p->oper.op[0])->constant.intVal) {
		Node* left = ex(p->oper.op[1]);
		if (left->type == typeReturn) {
			result->type = left->type;
			result->ret.value = left->ret.value;
			popSymbolTableScope();
			return;
		}
	} else if (p->oper.opCount > 2) {
		Node* right = ex(p->oper.op[2]);
		if (right->type == typeReturn) {
			result->type = right->type;
			result->ret.value = right->ret.value;
			popSymbolTableScope();
			return;
		}
	}

	result->type = typeOperator;
	result->oper.oper = token::IF;
	popSymbolTableScope();
}

void MC::Driver::delimiter(Node* p, Node* result) {
	result->type = typeOperator;
	result->oper.oper = ';';

	if (p->oper.opCount > 0) {
		Node* left = ex(p->oper.op[0]);
		if (left->type == typeReturn) {
			result->type = typeReturn;
			result->ret.value = left->ret.value;
			return;
		}
	}
	if (p->oper.opCount > 1) {
		Node* right = ex(p->oper.op[1]);
		if (right->type == typeReturn) {
			result->type = typeReturn;
			result->ret.value = right->ret.value;
			return;
		}
	}
}

void MC::Driver::assign(Node* p, Node* result) {
	Node* one = p->oper.op[0];
	VariableNode variableNode = one->variable;
	Node* value = ex(p->oper.op[1]);

	ConstantNode* variable = findVariable(variableNode.name);

	if (variable == nullptr) {
		std::cerr << "Warning: Unable to assign undefined variable '" << variableNode.name
				  << "." << std::endl;

		result->type = typeEmpty;
		return;
	}

	if (variable->dataType != value->constant.dataType) {
		std::cerr << "Warning: Incompatible assignment of type '" << dataTypeToString(value->constant.dataType)
				  << "' to variable '" << variableNode.name
				  << "' of type '" << dataTypeToString(variable->dataType)
				  << "'." << std::endl;
		result->type = typeEmpty;
		return;
	}

	dataTypeEnum type = variable->dataType;

	result->type = typeConstant;
	result->constant.dataType = type;
	result->constant = *variable = value->constant;
}

void MC::Driver::uminus(Node* p, Node* result) {
	Node* a = ex(p->oper.op[0]);
	result->type = typeConstant;
	result->constant.dataType = a->constant.dataType;
	if (result->constant.dataType == typeInt) {
		result->constant.intVal = -a->constant.intVal;
	} else if (result->constant.dataType == typeDouble) {
		result->constant.doubleVal = -a->constant.doubleVal;
	} else {
		std::cerr << "Warning: Invalid argument to unary minus." << std::endl;
	}
}

void MC::Driver::plus(Node* p, Node* result) {
	ConstantNode* left = &ex(p->oper.op[0])->constant;
	ConstantNode* right = &ex(p->oper.op[1])->constant;
	result->type = typeConstant;
	result->constant.dataType = widenNodes(left, right);
	if (result->constant.dataType == typeInt) {
		result->constant.intVal = left->intVal + right->intVal;
	} else if (result->constant.dataType == typeDouble) {
		result->constant.doubleVal = left->doubleVal + right->doubleVal;
	} else {
		std::cerr << "Warning: Invalid argument to plus operator." << std::endl;
	}
}
void MC::Driver::minus(Node* p, Node* result) {
	ConstantNode* left = &ex(p->oper.op[0])->constant;
	ConstantNode* right = &ex(p->oper.op[1])->constant;
	result->type = typeConstant;
	result->constant.dataType = widenNodes(left, right);
	if (result->constant.dataType == typeInt) {
		result->constant.intVal = left->intVal - right->intVal;
	} else if (result->constant.dataType == typeDouble) {
		result->constant.doubleVal = left->doubleVal - right->doubleVal;
	} else {
		std::cerr << "Warning: Invalid argument to minus operator.\n" << std::endl;
	}
}
void MC::Driver::multiply(Node* p, Node* result) {
	ConstantNode* left = &ex(p->oper.op[0])->constant;
	ConstantNode* right = &ex(p->oper.op[1])->constant;
	result->type = typeConstant;
	result->constant.dataType = widenNodes(left, right);
	if (result->constant.dataType == typeInt) {
		result->constant.intVal = left->intVal * right->intVal;
	} else if (result->constant.dataType == typeDouble) {
		result->constant.doubleVal = left->doubleVal * right->doubleVal;
	} else {
		std::cerr << "Warning: Invalid argument to multiply operator.\n" << std::endl;
	}
}
void MC::Driver::divide(Node* p, Node* result) {
	ConstantNode* left = &ex(p->oper.op[0])->constant;
	ConstantNode* right = &ex(p->oper.op[1])->constant;
	result->type = typeConstant;
	result->constant.dataType = widenNodes(left, right);
	if (result->constant.dataType == typeInt) {
		result->constant.intVal = left->intVal / right->intVal;
	} else if (result->constant.dataType == typeDouble) {
		result->constant.doubleVal = left->doubleVal / right->doubleVal;
	} else {
		std::cerr << "Warning: Invalid argument to divide operator.\n" << std::endl;
	}
}
void MC::Driver::modulo(Node* p, Node* result) {
	ConstantNode* left = &ex(p->oper.op[0])->constant;
	ConstantNode* right = &ex(p->oper.op[1])->constant;
	result->type = typeConstant;
	result->constant.dataType = widenNodes(left, right);
	if (result->constant.dataType == typeInt) {
		result->constant.intVal = left->intVal % right->intVal;
	} else {
		std::cerr << "Warning: Invalid argument to modulo operator.\n" << std::endl;
	}
}

void MC::Driver::andx(Node* p, Node* result) {
	Node* leftNode = ex(p->oper.op[0]);
	Node* rightNode = ex(p->oper.op[1]);
	ConstantNode* left = &leftNode->constant;
	ConstantNode* right = &rightNode->constant;
	result->type = typeConstant;
	result->constant.dataType = typeUndefined;

	if (left->dataType != typeBool) {
		std::cerr << "Warning: Invalid argument of type " << dataTypeToString(left->dataType)
				  << " to AND operator." << std::endl;
	}
	if (right->dataType != typeBool) {
		std::cerr << "Warning: Invalid argument of type " << dataTypeToString(right->dataType)
				  << " to AND operator." << std::endl;
	}
	if (left->dataType == typeBool && right->dataType == typeBool) {
		result->constant.dataType = typeBool;
		result->constant.boolVal = left->boolVal && right->boolVal;
	}
}
void MC::Driver::orx(Node* p, Node* result) {
	ConstantNode* left = &ex(p->oper.op[0])->constant;
	ConstantNode* right = &ex(p->oper.op[1])->constant;
	result->type = typeConstant;
	result->constant.dataType = typeUndefined;

	if (left->dataType != typeBool) {
		std::cerr << "Warning: Invalid argument of type " << dataTypeToString(left->dataType)
				  << " to OR operator." << std::endl;
	}
	if (right->dataType != typeBool) {
		std::cerr << "Warning: Invalid argument of type " << dataTypeToString(right->dataType)
				  << " to OR operator." << std::endl;
	}
	if (left->dataType == typeBool && right->dataType == typeBool) {
		result->constant.dataType = typeBool;
		result->constant.boolVal = left->boolVal || right->boolVal;
	}
}
void MC::Driver::neg(Node* p, Node* result) {
	ConstantNode* value = &ex(p->oper.op[0])->constant;
	result->type = typeConstant;
	result->constant.dataType = typeUndefined;

	if (value->dataType != typeBool) {
		std::cerr << "Warning: Invalid argument of type " << dataTypeToString(value->dataType)
				  << " to NEG operator." << std::endl;
	}
	if (value->dataType == typeBool) {
		result->constant.dataType = typeBool;
		result->constant.boolVal = !value->boolVal;
	}
}

void MC::Driver::lt(Node* p, Node* result) {
	Node* left = ex(p->oper.op[0]);
	Node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = typeBool;
	result->constant.boolVal = left->constant.intVal < right->constant.intVal;
}
void MC::Driver::le(Node* p, Node* result) {
	Node* left = ex(p->oper.op[0]);
	Node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = typeBool;
	result->constant.boolVal = left->constant.intVal <= right->constant.intVal;
}
void MC::Driver::gt(Node* p, Node* result) {
	Node* left = ex(p->oper.op[0]);
	Node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = typeBool;
	result->constant.boolVal = left->constant.intVal > right->constant.intVal;
}
void MC::Driver::ge(Node* p, Node* result) {
	Node* left = ex(p->oper.op[0]);
	Node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = typeBool;
	result->constant.boolVal = left->constant.intVal >= right->constant.intVal;
}
void MC::Driver::eq(Node* p, Node* result) {
	Node* left = ex(p->oper.op[0]);
	Node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = typeBool;
	result->constant.boolVal = left->constant.intVal == right->constant.intVal;
}
void MC::Driver::ne(Node* p, Node* result) {
	Node* left = ex(p->oper.op[0]);
	Node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = typeBool;
	result->constant.boolVal = left->constant.intVal != right->constant.intVal;
}

void MC::Driver::printx(Node* p, Node* result) {
	Node* value = ex(p->oper.op[0]);

	switch (value->type) {
		case typeOperator:
			switch (value->oper.oper) {
				case token::FOR: 	std::cout << "FOR";		break;
				case token::WHILE:	std::cout << "WHILE";	break;
				case token::IF:		std::cout << "IF";		break;
				case token::FUN:	std::cout << "FUN";		break;
				case token::VAR:	std::cout << "VAR";		break;
				default:			std::cout << "WRONG OPERATOR";
			}
			break;
		case typeConstant:
			std::cout << constantValueToString(value->constant);
			break;
		case typeFunctionDef:
			std::cout << "Function " << value->functionDef.name << " defined";
			break;
		case typeEmpty:
			std::cout << "EMPTY";
			break;
		default:
			std::cout << "WRONG TYPE";
	}

	result->type = typeEmpty;
}
void MC::Driver::println(Node* p, Node* result) {
	printx(p, result);
	std::cout << std::endl;
}

Node* MC::Driver::ex(Node* p) {
//	auto* result = newNode();
	auto* result = (Node*) malloc(10*sizeof(Node));;
	result->type = typeEmpty;

	if (!p)
		return result;

	enterNode(p);

	switch (p->type) {
		case typeConstant: {
			result->type = typeConstant;
			result->constant = p->constant;
			break;
		}
		case typeVariableDef: {
			VariableDefNode *variableDef = &p->variableDef;

			if (variableDef->defaultValue != nullptr) {
				Node* defaultValue = ex(variableDef->defaultValue);
				if (variableDef->value.dataType != defaultValue->constant.dataType) {
					std::cerr << "Warning: Defined value of type " << dataTypeToString(variableDef->value.dataType)
							  << " is incompatible with variable type " << dataTypeToString(defaultValue->constant.dataType)
							  << "." << std::endl;
				} else {
					variableDef->value = defaultValue->constant;
				}
			} else {
				defaultConstant(&variableDef->value);
			}

			addVariable(variableDef);
			break;
		}
		case typeVariable: {
			result->type = typeConstant;

			std::string name = p->variable.name;
			ConstantNode* variable = findVariable(name);

			if (variable == nullptr) {
				std::cerr << "Warning: Undefined variable '" << name << "'." << std::endl;
				result->type = typeEmpty;
				exitNode(result);
				return result;
			}

			result->constant = *variable;
			break;
		}
		case typeOperator: {
			switch (p->oper.oper) {
				case token::PRINT:		printx(p, result); break;
				case token::PRINTLN:	println(p, result); break;
				case token::RETURN:		returnx(p, result); break;

				case token::WHILE:	whilex(p, result); break;
				case token::IF:		ifx(p, result); break;
				case token::FOR:	forx(p, result); break;
				case ';':			delimiter(p, result); break;
				case '=':			assign(p, result); break;

				case token::UMINUS:	uminus(p, result); break;

				case '+':			plus(p, result); break;
				case '-':			minus(p, result); break;
				case '*':			multiply(p, result); break;
				case '/':			divide(p, result); break;
				case '%':			modulo(p, result); break;

				case token::AND:	andx(p, result); break;
				case token::OR:		orx(p, result); break;
				case token::NEG:	neg(p, result); break;

				case token::LT:		lt(p, result); break;
				case token::LE:		le(p, result); break;
				case token::GT:		gt(p, result); break;
				case token::GE:		ge(p, result); break;
				case token::EQ:		eq(p, result); break;
				case token::NE:		ne(p, result); break;
				default:
					std::cerr << "Error: unknown operator '" << p->oper.oper << "'." << std::endl;
			}
			break;
		}
		case typeFunctionDef: {
			addFunction(&p->functionDef);
			break;
		}
		case typeFunction: {
			std::string name = p->function.name;

			FunctionDefNode* function = findFunction(name);
			if (function == nullptr) {
				std::cerr << "Warning: Undefined function '" << name << "'." << std::endl;
				exitNode(result);
				return result;
			}

			for (int i = 0; i < function->paramCount; ++i) {
				VariableDefNode* paramDef = &function->params[i];
				ConstantNode* paramVar = &ex(p->function.params[i])->constant;
				if (paramDef->value.dataType == paramVar->dataType) {
					paramDef->value = *paramVar;
				} else {
					defaultConstant(&paramDef->value);
					std::cerr << "Warning: Passing incompatible parameter of type " << dataTypeToString(paramVar->dataType)
							  << " instead of type " << dataTypeToString(paramDef->value.dataType)
							  << "." << std::endl;
				}
			}

			replaceSymbolTableScope();
			for (int i = 0; i < function->paramCount; ++i) {
				addVariable(&function->params[i]);
			}
			Node* res = ex(function->root);
			popSymbolTableScope();

			result->type = typeConstant;
			result->constant = res->constant;
			break;
		}
		case typeObjectDef: {
			addObject(&p->objectDef);
			break;
		}
		case typeObject: {
			std::string name = p->object.name;

			ObjectDefNode *object = findObject(name);
			if (object == nullptr) {
				std::cerr << "Warning: Undefined object '" << name
						  << "'." << std::endl;
				result->type = typeEmpty;
				exitNode(result);
				return result;
			}

			result->type = typeConstant;
			result->constant.dataType = typeObj;
			result->constant.objectTypeName = name;
			result->constant.objectVal = (ObjectDefNode*) malloc(object->varCount * sizeof(ConstantNode*));

			ObjectDefNode* objectVal = result->constant.objectVal;
//			ConstantNode** objectVal = result->constant.objectVal;
			for (int i = 0; i < object->varCount; ++i) {
//				VariableDefNode* paramDef = &object->vars[i];
//				ConstantNode *ptr = &p->object.vars[i];
//				objectVal[i] = ptr;
//				if (paramDef->value.dataType == paramVar->dataType) {
//					paramDef->value = *paramVar;
//				} else {
//					defaultConstant(&paramDef->value);
//					printf("Warning: Passing incompatible parameter of type %s instead of type %s\n",
//						   dataTypeToString(paramVar->dataType), dataTypeToString(paramDef->value.dataType));
//				}
			}
			result->constant.objectVal = objectVal;

			break;
		}
		case typeEmpty:
			break;
		default:
			break;
	}

	exitNode(result);
	return result;
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
