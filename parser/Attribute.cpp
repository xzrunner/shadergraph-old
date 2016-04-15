#include "Attribute.h"

namespace sl
{
namespace parser
{

Attribute::Attribute(VariableType type, std::string name)
	: Variable(type, name)
{
}

std::string& Attribute::ToStatement(std::string& str) const
{
	str += "attribute ";
	str += VAR_INFOS[m_type].name;
	str += " " + m_name + ";\n";
	return str;
}

}
}