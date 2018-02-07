#include <cctype>
#include <fstream>
#include <c++/iostream>
#include <headers/types.hpp>
#include "parser.tab.hh"
#include "Driver.hpp"

using token = MC::Parser::token;


MC::Driver::Builder::Builder() {
	in = &std::cin;
	out = &std::cout;
	deb = nullptr;
}

MC::Driver::Builder MC::Driver::Builder::input(std::string filename) {
	auto* file = new std::ifstream(filename);
	if (!file->good()) {
		exit(EXIT_FAILURE);
	}
	in = file;
	return *this;
}

MC::Driver::Builder MC::Driver::Builder::input(std::istream& is) {
	if (!is.good() && is.eof()) {
		return *this;
	}
	in = &is;
	return *this;
}

MC::Driver::Builder MC::Driver::Builder::output(std::string filename) {
	auto* file = new std::ofstream(filename);
	if (!file->good()) {
		exit(EXIT_FAILURE);
	}
	out = file;
	return *this;
}

MC::Driver::Builder MC::Driver::Builder::output(std::ostream& os) {
	if (!os.good() && os.eof()) {
		return *this;
	}
	out = &os;
	return *this;
}

MC::Driver::Builder MC::Driver::Builder::debug(std::string filename) {
	auto* file = new std::ofstream(filename);
	if (!file->good()) {
		exit(EXIT_FAILURE);
	}
	deb = file;
	return *this;
}

MC::Driver::Builder MC::Driver::Builder::debug(std::ostream& os) {
	if (!os.good() && os.eof()) {
		return *this;
	}
	deb = &os;
	return *this;
}

MC::Driver::Builder MC::Driver::Builder::heap(int heapSize) {
	this->heapSize = heapSize;
	return *this;
}

MC::Driver MC::Driver::Builder::build() {
	return Driver(in, out, deb, heapSize);
};


MC::Driver::Driver(std::istream* in,
				   std::ostream* out,
				   std::ostream* deb,
				   int heapSize): in(in), out(out), deb(deb), heap(heapSize) {
}

MC::Driver::~Driver() {
	delete scanner;
	scanner = nullptr;
	delete parser;
	parser = nullptr;
	delete printer;
	printer = nullptr;
}

int MC::Driver::parse() {
	if (!in->good() && in->eof()) {
		return 1;
	}

	delete scanner;
	try {
		scanner = new MC::Scanner(in);
	} catch (std::bad_alloc& ba) {
		std::cerr << "Failed to allocate scanner: (" << ba.what() << "), exiting!!\n";
		exit(EXIT_FAILURE);
	}

	delete parser;
	try {
		parser = new MC::Parser((*scanner) /* scanner */,
								(*this) /* driver */ );
	} catch (std::bad_alloc& ba) {
		std::cerr << "Failed to allocate parser: (" << ba.what() << "), exiting!!\n";
		exit(EXIT_FAILURE);
	}

	delete printer;
	if (deb != nullptr) {
		try {
			printer = new MC::AstPrinter(deb);
		} catch (std::bad_alloc& ba) {
			std::cerr << "Failed to allocate printer: (" << ba.what() << "), exiting!!\n";
			exit(EXIT_FAILURE);
		}
	} else {
		printer = nullptr;
	}

	return parser->parse();
}


void MC::Driver::returnx(Node* p, Node* result) {
	result->setType(Node::Return);
	if (p->oper.op.empty()) {
		result->ret.value.setType(typeVoid);
	} else {
		Node* v = ex(p->oper.op[0]);
		ConstantNode& value = (v->getType() == Node::Variable) ? v->variable->value : v->constant;
		result->ret.value = value;
	}
}

void MC::Driver::whilex(Node* p, Node* result) {
	while (ex(p->oper.op[0])->constant.boolVal) {
		pushSymbolTableScope();
		Node* left = ex(p->oper.op[1]);
		if (left->getType() == Node::Return) {
			result->setType(Node::Return);
			result->ret.value = left->ret.value;
			popSymbolTableScope();
			return;
		}
		popSymbolTableScope();
	}

	result->setType(Node::Operator);
	result->oper.oper = token::WHILE;
}

void MC::Driver::forx(Node* p, Node* result) {
	for (ex(p->oper.op[0]);
		 ex(p->oper.op[1])->constant.boolVal;
		 ex(p->oper.op[2])) {

		pushSymbolTableScope();
		Node* left = ex(p->oper.op[3]);
		if (left->getType() == Node::Return) {
			popSymbolTableScope();
			result->setType(Node::Return);
			result->ret.value = left->ret.value;
			return;
		}
		popSymbolTableScope();
	}
	result->setType(Node::Operator);
	result->oper.oper = token::FOR;
}

void MC::Driver::ifx(Node* p, Node* result) {
	pushSymbolTableScope();
	if (ex(p->oper.op[0])->constant.boolVal) {
		Node* left = ex(p->oper.op[1]);
		if (left->getType() == Node::Return) {
			result->setType(left->getType());
			result->ret.value = left->ret.value;
			popSymbolTableScope();
			return;
		}
	} else if (p->oper.op.size() > 2) {
		Node* right = ex(p->oper.op[2]);
		if (right->getType() == Node::Return) {
			result->setType(right->getType());
			result->ret.value = right->ret.value;
			popSymbolTableScope();
			return;
		}
	}

	result->setType(Node::Operator);
	result->oper.oper = token::IF;
	popSymbolTableScope();
}

void MC::Driver::delimiter(Node* p, Node* result) {
	result->setType(Node::Operator);
	result->oper.oper = ';';

	if (!p->oper.op.empty()) {
		Node* left = ex(p->oper.op[0]);
		if (left->getType() == Node::Return) {
			result->setType(Node::Return);
			result->ret.value = left->ret.value;
			return;
		}
	}
	if (p->oper.op.size() > 1) {
		Node* right = ex(p->oper.op[1]);
		if (right->getType() == Node::Return) {
			result->setType(Node::Return);
			result->ret.value = right->ret.value;
			return;
		}
	}
}

void MC::Driver::dot(Node* p, Node* result) {
	Node* left = ex(p->oper.op[0]);
	ConstantNode& value = (left->getType() == Node::Variable) ? left->variable->value : left->constant;
	VariableNode* right = p->oper.op[1]->variable;
	if (value.getType() != typeObject) {
		std::cerr << "Warning: Unable to access '" << right->name
				  << "' in non-object type " << value.getType()
				  << "." << std::endl;

		result->setType(Node::Empty);
		return;
	}
	if (value.objectVal == nullptr) {
		std::cerr << "Warning: Unable to access '" << right->name
				  << "' in null object." << std::endl;
		result->setType(Node::Empty);
		return;
	}

	ObjectNode* object = value.objectVal;
	ObjectDefNode* objectDef = findObject(value.objectTypeName);

	for (size_t i = 0; i < objectDef->vars.size(); i++) {
		VariableDefNode& varDef = objectDef->vars[i];
		if (varDef.name == right->name) {
			VariableNode* var = object->vars[i];
			result->setType(Node::Variable);
			result->variable = var;
			return;
		}
	}

	std::cerr << "Warning: Unable to access member '" << right->name
			  << "' in object type " << left->constant.objectVal->name
			  << "." << std::endl;

	result->setType(Node::Empty);
}

void MC::Driver::assign(Node* p, Node* result) {
//	std::cout << "Left value type " << p->oper.op[0]->getType()
//			  << " " << p->oper.op[0]
//			  << " " << &p->oper.op[0]->variable
//			  << " " << &p->oper.op[0]->variable->value
//			  << std::endl;
//	std::cout << "Left value type ex " << lval->getType()
//			  << " " << lval
//			  << " " << &lval->variable
//			  << " " << &lval->variable->value
//			  << std::endl;
	Node* lNode = ex(p->oper.op[0]);

	if (lNode->getType() != Node::Variable) {
		std::cerr << "Warning: Expression is not assignable." << std::endl;
		result->setType(Node::Empty);
		return;
	}
	VariableNode* lvar = lNode->variable;

	Node* rval = ex(p->oper.op[1]);
	ConstantNode& rvalue = (rval->getType() == Node::Variable) ? rval->variable->value : rval->constant;

	if (lvar->value.getType() != rvalue.getType()) {
		std::cerr << "Warning: Incompatible assignment of type '" << rvalue.getType()
				  << "' to variable '" << lvar->name
				  << "' of type '" << lvar->value.getType()
				  << "'." << std::endl;
		result->setType(Node::Empty);
		return;
	}

	result->setType(Node::Constant);
	result->constant.setType(lvar->value.getType());
	result->constant = lvar->value = rvalue;
}

void MC::Driver::uminus(Node* p, Node* result) {
	Node* v = ex(p->oper.op[0]);
	ConstantNode& value = (v->getType() == Node::Variable) ? v->variable->value : v->constant;
	result->setType(Node::Constant);
	result->constant.setType(value.getType());
	if (result->constant.getType() == typeInt) {
		result->constant.intVal = -value.intVal;
	} else if (result->constant.getType() == typeDouble) {
		result->constant.doubleVal = -value.doubleVal;
	} else {
		std::cerr << "Warning: Invalid argument to unary minus." << std::endl;
	}
}

void MC::Driver::plus(Node* p, Node* result) {
	Node* l = ex(p->oper.op[0]);
	Node* r = ex(p->oper.op[1]);
	ConstantNode& left = (l->getType() == Node::Variable) ? l->variable->value : l->constant;
	ConstantNode& right = (r->getType() == Node::Variable) ? r->variable->value : r->constant;
	result->setType(Node::Constant);
	result->constant.setType(widenNodes(left, right));
	if (result->constant.getType() == typeInt) {
		result->constant.intVal = left.intVal + right.intVal;
	} else if (result->constant.getType() == typeDouble) {
		result->constant.doubleVal = left.doubleVal + right.doubleVal;
	} else {
		std::cerr << "Warning: Invalid argument to plus operator." << std::endl;
	}
}

void MC::Driver::minus(Node* p, Node* result) {
	Node* l = ex(p->oper.op[0]);
	Node* r = ex(p->oper.op[1]);
	ConstantNode& left = (l->getType() == Node::Variable) ? l->variable->value : l->constant;
	ConstantNode& right = (r->getType() == Node::Variable) ? r->variable->value : r->constant;
	result->setType(Node::Constant);
	result->constant.setType(widenNodes(left, right));
	if (result->constant.getType() == typeInt) {
		result->constant.intVal = left.intVal - right.intVal;
	} else if (result->constant.getType() == typeDouble) {
		result->constant.doubleVal = left.doubleVal - right.doubleVal;
	} else {
		std::cerr << "Warning: Invalid argument to minus operator.\n" << std::endl;
	}
}

void MC::Driver::multiply(Node* p, Node* result) {
	Node* l = ex(p->oper.op[0]);
	Node* r = ex(p->oper.op[1]);
	ConstantNode& left = (l->getType() == Node::Variable) ? l->variable->value : l->constant;
	ConstantNode& right = (r->getType() == Node::Variable) ? r->variable->value : r->constant;
	result->setType(Node::Constant);
	result->constant.setType(widenNodes(left, right));
	if (result->constant.getType() == typeInt) {
		result->constant.intVal = left.intVal * right.intVal;
	} else if (result->constant.getType() == typeDouble) {
		result->constant.doubleVal = left.doubleVal * right.doubleVal;
	} else {
		std::cerr << "Warning: Invalid argument to multiply operator.\n" << std::endl;
	}
}

void MC::Driver::divide(Node* p, Node* result) {
	Node* l = ex(p->oper.op[0]);
	Node* r = ex(p->oper.op[1]);
	ConstantNode& left = (l->getType() == Node::Variable) ? l->variable->value : l->constant;
	ConstantNode& right = (r->getType() == Node::Variable) ? r->variable->value : r->constant;
	result->setType(Node::Constant);
	result->constant.setType(widenNodes(left, right));
	if (result->constant.getType() == typeInt) {
		result->constant.intVal = left.intVal / right.intVal;
	} else if (result->constant.getType() == typeDouble) {
		result->constant.doubleVal = left.doubleVal / right.doubleVal;
	} else {
		std::cerr << "Warning: Invalid argument to divide operator.\n" << std::endl;
	}
}

void MC::Driver::modulo(Node* p, Node* result) {
	Node* l = ex(p->oper.op[0]);
	Node* r = ex(p->oper.op[1]);
	ConstantNode& left = (l->getType() == Node::Variable) ? l->variable->value : l->constant;
	ConstantNode& right = (r->getType() == Node::Variable) ? r->variable->value : r->constant;
	result->setType(Node::Constant);
	result->constant.setType(widenNodes(left, right));
	if (result->constant.getType() == typeInt) {
		result->constant.intVal = left.intVal % right.intVal;
	} else {
		std::cerr << "Warning: Invalid argument to modulo operator.\n" << std::endl;
	}
}

void MC::Driver::andx(Node* p, Node* result) {
	result->setType(Node::Constant);
	result->constant.setType(typeUndefined);

	Node* l = ex(p->oper.op[0]);
	ConstantNode& left = (l->getType() == Node::Variable) ? l->variable->value : l->constant;
	if (left.getType() != typeBool) {
		std::cerr << "Warning: Invalid argument of type " << left.getType()
				  << " to AND operator." << std::endl;
		return;
	} else if (!left.boolVal) {
		result->constant.setType(typeBool);
		result->constant.boolVal = false;
		return;
	}

	Node* r = ex(p->oper.op[1]);
	ConstantNode& right = (r->getType() == Node::Variable) ? r->variable->value : r->constant;
	if (right.getType() != typeBool) {
		std::cerr << "Warning: Invalid argument of type " << right.getType()
				  << " to AND operator." << std::endl;
	} else if (!right.boolVal) {
		result->constant.setType(typeBool);
		result->constant.boolVal = false;
		return;
	}

	result->constant.setType(typeBool);
	result->constant.boolVal = true;
}

void MC::Driver::orx(Node* p, Node* result) {
	result->setType(Node::Constant);
	result->constant.setType(typeUndefined);

	Node* l = ex(p->oper.op[0]);
	ConstantNode& left = (l->getType() == Node::Variable) ? l->variable->value : l->constant;
	if (left.getType() != typeBool) {
		std::cerr << "Warning: Invalid argument of type " << left.getType()
				  << " to OR operator." << std::endl;
		return;
	} else if (left.boolVal) {
		result->constant.setType(typeBool);
		result->constant.boolVal = true;
		return;
	}

	Node* r = ex(p->oper.op[1]);
	ConstantNode& right = (r->getType() == Node::Variable) ? r->variable->value : r->constant;
	if (right.getType() != typeBool) {
		std::cerr << "Warning: Invalid argument of type " << right.getType()
				  << " to OR operator." << std::endl;
		return;
	} else if (right.boolVal) {
		result->constant.setType(typeBool);
		result->constant.boolVal = true;
		return;
	}
	result->constant.setType(typeBool);
	result->constant.boolVal = false;
}

void MC::Driver::neg(Node* p, Node* result) {
	Node* v = ex(p->oper.op[0]);
	ConstantNode& value = (v->getType() == Node::Variable) ? v->variable->value : v->constant;
	result->setType(Node::Constant);
	result->constant.setType(typeUndefined);

	if (value.getType() != typeBool) {
		std::cerr << "Warning: Invalid argument of type " << value.getType()
				  << " to NEG operator." << std::endl;
	}
	if (value.getType() == typeBool) {
		result->constant.setType(typeBool);
		result->constant.boolVal = !value.boolVal;
	}
}

void MC::Driver::inc(Node* p, Node* result) {
	Node* v = ex(p->oper.op[0]);
	if (v->getType() != Node::Variable) {
		std::cerr << "Warning: Cannot increment non-variable." << std::endl;
	} else {
		ConstantNode& value = v->variable->value;
		result->setType(Node::Constant);

		if (value.getType() == typeInt) {
			result->constant.setType(typeInt);
			result->constant.intVal = ++value.intVal;
		} else if (value.getType() == typeDouble) {
			result->constant.setType(typeDouble);
			result->constant.doubleVal = ++value.doubleVal;
		} else {
			result->constant.setType(typeUndefined);
			std::cerr << "Warning: Cannot increment non-numeric variable." << std::endl;
		}
	}
}

void MC::Driver::dec(Node* p, Node* result) {
	Node* v = ex(p->oper.op[0]);
	if (v->getType() != Node::Variable) {
		std::cerr << "Warning: Cannot decrement non-variable." << std::endl;
	} else {
		ConstantNode& value = v->variable->value;
		result->setType(Node::Constant);

		if (value.getType() == typeInt) {
			result->constant.setType(typeInt);
			result->constant.intVal = --value.intVal;
		} else if (value.getType() == typeDouble) {
			result->constant.setType(typeDouble);
			result->constant.doubleVal = --value.doubleVal;
		} else {
			result->constant.setType(typeUndefined);
			std::cerr << "Warning: Cannot decrement non-numeric variable." << std::endl;
		}
	}
}

void MC::Driver::lt(Node* p, Node* result) {
	Node* l = ex(p->oper.op[0]);
	Node* r = ex(p->oper.op[1]);
	ConstantNode& left = (l->getType() == Node::Variable) ? l->variable->value : l->constant;
	ConstantNode& right = (r->getType() == Node::Variable) ? r->variable->value : r->constant;
	result->setType(Node::Constant);
	result->constant.setType(typeUndefined);

	DataType max = maxType(left, right);

	if (max == typeInt) {
		result->constant.setType(typeBool);
		widenNodes(left, right);
		result->constant.boolVal = left.intVal < right.intVal;
	} else if (max == typeDouble) {
		result->constant.setType(typeBool);
		widenNodes(left, right);
		result->constant.boolVal = left.doubleVal < right.doubleVal;
	} else {
		std::cerr << "Warning: Invalid arguments of types " << left.getType()
				  << " and " << right.getType()
				  << " to LT operator." << std::endl;
	}
}

void MC::Driver::le(Node* p, Node* result) {
	Node* l = ex(p->oper.op[0]);
	Node* r = ex(p->oper.op[1]);
	ConstantNode& left = (l->getType() == Node::Variable) ? l->variable->value : l->constant;
	ConstantNode& right = (r->getType() == Node::Variable) ? r->variable->value : r->constant;
	result->setType(Node::Constant);
	result->constant.setType(typeUndefined);

	DataType max = maxType(left, right);

	if (max == typeInt) {
		result->constant.setType(typeBool);
		widenNodes(left, right);
		result->constant.boolVal = left.intVal <= right.intVal;
	} else if (max == typeDouble) {
		result->constant.setType(typeBool);
		widenNodes(left, right);
		result->constant.boolVal = left.doubleVal <= right.doubleVal;
	} else {
		std::cerr << "Warning: Invalid arguments of types " << left.getType()
				  << " and " << right.getType()
				  << " to LE operator." << std::endl;
	}
}

void MC::Driver::gt(Node* p, Node* result) {
	Node* l = ex(p->oper.op[0]);
	Node* r = ex(p->oper.op[1]);
	ConstantNode& left = (l->getType() == Node::Variable) ? l->variable->value : l->constant;
	ConstantNode& right = (r->getType() == Node::Variable) ? r->variable->value : r->constant;
	result->setType(Node::Constant);
	result->constant.setType(typeUndefined);

	DataType max = maxType(left, right);

	if (max == typeInt) {
		result->constant.setType(typeBool);
		widenNodes(left, right);
		result->constant.boolVal = left.intVal > right.intVal;
	} else if (max == typeDouble) {
		result->constant.setType(typeBool);
		widenNodes(left, right);
		result->constant.boolVal = left.doubleVal > right.doubleVal;
	} else {
		std::cerr << "Warning: Invalid arguments of types " << left.getType()
				  << " and " << right.getType()
				  << " to GT operator." << std::endl;
	}}

void MC::Driver::ge(Node* p, Node* result) {
	Node* l = ex(p->oper.op[0]);
	Node* r = ex(p->oper.op[1]);
	ConstantNode& left = (l->getType() == Node::Variable) ? l->variable->value : l->constant;
	ConstantNode& right = (r->getType() == Node::Variable) ? r->variable->value : r->constant;
	result->setType(Node::Constant);
	result->constant.setType(typeUndefined);

	DataType max = maxType(left, right);

	if (max == typeInt) {
		result->constant.setType(typeBool);
		widenNodes(left, right);
		result->constant.boolVal = left.intVal >= right.intVal;
	} else if (max == typeDouble) {
		result->constant.setType(typeBool);
		widenNodes(left, right);
		result->constant.boolVal = left.doubleVal >= right.doubleVal;
	} else {
		std::cerr << "Warning: Invalid arguments of types " << left.getType()
				  << " and " << right.getType()
				  << " to GE operator." << std::endl;
	}}

void MC::Driver::eq(Node* p, Node* result) {
	Node* l = ex(p->oper.op[0]);
	Node* r = ex(p->oper.op[1]);
	ConstantNode& left = (l->getType() == Node::Variable) ? l->variable->value : l->constant;
	ConstantNode& right = (r->getType() == Node::Variable) ? r->variable->value : r->constant;
	result->setType(Node::Constant);
	result->constant.setType(typeBool);
	result->setType(Node::Constant);
	result->constant.setType(typeUndefined);

	DataType max = maxType(left, right);

	if (max == typeInt) {
		result->constant.setType(typeBool);
		widenNodes(left, right);
		result->constant.boolVal = left.intVal == right.intVal;
	} else if (max == typeDouble) {
		result->constant.setType(typeBool);
		widenNodes(left, right);
		result->constant.boolVal = left.doubleVal == right.doubleVal;
	} else if (max == typeObject) {
		result->constant.setType(typeBool);
		widenNodes(left, right);
		result->constant.boolVal = left.objectVal == right.objectVal;
	} else {
		std::cerr << "Warning: Invalid arguments of types " << left.getType()
				  << " and " << right.getType()
				  << " to EQ operator." << std::endl;
	}
}

void MC::Driver::ne(Node* p, Node* result) {
	Node* l = ex(p->oper.op[0]);
	Node* r = ex(p->oper.op[1]);
	ConstantNode& left = (l->getType() == Node::Variable) ? l->variable->value : l->constant;
	ConstantNode& right = (r->getType() == Node::Variable) ? r->variable->value : r->constant;
	result->setType(Node::Constant);
	result->constant.setType(typeUndefined);

	DataType max = maxType(left, right);

	if (max == typeInt) {
		result->constant.setType(typeBool);
		widenNodes(left, right);
		result->constant.boolVal = left.intVal != right.intVal;
	} else if (max == typeDouble) {
		result->constant.setType(typeBool);
		widenNodes(left, right);
		result->constant.boolVal = left.doubleVal != right.doubleVal;
	} else if (max == typeObject) {
		result->constant.setType(typeBool);
		widenNodes(left, right);
		result->constant.boolVal = left.objectVal != right.objectVal;
	} else {
		std::cerr << "Warning: Invalid arguments of types " << left.getType()
				  << " and " << right.getType()
				  << " to NE operator." << std::endl;
	}
}

void MC::Driver::printx(Node* p, Node* result) {
	Node* value = ex(p->oper.op[0]);

	switch (value->getType()) {
		case Node::Operator:
			switch (value->oper.oper) {
				case token::FOR:
					*out << "FOR";
					break;
				case token::WHILE:
					*out << "WHILE";
					break;
				case token::IF:
					*out << "IF";
					break;
				case token::FUN:
					*out << "FUN";
					break;
				case token::VAR:
					*out << "VAR";
					break;
				default:
					*out << "WRONG OPERATOR";
			}
			break;
		case Node::Constant:
			*out << value->constant;
			break;
		case Node::Variable:
			*out << value->variable->value;
			break;
		case Node::FunctionDef:
			*out << "Function " << value->functionDef.name << " defined";
			break;
		case Node::Empty:
			*out << "EMPTY";
			break;
		default:
			*out << "WRONG TYPE";
	}

	result->setType(Node::Empty);
}

void MC::Driver::println(Node* p, Node* result) {
	printx(p, result);
	*out << std::endl;
}

Node* MC::Driver::ex(Node* p) {
	auto* result = new Node;
	result->setType(Node::Empty);

	if (!p)
		return result;

	if (printer != nullptr) {
		printer->enterNode(p);
	}

	switch (p->getType()) {
		case Node::Constant: {
			result->setType(Node::Constant);
			result->constant = p->constant;
			goto end;
		}
		case Node::VariableDef: {
			VariableDefNode& variableDef = p->variableDef;

			ConstantNode& varDefValue = variableDef.value;
			if (variableDef.defaultValue != nullptr) {
				Node* defaultValue = ex(variableDef.defaultValue);
				ConstantNode& value = (defaultValue->getType() == Node::Variable) ? defaultValue->variable->value
																				  : defaultValue->constant;

				if (varDefValue.getType() != value.getType()) {
					std::cerr << "Warning: Defined value of type " << varDefValue.getType()
							  << " is incompatible with variable type " << value.getType()
							  << "." << std::endl;
				} else {
					varDefValue = value;
				}
			} else {
				defaultConstant(varDefValue);
			}

			auto* variable = new VariableNode;
			variable->name = variableDef.name;
			variable->value = variableDef.value;

			addVariable(variable);
			goto end;
		}
		case Node::Variable: {
			std::string name = p->variable->name;
			VariableNode* variable = findVariable(name);

			if (variable == nullptr) {
				std::cerr << "Warning: Undefined variable '" << name << "'." << std::endl;
				result->setType(Node::Empty);
				goto end;
			}

			result->setType(Node::Variable);
			result->variable = variable;
			goto end;
		}
		case Node::Operator: {
			switch (p->oper.oper) {
				case token::PRINT:
					printx(p, result);
					break;
				case token::PRINTLN:
					println(p, result);
					break;
				case token::RETURN:
					returnx(p, result);
					break;

				case token::WHILE:
					whilex(p, result);
					break;
				case token::IF:
					ifx(p, result);
					break;
				case token::FOR:
					forx(p, result);
					break;
				case ';':
					delimiter(p, result);
					break;
				case '.':
					dot(p, result);
					break;
				case '=':
					assign(p, result);
					break;

				case token::UMINUS:
					uminus(p, result);
					break;

				case '+':
					plus(p, result);
					break;
				case '-':
					minus(p, result);
					break;
				case '*':
					multiply(p, result);
					break;
				case '/':
					divide(p, result);
					break;
				case '%':
					modulo(p, result);
					break;

				case token::AND:
					andx(p, result);
					break;
				case token::OR:
					orx(p, result);
					break;
				case token::NEG:
					neg(p, result);
					break;

				case token::INC:
					inc(p, result);
					break;
				case token::DEC:
					dec(p, result);
					break;

				case token::LT:
					lt(p, result);
					break;
				case token::LE:
					le(p, result);
					break;
				case token::GT:
					gt(p, result);
					break;
				case token::GE:
					ge(p, result);
					break;
				case token::EQ:
					eq(p, result);
					break;
				case token::NE:
					ne(p, result);
					break;

				default:
					std::cerr << "Error: unknown operator '" << p->oper.oper << "'." << std::endl;
			}
			goto end;
		}
		case Node::FunctionDef: {
			addFunction(p->functionDef);
			goto end;
		}
		case Node::Function: {
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

			std::vector<VariableNode*> params;

			for (size_t i = 0; i < functionDef->params.size(); i++) {
				VariableDefNode& formalParamDef = functionDef->params[i];
				Node* v = ex(p->function.params[i]);
				ConstantNode& actualParamVar = (v->getType() == Node::Variable) ? v->variable->value : v->constant;
				if (formalParamDef.value.getType() != actualParamVar.getType()) {
					std::cerr << "Warning: Passing incompatible parameter of type " << actualParamVar.getType()
							  << " instead of type " << formalParamDef.value.getType()
							  << "." << std::endl;
					goto end;
				}

				auto* node = new VariableNode();
				node->name = formalParamDef.name;
				node->value = actualParamVar;
				params.push_back(node);
			}

			replaceSymbolTableScope();
			for (VariableNode* param : params) {
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

			result->setType(Node::Constant);

			if (functionDef->dataType == typeVoid) {
				result->constant.setType(typeVoid);
			} else {
				result->constant = res->constant;
			}

			goto end;
		}
		case Node::ObjectDef: {
			addObject(p->objectDef);
			goto end;
		}
		case Node::Object: {
			std::string name = p->object.name;

			ObjectDefNode* objectDef = findObject(name);
			if (objectDef == nullptr) {
				std::cerr << "Warning: Undefined object '" << name
						  << "'." << std::endl;
				result->setType(Node::Empty);
				goto end;
			}

			if (heap.isFull()) {
				heap.performGarbageCollection(currentSymbolTable);
			}

			auto* objectVal = heap.allocateNewObject();
			if (objectVal == nullptr) {
				std::cerr << "ERROR: Heap is full. Cannot allocate new objects." << std::endl;
				goto end;
			}

			for (auto& varDef : objectDef->vars) {
				auto* var = new VariableNode;
				var->name = varDef.name;
				var->value = varDef.value;
				if (varDef.defaultValue != nullptr) {
					Node* defaultVal = ex(varDef.defaultValue);
					var->value = defaultVal->constant;
				} else {
					defaultConstant(var->value);
				}
				objectVal->vars.push_back(var);
			}

			result->setType(Node::Constant);
			result->constant.setType(typeObject);
			result->constant.objectTypeName = name;
			result->constant.objectVal = objectVal;

			goto end;
		}
		case Node::Return:
			goto end;
		case Node::Empty:
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

VariableNode* MC::Driver::findVariable(const std::string& symbol) {
	return currentSymbolTable->findVariable(symbol);
}

FunctionDefNode* MC::Driver::findFunction(const std::string& symbol) {
	return currentSymbolTable->findFunction(symbol);
}

ObjectDefNode* MC::Driver::findObject(const std::string& symbol) {
	return currentSymbolTable->findObject(symbol);
}

void MC::Driver::addVariable(VariableNode* variableDef) {
	currentSymbolTable->addVariable(variableDef);
}

void MC::Driver::addFunction(FunctionDefNode& function) {
	currentSymbolTable->addFunction(function);
}

void MC::Driver::addObject(ObjectDefNode& object) {
	currentSymbolTable->addObject(object);
}


