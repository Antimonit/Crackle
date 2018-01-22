#include <cctype>
#include <fstream>
#include <cassert>
#include <c++/iostream>
#include "parser.tab.hh"
#include "driver.hpp"

using token = MC::Parser::token;

MC::Driver::Driver() {
	in = &std::cin;
	out = &std::cout;
	deb = nullptr;
}

MC::Driver::~Driver() {
	delete scanner;
	scanner = nullptr;
	delete parser;
	parser = nullptr;
	delete printer;
	printer = nullptr;
	if (in != nullptr && in != &std::cin) {
		delete in;
		in = nullptr;
	}
	if (out != nullptr && out != &std::cout && out != &std::cerr) {
		delete out;
		out = nullptr;
	}
	if (deb != nullptr && deb != &std::cout && deb != &std::cerr) {
		delete deb;
		deb = nullptr;
	}
}

void MC::Driver::input(std::string filename) {
	auto* file = new std::ifstream(filename);
	if (!file->good()) {
		exit(EXIT_FAILURE);
	}
	in = file;
}

void MC::Driver::input(std::istream& is) {
	if (!is.good() && is.eof()) {
		return;
	}
	in = &is;
}

void MC::Driver::output(std::string filename) {
	auto* file = new std::ofstream(filename);
	if (!file->good()) {
		exit(EXIT_FAILURE);
	}
	out = file;
}

void MC::Driver::output(std::ostream& os) {
	if (!os.good() && os.eof()) {
		return;
	}
	out = &os;
}

void MC::Driver::debug(std::string filename) {
	auto* file = new std::ofstream(filename);
	if (!file->good()) {
		exit(EXIT_FAILURE);
	}
	deb = file;
}

void MC::Driver::debug(std::ostream& os) {
	if (!os.good() && os.eof()) {
		return;
	}
	deb = &os;
}

int MC::Driver::parse() {
	if (!in->good() && in->eof()) {
		return 1;
	}
	if (!out->good() && out->eof()) {
		return 1;
	}

	delete scanner;
	try {
		scanner = new MC::Scanner(in);
	} catch (std::bad_alloc &ba) {
		std::cerr << "Failed to allocate scanner: (" << ba.what() << "), exiting!!\n";
		exit(EXIT_FAILURE);
	}

	delete parser;
	try {
		parser = new MC::Parser((*scanner) /* scanner */,
								(*this) /* driver */ );
	} catch (std::bad_alloc &ba) {
		std::cerr << "Failed to allocate parser: (" << ba.what() << "), exiting!!\n";
		exit(EXIT_FAILURE);
	}

	delete printer;
	if (deb != nullptr) {
		try {
			printer = new MC::AstPrinter(deb);
		} catch (std::bad_alloc &ba) {
			std::cerr << "Failed to allocate printer: (" << ba.what() << "), exiting!!\n";
			exit(EXIT_FAILURE);
		}
	} else {
		printer = nullptr;
	}

	return parser->parse();
}



void MC::Driver::returnx(Node* p, Node* result) {
	Node* value = ex(p->oper.op[0]);

	result->setType(typeReturn);
	result->ret.value = value->constant;
}

void MC::Driver::whilex(Node* p, Node* result) {
	while (ex(p->oper.op[0])->constant.boolVal) {
		pushSymbolTableScope();
		Node* left = ex(p->oper.op[1]);
		if (left->getType() == typeReturn) {
			result->setType(typeReturn);
			result->ret.value = left->ret.value;
			popSymbolTableScope();
			return;
		}
		popSymbolTableScope();
	}

	result->setType(typeOperator);
	result->oper.oper = token::WHILE;
}

void MC::Driver::forx(Node* p, Node* result) {
	for (ex(p->oper.op[0]);
		 ex(p->oper.op[1])->constant.boolVal;
		 ex(p->oper.op[2])) {

		pushSymbolTableScope();
		Node* left = ex(p->oper.op[3]);
		if (left->getType() == typeReturn) {
			popSymbolTableScope();
			result->setType(typeReturn);
			result->ret.value = left->ret.value;
			return;
		}
		popSymbolTableScope();
	}
	result->setType(typeOperator);
	result->oper.oper = token::FOR;
}

void MC::Driver::ifx(Node* p, Node* result) {
	pushSymbolTableScope();
	if (ex(p->oper.op[0])->constant.boolVal) {
		Node* left = ex(p->oper.op[1]);
		if (left->getType() == typeReturn) {
			result->setType(left->getType());
			result->ret.value = left->ret.value;
			popSymbolTableScope();
			return;
		}
	} else if (p->oper.op.size() > 2) {
		Node* right = ex(p->oper.op[2]);
		if (right->getType() == typeReturn) {
			result->setType(right->getType());
			result->ret.value = right->ret.value;
			popSymbolTableScope();
			return;
		}
	}

	result->setType(typeOperator);
	result->oper.oper = token::IF;
	popSymbolTableScope();
}

void MC::Driver::delimiter(Node* p, Node* result) {
	result->setType(typeOperator);
	result->oper.oper = ';';

	if (p->oper.op.size() > 0) {
		Node* left = ex(p->oper.op[0]);
		if (left->getType() == typeReturn) {
			result->setType(typeReturn);
			result->ret.value = left->ret.value;
			return;
		}
	}
	if (p->oper.op.size() > 1) {
		Node* right = ex(p->oper.op[1]);
		if (right->getType() == typeReturn) {
			result->setType(typeReturn);
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

		result->setType(typeEmpty);
		return;
	}

	if (variable->getType() != value->constant.getType()) {
		std::cerr << "Warning: Incompatible assignment of type '" << value->constant.getType()
				  << "' to variable '" << variableNode.name
				  << "' of type '" << variable->getType()
				  << "'." << std::endl;
		result->setType(typeEmpty);
		return;
	}

	DataType type = variable->getType();

	result->setType(typeConstant);
	result->constant.setType(type);
	result->constant = *variable = value->constant;
}

void MC::Driver::uminus(Node* p, Node* result) {
	Node* a = ex(p->oper.op[0]);
	result->setType(typeConstant);
	result->constant.setType(a->constant.getType());
	if (result->constant.getType() == typeInt) {
		result->constant.intVal = -a->constant.intVal;
	} else if (result->constant.getType() == typeDouble) {
		result->constant.doubleVal = -a->constant.doubleVal;
	} else {
		std::cerr << "Warning: Invalid argument to unary minus." << std::endl;
	}
}

void MC::Driver::plus(Node* p, Node* result) {
	ConstantNode* left = &ex(p->oper.op[0])->constant;
	ConstantNode* right = &ex(p->oper.op[1])->constant;
	result->setType(typeConstant);
	result->constant.setType(widenNodes(*left, *right));
	if (result->constant.getType() == typeInt) {
		result->constant.intVal = left->intVal + right->intVal;
	} else if (result->constant.getType() == typeDouble) {
		result->constant.doubleVal = left->doubleVal + right->doubleVal;
	} else {
		std::cerr << "Warning: Invalid argument to plus operator." << std::endl;
	}
}
void MC::Driver::minus(Node* p, Node* result) {
	ConstantNode* left = &ex(p->oper.op[0])->constant;
	ConstantNode* right = &ex(p->oper.op[1])->constant;
	result->setType(typeConstant);
	result->constant.setType(widenNodes(*left, *right));
	if (result->constant.getType() == typeInt) {
		result->constant.intVal = left->intVal - right->intVal;
	} else if (result->constant.getType() == typeDouble) {
		result->constant.doubleVal = left->doubleVal - right->doubleVal;
	} else {
		std::cerr << "Warning: Invalid argument to minus operator.\n" << std::endl;
	}
}
void MC::Driver::multiply(Node* p, Node* result) {
	ConstantNode* left = &ex(p->oper.op[0])->constant;
	ConstantNode* right = &ex(p->oper.op[1])->constant;
	result->setType(typeConstant);
	result->constant.setType(widenNodes(*left, *right));
	if (result->constant.getType() == typeInt) {
		result->constant.intVal = left->intVal * right->intVal;
	} else if (result->constant.getType() == typeDouble) {
		result->constant.doubleVal = left->doubleVal * right->doubleVal;
	} else {
		std::cerr << "Warning: Invalid argument to multiply operator.\n" << std::endl;
	}
}
void MC::Driver::divide(Node* p, Node* result) {
	ConstantNode* left = &ex(p->oper.op[0])->constant;
	ConstantNode* right = &ex(p->oper.op[1])->constant;
	result->setType(typeConstant);
	result->constant.setType(widenNodes(*left, *right));
	if (result->constant.getType() == typeInt) {
		result->constant.intVal = left->intVal / right->intVal;
	} else if (result->constant.getType() == typeDouble) {
		result->constant.doubleVal = left->doubleVal / right->doubleVal;
	} else {
		std::cerr << "Warning: Invalid argument to divide operator.\n" << std::endl;
	}
}
void MC::Driver::modulo(Node* p, Node* result) {
	ConstantNode* left = &ex(p->oper.op[0])->constant;
	ConstantNode* right = &ex(p->oper.op[1])->constant;
	result->setType(typeConstant);
	result->constant.setType(widenNodes(*left, *right));
	if (result->constant.getType() == typeInt) {
		result->constant.intVal = left->intVal % right->intVal;
	} else {
		std::cerr << "Warning: Invalid argument to modulo operator.\n" << std::endl;
	}
}

void MC::Driver::andx(Node* p, Node* result) {
	ConstantNode* left = &ex(p->oper.op[0])->constant;;
	ConstantNode* right = &ex(p->oper.op[1])->constant;;
	result->setType(typeConstant);
	result->constant.setType(typeUndefined);

	if (left->getType() != typeBool) {
		std::cerr << "Warning: Invalid argument of type " << left->getType()
				  << " to AND operator." << std::endl;
	}
	if (right->getType() != typeBool) {
		std::cerr << "Warning: Invalid argument of type " << right->getType()
				  << " to AND operator." << std::endl;
	}
	if (left->getType() == typeBool && right->getType() == typeBool) {
		result->constant.setType(typeBool);
		result->constant.boolVal = left->boolVal && right->boolVal;
	}
}
void MC::Driver::orx(Node* p, Node* result) {
	ConstantNode* left = &ex(p->oper.op[0])->constant;
	ConstantNode* right = &ex(p->oper.op[1])->constant;
	result->setType(typeConstant);
	result->constant.setType(typeUndefined);

	if (left->getType() != typeBool) {
		std::cerr << "Warning: Invalid argument of type " << left->getType()
				  << " to OR operator." << std::endl;
	}
	if (right->getType() != typeBool) {
		std::cerr << "Warning: Invalid argument of type " << right->getType()
				  << " to OR operator." << std::endl;
	}
	if (left->getType() == typeBool && right->getType() == typeBool) {
		result->constant.setType(typeBool);
		result->constant.boolVal = left->boolVal || right->boolVal;
	}
}
void MC::Driver::neg(Node* p, Node* result) {
	ConstantNode* value = &ex(p->oper.op[0])->constant;
	result->setType(typeConstant);
	result->constant.setType(typeUndefined);


	if (value->getType() != typeBool) {
		std::cerr << "Warning: Invalid argument of type " << value->getType()
				  << " to NEG operator." << std::endl;
	}
	if (value->getType() == typeBool) {
		result->constant.setType(typeBool);
		result->constant.boolVal = !value->boolVal;
	}
}

void MC::Driver::lt(Node* p, Node* result) {
	ConstantNode* left = &ex(p->oper.op[0])->constant;;
	ConstantNode* right = &ex(p->oper.op[1])->constant;;
	result->setType(typeConstant);
	result->constant.setType(typeUndefined);

	DataType max = maxType(*left, *right);

	if (max == typeInt) {
		result->constant.setType(typeBool);
		widenNodes(*left, *right);
		result->constant.boolVal = left->intVal < right->intVal;
	} else if (max == typeDouble) {
		result->constant.setType(typeBool);
		widenNodes(*left, *right);
		result->constant.boolVal = left->doubleVal < right->doubleVal;
	} else {
		std::cerr << "Warning: Invalid arguments of types " << left->getType()
				  << " and " << right->getType()
				  << " to LT operator." << std::endl;
	}
}
void MC::Driver::le(Node* p, Node* result) {
	Node* left = ex(p->oper.op[0]);
	Node* right = ex(p->oper.op[1]);
	result->setType(typeConstant);
	result->constant.setType(typeBool);
	result->constant.boolVal = left->constant.intVal <= right->constant.intVal;
}
void MC::Driver::gt(Node* p, Node* result) {
	Node* left = ex(p->oper.op[0]);
	Node* right = ex(p->oper.op[1]);
	result->setType(typeConstant);
	result->constant.setType(typeBool);
	result->constant.boolVal = left->constant.intVal > right->constant.intVal;
}
void MC::Driver::ge(Node* p, Node* result) {
	Node* left = ex(p->oper.op[0]);
	Node* right = ex(p->oper.op[1]);
	result->setType(typeConstant);
	result->constant.setType(typeBool);
	result->constant.boolVal = left->constant.intVal >= right->constant.intVal;
}
void MC::Driver::eq(Node* p, Node* result) {
	Node* left = ex(p->oper.op[0]);
	Node* right = ex(p->oper.op[1]);
	result->setType(typeConstant);
	result->constant.setType(typeBool);
	result->constant.boolVal = left->constant.intVal == right->constant.intVal;
}
void MC::Driver::ne(Node* p, Node* result) {
	Node* left = ex(p->oper.op[0]);
	Node* right = ex(p->oper.op[1]);
	result->setType(typeConstant);
	result->constant.setType(typeBool);
	result->constant.boolVal = left->constant.intVal != right->constant.intVal;
}

void MC::Driver::printx(Node* p, Node* result) {
	Node* value = ex(p->oper.op[0]);

	switch (value->getType()) {
		case typeOperator:
			switch (value->oper.oper) {
				case token::FOR: 	*out << "FOR";		break;
				case token::WHILE:	*out << "WHILE";	break;
				case token::IF:		*out << "IF";		break;
				case token::FUN:	*out << "FUN";		break;
				case token::VAR:	*out << "VAR";		break;
				default:			*out << "WRONG OPERATOR";
			}
			break;
		case typeConstant:
			*out << value->constant;
			break;
		case typeFunctionDef:
			*out << "Function " << value->functionDef.name << " defined";
			break;
		case typeEmpty:
			*out << "EMPTY";
			break;
		default:
			*out << "WRONG TYPE";
	}

	result->setType(typeEmpty);
}
void MC::Driver::println(Node* p, Node* result) {
	printx(p, result);
	*out << std::endl;
}

Node* MC::Driver::ex(Node* p) {
	auto* result = new Node;
	result->setType(typeEmpty);

	if (!p)
		return result;

	if (printer != nullptr) {
		printer->enterNode(p);
	}

	switch (p->getType()) {
		case typeConstant: {
			result->setType(typeConstant);
			result->constant = p->constant;
			goto end;
		}
		case typeVariableDef: {
			VariableDefNode *variableDef = &p->variableDef;

			if (variableDef->defaultValue != nullptr) {
				Node* defaultValue = ex(variableDef->defaultValue);
				if (variableDef->value.getType() != defaultValue->constant.getType()) {
					std::cerr << "Warning: Defined value of type " << variableDef->value.getType()
							  << " is incompatible with variable type " << defaultValue->constant.getType()
							  << "." << std::endl;
				} else {
					variableDef->value = defaultValue->constant;
				}
			} else {
				defaultConstant(variableDef->value);
			}

			addVariable(variableDef);
			goto end;
		}
		case typeVariable: {
			result->setType(typeConstant);

			std::string name = p->variable.name;
			ConstantNode* variable = findVariable(name);

			if (variable == nullptr) {
				std::cerr << "Warning: Undefined variable '" << name << "'." << std::endl;
				result->setType(typeEmpty);
				goto end;
			}

			result->constant = *variable;
			goto end;
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
			goto end;
		}
		case typeFunctionDef: {
			addFunction(&p->functionDef);
			goto end;
		}
		case typeFunction: {
			std::string name = p->function.name;

			FunctionDefNode* functionDef = findFunction(name);
			if (functionDef == nullptr) {
				std::cerr << "Warning: Undefined function '" << name << "'." << std::endl;
				goto end;
			}

			if (functionDef->params.size() != p->function.params.size()) {
				std::cerr << "Warning: Wrong number of arguments. Expecting " << functionDef->params.size()
						  << ", received " << p->function.params.size()
						  << "." << std::endl;
				goto end;
			}

			std::vector<VariableDefNode*> params;

			for (size_t i = 0; i < functionDef->params.size(); ++i) {
				VariableDefNode* formalParamDef = &functionDef->params[i];
				ConstantNode* actualParamVar = &ex(p->function.params[i])->constant;
				if (formalParamDef->value.getType() != actualParamVar->getType()) {
					std::cerr << "Warning: Passing incompatible parameter of type " << actualParamVar->getType()
							  << " instead of type " << formalParamDef->value.getType()
							  << "." << std::endl;
					goto end;
				}

				auto* node = new VariableDefNode();
				node->name = formalParamDef->name;
				node->value = *actualParamVar;
				params.push_back(node);
			}

			replaceSymbolTableScope();
			for (auto& param : params) {
				addVariable(param);
			}
			Node* res = ex(functionDef->root);
			popSymbolTableScope();

			if (functionDef->dataType != res->constant.getType()) {
				std::cerr << "Warning: Wrong return type. Expecting " << functionDef->dataType
						  << ", received " << res->constant.getType()
						  << "." << std::endl;
				goto end;
			}

			result->setType(typeConstant);
			result->constant = res->constant;
			goto end;
		}
		case typeObjectDef: {
			addObject(&p->objectDef);
			goto end;
		}
		case typeObject: {
			std::string name = p->object.name;

			ObjectDefNode *object = findObject(name);
			if (object == nullptr) {
				std::cerr << "Warning: Undefined object '" << name
						  << "'." << std::endl;
				result->setType(typeEmpty);
				goto end;
			}

			result->setType(typeConstant);
			result->constant.setType(typeObj);
			result->constant.objectTypeName = name;
			result->constant.objectVal = new ObjectDefNode;

			ObjectDefNode* objectVal = result->constant.objectVal;
//			ConstantNode** objectVal = result->constant.objectVal;
			for (auto& var : object->vars) {
//				VariableDefNode* paramDef = &var;
//				ConstantNode *ptr = &p->object.vars[i];
//				objectVal[i] = ptr;
//				if (paramDef->value.dataType == paramVar->dataType) {
//					paramDef->value = *paramVar;
//				} else {
//					defaultConstant(&paramDef->value);
//					printf("Warning: Passing incompatible parameter of type %s instead of type %s\n",
//						   paramVar->dataType), paramDef->value.dataType;
//				}
			}
			result->constant.objectVal = objectVal;

			goto end;
		}
		case typeReturn:
			goto end;
		case typeEmpty:
			goto end;
		default:
			goto end;
	}

	end:

	if (printer != nullptr) {
		printer->exitNode(result);
	}
	return result;
}


void MC::Driver::replaceSymbolTableScope() {
	auto* table = new SymbolTable();
	table->parentTable = rootSymbolTable;
	table->previousTable = currentSymbolTable;
	currentSymbolTable = table;
}

void MC::Driver::pushSymbolTableScope() {
	auto* table = new SymbolTable();
	table->parentTable = currentSymbolTable;
	table->previousTable = currentSymbolTable;
	currentSymbolTable = table;
}

void MC::Driver::popSymbolTableScope() {
	SymbolTable* table = currentSymbolTable;
	currentSymbolTable = table->previousTable;
	delete table;
}


