#include "shaderlab/Filter.h"
#include "shaderlab/StringHelper.h"

namespace sl
{
namespace parser
{

CU_STR& Filter::ToStatements(CU_STR& str) const
{
	if (!m_input) {
		return str;
	}

	CheckType(m_input->GetOutput(), VT_FLOAT4);

	CU_STR s = GetBody();
	StringHelper::ReplaceAll(s, "_DST_COL_", m_output);
	str += s;
	return str;
}

}
}