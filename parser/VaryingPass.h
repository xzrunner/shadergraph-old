#ifndef _SHADERLAB_PARSER_VARYING_PASS_H_
#define _SHADERLAB_PARSER_VARYING_PASS_H_

#include "Node.h"
#include "VariableType.h"

namespace sl
{
namespace parser
{

class VaryingPass : public Node
{
public:
	VaryingPass(Variable var);

	virtual std::string& ToStatements(std::string& str) const;

	virtual Variable GetOutput() const;

private:
	Variable m_var;

}; // VaryingPass

}
}

#endif // _SHADERLAB_PARSER_VARYING_PASS_H_