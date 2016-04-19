#include "VaryingSender.h"
#include "Varying.h"

namespace sl
{
namespace parser
{

VaryingSender::VaryingSender(Variable var)
	: m_var(var)
{
	m_varyings.push_back(new Varying(var.GetType(), var.GetName()));
}

std::string& VaryingSender::ToStatements(std::string& str) const
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

Variable VaryingSender::GetOutput() const
{
	return Variable(m_var.GetType(), std::string("v_") + m_var.GetName());
}

}
}