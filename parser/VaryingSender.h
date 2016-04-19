#ifndef _SHADERLAB_PARSER_VARYING_SENDER_H_
#define _SHADERLAB_PARSER_VARYING_SENDER_H_

#include "Node.h"
#include "VariableType.h"

namespace sl
{
namespace parser
{

class VaryingSender : public Node
{
public:
	VaryingSender(Variable var);

	virtual std::string& ToStatements(std::string& str) const;

	virtual Variable GetOutput() const;

private:
	Variable m_var;

}; // VaryingSender

}
}

#endif // _SHADERLAB_PARSER_VARYING_SENDER_H_