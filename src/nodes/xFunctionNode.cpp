//
// Created by DAVE on 7. 2. 2018.
//

#include <nodes/constant/xConstantVoidNode.h>
#include <c++/sstream>
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
		throw "Undefined function '" + name + "'.";
	}

	if (functionDef->params.size() != params.size()) {
		std::stringstream ss;
		ss	<< "Wrong number of arguments. Expecting " << functionDef->params.size()
	  		<< ", received " << params.size()
		 	<< ".";
		throw ss.str();
	}

	std::vector<xVariableNode*> params;

	for (size_t i = 0; i < functionDef->params.size(); i++) {
		xVariableDefNode* formalParamDef = functionDef->params[i];
		xNode* v = this->params[i]->ex(driver);
		xConstantNode* actualParamVar = v->getValue()->clone();
//		xNode* v = ex(p->function.params[i]);
//		xConstantNode& actualParamVar = (v->getType() == Node::Variable) ? v->variable->value : v->constant;
		if (formalParamDef->value->getType() != actualParamVar->getType()) {
			std::stringstream ss;
			ss	<< "Passing incompatible parameter of type " << actualParamVar->getType()
				<< " instead of type " << formalParamDef->value->getType()
				<< ".";
			throw ss.str();
		}

		auto* param = new xVariableNode(formalParamDef->name, actualParamVar);
		params.push_back(param);
	}

	driver->replaceSymbolTableScope();
	for (xVariableNode* param : params) {
		driver->addVariable(param);
	}
	xNode* res = functionDef->root->ex(driver);
	driver->popSymbolTableScope();

	xReturnNode* returnRes = dynamic_cast<xReturnNode*>(res);
	if (functionDef->dataType != returnRes->value->getValue()->getType()) {
		std::stringstream ss;
		ss	<< "Wrong return type. Expecting " << functionDef->dataType
		  	<< ", received " << res->getValue()->getType()
			<< ".";
		throw ss.str();
	}

	if (functionDef->dataType == typeVoid) {
		return new xConstantVoidNode();		// TODO: generify and remove this?
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
