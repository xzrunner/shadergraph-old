#ifndef _SHADERLAB_PARSER_VARYING_NODE_H_
#define _SHADERLAB_PARSER_VARYING_NODE_H_

#include "shaderlab/Node.h"

namespace sl
{
namespace parser
{

class VaryingNode : public Node
{
public:
	VaryingNode(Variable var);

	virtual CU_STR& ToStatements(CU_STR& str) const override;

	virtual Variable GetOutput() const override;

private:
	Variable m_var;

}; // VaryingNode

}
}

#endif // _SHADERLAB_PARSER_VARYING_NODE_H_