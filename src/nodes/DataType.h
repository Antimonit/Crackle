//
// Created by DAVE on 14. 2. 2018.
//

#ifndef CRACKLE_DATATYPE_H
#define CRACKLE_DATATYPE_H

#include <c++/string>
#include <c++/ostream>

enum DataType {
	typeInt,
	typeDouble,
	typeString,
	typeBool,
	typeVoid,
	typeObject,
	typeUndefined
};

std::string typeToString(DataType value);
std::ostream& operator<<(std::ostream& out, DataType value);
std::string operator+(const std::string& out, DataType value);


#endif //CRACKLE_DATATYPE_H
