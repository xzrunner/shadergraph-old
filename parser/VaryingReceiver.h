#ifndef _SHADERLAB_PARSER_VARYING_RECEIVER_H_
#define _SHADERLAB_PARSER_VARYING_RECEIVER_H_

#include "Node.h"

namespace sl
{
namespace parser
{

class VaryingReceiver : public Node
{
public:
	VaryingReceiver(Variable var) : m_var(var) {}

	virtual std::string& ToStatements(std::string& str) const { return str; }

	virtual Variable GetOutput() const {
		return Variable(m_var.GetType(), std::string("v_") + m_var.GetName());
	}

private:
	Variable m_var;

}; // VaryingReceiver

}
}

#endif // _SHADERLAB_PARSER_VARYING_RECEIVER_H_