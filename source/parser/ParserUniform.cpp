#include "ParserUniform.h"

#include <string.h>

namespace sl
{
namespace parser
{

Uniform::Uniform(const Variable& var)
	: Variable(var)
{
	memset(m_value, 0, sizeof(m_value));
}

Uniform::Uniform(VariableType type, CU_STR name)
	: Variable(type, name)
{
	memset(m_value, 0, sizeof(m_value));
}

CU_STR& Uniform::ToStatement(CU_STR& str) const
{
	str += "uniform ";
	str += VAR_INFOS[m_type].name;
	str += " u_" + m_name + ";\n";
	return str;
}

}
}
