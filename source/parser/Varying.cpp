#include "Varying.h"

namespace sl
{
namespace parser
{

Varying::Varying(const Variable& var)
	: Variable(var)
{
}

Varying::Varying(VariableType type, CU_STR name)
	: Variable(type, name)
{
}

CU_STR& Varying::ToStatement(CU_STR& str) const
{
	str += "varying ";
	str += VAR_INFOS[m_type].name;
	str += " v_" + m_name + ";\n";
	return str;
}

}
}