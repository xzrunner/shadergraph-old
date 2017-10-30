#ifndef _SHADERLAB_PARSER_ATTRIBUTE_NODE_H_
#define _SHADERLAB_PARSER_ATTRIBUTE_NODE_H_

#include "shaderlab/Node.h"

namespace sl
{
namespace parser
{

class AttributeNode : public Node
{
public:
	AttributeNode(Variable var);

	virtual CU_STR& ToStatements(CU_STR& str) const;

	virtual Variable GetOutput() const;

private:
	Variable m_var;


}; // AttributeNode

}
}

#endif // _SHADERLAB_PARSER_ATTRIBUTE_NODE_H_