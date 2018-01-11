//
// Created by DAVE on 8. 1. 2018.
//

#include <headers/types.hpp>
#include "headers/ast_printer.pph"
#include <list>
#include <c++/cstdio>
#include <c++/iostream>
#include <y.tab.h>


using namespace std;

list<node*> stack;
int indent = 0;

void printConstant(node* node) {
	cout << "Constant ";
	cout << dataTypeToString(node->constant.dataType) << " " << constantValueToString(node->constant);
}

void printVariable(node* node) {
	cout << "Variable ";
	cout << node->variable.name; // << " " << constantValueToString(node->variable);
}

void printOperator(node* node) {
	cout << "Operator ";
	switch (node->oper.oper) {
		case PRINT: 	cout << "PRINT"; break;
		case PRINTLN: 	cout << "PRINTLN"; break;
		case VAR: 		cout << "VAR"; break;
		case RETURN: 	cout << "RETURN"; break;
		case ',': 		cout << "WHILE"; break;

		case WHILE:		cout << "WHILE"; break;
		case IF:		cout << "IF"; break;
		case FOR:		cout << "FOR"; break;
		case ';':		cout << "SEMICOLON"; break;
		case '=':		cout << "ASSIGN"; break;

		case UMINUS:	cout << "UNARY MINUS"; break;

		case '+':		cout << "PLUS"; break;
		case '-':		cout << "MINUS"; break;
		case '*':		cout << "MULTIPLY"; break;
		case '/':		cout << "DIVIDE"; break;
		case '%':		cout << "MODULO"; break;

		case AND:		cout << "AND"; break;
		case OR:		cout << "OR"; break;
		case NEG:		cout << "NEG"; break;

		case LT:		cout << "LT"; break;
		case LE:		cout << "LE"; break;
		case GT:		cout << "GT"; break;
		case GE:		cout << "GE"; break;
		case EQ:		cout << "EQ"; break;
		case NE:		cout << "NE"; break;
	}
}

void printFunctionDef(node* node) {
	cout << "Function Definition " << node->function.name;
}

void printFunctionCall(node* node) {
	cout << "Function Call " << node->functionCall.name;
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
		case typeVariable:
			printVariable(node);
			break;
		case typeOperator:
			printOperator(node);
			break;
		case typeFunctionDef:
			printFunctionDef(node);
			break;
		case typeFunctionCall:
			printFunctionCall(node);
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

