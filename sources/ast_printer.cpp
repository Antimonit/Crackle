//
// Created by DAVE on 8. 1. 2018.
//

#include <headers/types.hpp>
#include "headers/ast_printer.hpp"
#include <list>
#include <c++/cstdio>
#include <c++/iostream>
#include <parser.tab.hh>

using namespace std;
using token = MC::Parser::token;

list<node*> stack;
int indent = 0;

void printConstant(node* node) {
	cout << "Constant ";
	cout << dataTypeToString(node->constant.dataType) << " " << constantValueToString(node->constant);
}

void printOperator(node* node) {
	cout << "Operator ";
	switch (node->oper.oper) {
		case token::PRINT: 		cout << "PRINT"; break;
		case token::PRINTLN: 	cout << "PRINTLN"; break;
		case token::VAR: 		cout << "VAR"; break;
		case token::RETURN: 	cout << "RETURN"; break;
		case ',': 				cout << "WHILE"; break;

		case token::WHILE:		cout << "WHILE"; break;
		case token::IF:			cout << "IF"; break;
		case token::FOR:		cout << "FOR"; break;
		case ';':				cout << "SEMICOLON"; break;
		case '=':				cout << "ASSIGN"; break;

		case token::UMINUS:		cout << "UNARY MINUS"; break;

		case '+':				cout << "PLUS"; break;
		case '-':				cout << "MINUS"; break;
		case '*':				cout << "MULTIPLY"; break;
		case '/':				cout << "DIVIDE"; break;
		case '%':				cout << "MODULO"; break;

		case token::AND:		cout << "AND"; break;
		case token::OR:			cout << "OR"; break;
		case token::NEG:		cout << "NEG"; break;

		case token::LT:			cout << "LT"; break;
		case token::LE:			cout << "LE"; break;
		case token::GT:			cout << "GT"; break;
		case token::GE:			cout << "GE"; break;
		case token::EQ:			cout << "EQ"; break;
		case token::NE:			cout << "NE"; break;
	}
}

void printVariableDef(node* node) {
	cout << "Variable Definition ";
	cout << dataTypeToString(node->variableDef.value.dataType) << " ";
	cout << node->variableDef.name << " ";
}

void printVariable(node* node) {
	cout << "Variable ";
	cout << node->variable.name; // << " " << constantValueToString(node->variable);
}

void printFunctionDef(node* node) {
	cout << "Function Definition " << node->functionDef.name;
}

void printFunction(node* node) {
	cout << "Function " << node->function.name;
}

void printObjectDef(node* node) {
	cout << "Object Definition " << node->objectDef.name;
}

void printObject(node* node) {
	cout << "Object " << node->object.name;
}

void printReturn(node* node) {
	cout << "Return " << constantValueToString(node->ret.value);
}


void printNode(bool entering, node* node) {
	cout << '\t';
	for (int i = 0; i < indent; ++i) {
		cout << static_cast<char>(179) << " ";	/* │ */
	}
	if (entering)
		cout << static_cast<char>(218); /* ┌ */
	else
		cout << static_cast<char>(192); /* └ */

	switch (node->type) {
		case typeConstant:
			printConstant(node);
			break;
		case typeOperator:
			printOperator(node);
			break;
		case typeVariableDef:
			printVariableDef(node);
			break;
		case typeVariable:
			printVariable(node);
			break;
		case typeFunctionDef:
			printFunctionDef(node);
			break;
		case typeFunction:
			printFunction(node);
			break;
		case typeObjectDef:
			printObjectDef(node);
			break;
		case typeObject:
			printObject(node);
			break;
		case typeReturn:
			printReturn(node);
			break;
		case typeEmpty:
			cout << "EMPTY";
			break;
		default:
			cout << "unknown";
			break;
	}
	cout << endl;
}

extern "C" void enterNode(node* node) {
	stack.push_back(node);
	if (stack.back()->type != typeOperator || stack.back()->oper.oper != ';') {
		printNode(true, node);
		indent++;
	}
}

extern "C" void exitNode(node* node) {
	if (stack.back()->type != typeOperator || stack.back()->oper.oper != ';') {
		indent--;
		printNode(false, node);
	}
	stack.pop_back();
}

