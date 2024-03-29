#ifndef _SHADERLAB_PARSER_VARYING_H_
#define _SHADERLAB_PARSER_VARYING_H_

#include "shaderlab/Variable.h"

namespace sl
{
namespace parser
{

class Varying : public Variable
{
public:
	Varying(const Variable& var);
	Varying(VariableType type, CU_STR name);

	virtual CU_STR& ToStatement(CU_STR& str) const override;

}; // Varying

}
}

#endif // _SHADERLAB_PARSER_VARYING_H_