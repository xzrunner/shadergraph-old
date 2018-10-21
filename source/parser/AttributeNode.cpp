#include "shaderlab/AttributeNode.h"
#include "shaderlab/Attribute.h"

namespace sl
{
namespace parser
{

AttributeNode::AttributeNode(Variable var)
	: m_var(var)
{
	m_attributes.push_back(new Attribute(var));
}

CU_STR& AttributeNode::ToStatements(CU_STR& str) const
{
	return str;
}

Variable AttributeNode::GetOutput() const
{
	return m_var;
}

}
}