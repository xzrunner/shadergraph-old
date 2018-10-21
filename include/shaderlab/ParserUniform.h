#ifndef _SHADERLAB_PARSER_UNIFORM_H_
#define _SHADERLAB_PARSER_UNIFORM_H_

#include "shaderlab/Variable.h"

namespace sl
{
namespace parser
{

class Uniform : public Variable
{
public:
	Uniform(const Variable& var);
	Uniform(VariableType type, CU_STR name);

	virtual CU_STR& ToStatement(CU_STR& str) const override;

private:
	float m_value[16];

}; // Uniform

}
}

#endif // _SHADERLAB_PARSER_UNIFORM_H_