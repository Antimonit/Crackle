//
// Created by DAVE on 8. 1. 2018.
//

#include <headers/types.h>
#include "ast_printer.h"
#include <list>
#include <c++/cstdio>
#include <c++/iostream>
#include <y.tab.h>


using namespace std;

list<node*> stack;

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
		case '=':		cout << "EQUALS"; break;

		case UMINUS:	cout << "Unary Minus " << constantValueToString(node->constant); break;

		case '+':		cout << "PLUS " << constantValueToString(node->constant); break;
		case '-':		cout << "MINUS " << constantValueToString(node->constant); break;
		case '*':		cout << "MULTIPLY " << constantValueToString(node->constant); break;
		case '/':		cout << "DIVIDE " << constantValueToString(node->constant); break;
		case '%':		cout << "MODULO " << constantValueToString(node->constant); break;

		case AND:		cout << "AND " << constantValueToString(node->constant); break;
		case OR:		cout << "OR " << constantValueToString(node->constant); break;
		case NEG:		cout << "NEG " << constantValueToString(node->constant); break;

		case LT:		cout << "LT " << constantValueToString(node->constant); break;
		case LE:		cout << "LE " << constantValueToString(node->constant); break;
		case GT:		cout << "GT " << constantValueToString(node->constant); break;
		case GE:		cout << "GE " << constantValueToString(node->constant); break;
		case EQ:		cout << "EQ " << constantValueToString(node->constant); break;
		case NE:		cout << "NE " << constantValueToString(node->constant); break;
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
	for (int i = 0; i < stack.size(); ++i) {
		cout << "  ";
	}
	if (entering)
		cout << "NODE > ";
	else
		cout << "NODE < ";

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
	printNode(true, node);
	stack.push_back(node);
}

extern "C" void exitNode(node* node) {
	stack.pop_back();
	printNode(false, node);
}

