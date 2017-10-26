#include "shaderlab/VaryingNode.h"
#include "shaderlab/Varying.h"

#include <stdio.h>

namespace sl
{
namespace parser
{

VaryingNode::VaryingNode(Variable var)
	: m_var(var)
{
	m_varyings.push_back(new Varying(var));
}

CU_STR& VaryingNode::ToStatements(CU_STR& str) const
{
	if (!m_input) {
		return str;
	}

	CheckType(m_var, m_input->GetOutput());

	char buf[128];
	sprintf(buf, "v_%s = %s;\n", m_var.GetName(), m_input->GetOutput().GetName());
	str += buf;
	return str;
}

Variable VaryingNode::GetOutput() const
{
	return Variable(m_var.GetType(), CU_STR("v_") + m_var.GetName());
}

}
}