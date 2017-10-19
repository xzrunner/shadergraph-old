#include "Attribute.h"

namespace sl
{
namespace parser
{

Attribute::Attribute(const Variable& var)
	: Variable(var)
{
}

Attribute::Attribute(VariableType type, CU_STR name)
	: Variable(type, name)
{
}

CU_STR& Attribute::ToStatement(CU_STR& str) const
{
	str += "attribute ";
	str += VAR_INFOS[m_type].name;
	str += " " + m_name + ";\n";
	return str;
}

}
}