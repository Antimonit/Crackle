//
// Created by DAVE on 7. 2. 2018.
//

#ifndef CRACKLE_XFUNCTIONNODE_H
#define CRACKLE_XFUNCTIONNODE_H


#include <c++/string>
#include <c++/vector>
#include "xNode.h"
#include "xConstantNode.h"
#include "xEmptyNode.h"
#include "xFunctionDefNode.h"

class xFunctionNode : xNode {
public:
	std::string name;
	std::vector<xNode*> params;	// actual parameters


	xNode* ex(MC::Driver* driver) override {

		xFunctionDefNode* functionDef = driver->findFunction(name);
		if (functionDef == nullptr) {
			std::cerr << "Warning: Undefined function '" << name << "'." << std::endl;
			return new xEmptyNode;
		}

		if (functionDef->params.size() != params.size()) {
			std::cerr << "Warning: Wrong number of arguments. Expecting " << functionDef->params.size()
					  << ", received " << params.size()
					  << "." << std::endl;
			return new xEmptyNode;
		}

		std::vector<xVariableNode*> params;

		for (size_t i = 0; i < functionDef->params.size(); i++) {
			xVariableDefNode* formalParamDef = functionDef->params[i];
			xConstantNode* actualParamVar = params[i]->getValue();
//			xConstantNode& actualParamVar = (v->getType() == Node::Variable) ? v->variable->value : v->constant;
			if (formalParamDef->value->getType() != actualParamVar->getType()) {
				std::cerr << "Warning: Passing incompatible parameter of type " << actualParamVar->getType()
						  << " instead of type " << formalParamDef->value->getType()
						  << "." << std::endl;
				return new xEmptyNode;
			}

			auto* node = new xVariableNode();
			node->name = formalParamDef->name;
			node->value = actualParamVar;
			params.push_back(node);
		}

		driver->replaceSymbolTableScope();
		for (xVariableNode* param : params) {
			driver->addVariable(param);
		}
		xNode* res = functionDef->root->ex(driver);
		driver->popSymbolTableScope();

		if (functionDef->dataType != res->getValue()->getType()) {
			std::cerr << "Warning: Wrong return type. Expecting " << functionDef->dataType
					  << ", received " << res->getValue()->getType()
					  << "." << std::endl;
			return new xEmptyNode;
		}

		if (functionDef->dataType == typeVoid) {
			return new xConstantNode(typeVoid);
		} else {
			return res;
		}
	}

};


#endif //CRACKLE_XFUNCTIONNODE_H
