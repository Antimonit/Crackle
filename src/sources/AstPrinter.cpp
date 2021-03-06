//
// Created by DAVE on 8. 1. 2018.
//

#include <list>
#include <parser.tab.hh>
#include "headers/AstPrinter.hpp"

using token = MC::Parser::token;

void MC::AstPrinter::printOperator(Node* node) {
	out << "Operator ";
	switch (node->oper.oper) {
		case token::PRINT: 		out << "PRINT"; break;
		case token::PRINTLN: 	out << "PRINTLN"; break;
		case token::VAR: 		out << "VAR"; break;
		case token::RETURN: 	out << "RETURN"; break;
		case ',': 				out << "WHILE"; break;

		case token::WHILE:		out << "WHILE"; break;
		case token::IF:			out << "IF"; break;
		case token::FOR:		out << "FOR"; break;
		case ';':				out << "SEMICOLON"; break;
		case '.':				out << "DOT"; break;
		case '=':				out << "ASSIGN"; break;

		case token::UMINUS:		out << "UNARY MINUS"; break;

		case '+':				out << "PLUS"; break;
		case '-':				out << "MINUS"; break;
		case '*':				out << "MULTIPLY"; break;
		case '/':				out << "DIVIDE"; break;
		case '%':				out << "MODULO"; break;

		case token::AND:		out << "AND"; break;
		case token::OR:			out << "OR"; break;
		case token::NEG:		out << "NEG"; break;

		case token::LT:			out << "LT"; break;
		case token::LE:			out << "LE"; break;
		case token::GT:			out << "GT"; break;
		case token::GE:			out << "GE"; break;
		case token::EQ:			out << "EQ"; break;
		case token::NE:			out << "NE"; break;
		default:				out << "<unknown>"; break;
	}
}

void MC::AstPrinter::printNode(bool entering, Node* node) {
	out << '\t';
	for (int i = 0; i < indent; ++i) {
		out << static_cast<char>(179) << " ";	/* │ */
	}
	if (entering)
		out << static_cast<char>(218); /* ┌ */
	else
		out << static_cast<char>(192); /* └ */

	switch (node->getType()) {
		case Node::Constant:
			out << "Constant " << node->constant.getType() << " " << node->constant;
			break;
		case Node::Operator:
			printOperator(node);
			break;
		case Node::VariableDef:
			out << "Variable Definition " << node->variableDef;
			break;
		case Node::Variable:
			out << "Variable " << node->variable->name;
			break;
		case Node::FunctionDef:
			out << "Function Definition " << node->functionDef.name;
			break;
		case Node::Function:
			out << "Function " << node->function.name;
			break;
		case Node::ObjectDef:
			out << "Object Definition " << node->objectDef.name;
			break;
		case Node::Object:
			out << "Object " << node->object.name;
			break;
		case Node::Return:
			out << "Return " << node->ret.value;
			break;
		case Node::Empty:
			out << "EMPTY";
			break;
		default:
			out << "unknown";
			break;
	}
	out << std::endl;
}

void MC::AstPrinter::enterNode(Node* node) {
	stack.push_back(node);
	if (stack.back()->getType() != Node::Operator || stack.back()->oper.oper != ';') {
		printNode(true, node);
		indent++;
	}
}

void MC::AstPrinter::exitNode(Node* node) {
	if (stack.back()->getType() != Node::Operator || stack.back()->oper.oper != ';') {
		indent--;
		printNode(false, node);
	}
	stack.pop_back();
}
