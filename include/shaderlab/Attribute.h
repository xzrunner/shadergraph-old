#ifndef _SHADERLAB_PARSER_ATTRIBUTE_H_
#define _SHADERLAB_PARSER_ATTRIBUTE_H_

#include "shaderlab/Variable.h"

namespace sl
{
namespace parser
{

class Attribute : public Variable
{
public:
	Attribute(const Variable& var);
	Attribute(VariableType type, CU_STR name);

	virtual CU_STR& ToStatement(CU_STR& str) const;

}; // Attribute

}
}

#endif // _SHADERLAB_PARSER_ATTRIBUTE_H_