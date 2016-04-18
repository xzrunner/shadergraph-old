#include "VaryingPass.h"
#include "Varying.h"

namespace sl
{
namespace parser
{

VaryingPass::VaryingPass(VariableType type, std::string name)
	: m_name(name)
{
	m_varyings.push_back(new Varying(type, name));
}

std::string& VaryingPass::ToStatements(std::string& str) const
{
	if (!m_input) {
		return str;
	}

	char buf[128];
	sprintf(buf, "v_%s = %s;\n", m_name.c_str(), m_input->OutputName());
	str += buf;
	return str;
}

}
}