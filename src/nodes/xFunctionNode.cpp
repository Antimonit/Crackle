//
// Created by DAVE on 7. 2. 2018.
//

#include "xFunctionNode.h"
#include "Driver.hpp"

#include "xConstantNode.h"
#include "xEmptyNode.h"
#include "xVariableNode.h"
#include "xVariableDefNode.h"
#include "xFunctionDefNode.h"
#include "xReturnNode.h"

xNode* xFunctionNode::ex(MC::Driver* driver) {

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

	xReturnNode* returnRes = dynamic_cast<xReturnNode*>(res);
	if (functionDef->dataType != returnRes->value->getValue()->getType()) {
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

std::string xFunctionNode::print() const {
	return "Function";
}

std::ostream& xFunctionNode::print(std::ostream& out) const {
	return out << name << " "
			   << params.size() << " params";
}
