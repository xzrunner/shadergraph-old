#include "ColorAddMulti.h"
#include "Attribute.h"
#include "Varying.h"
#include "StringHelper.h"

namespace sl
{
namespace parser
{

ColorAddMulti::ColorAddMulti()
{
	m_attributes.push_back(new Attribute(VT_FLOAT4, "color"));
	m_attributes.push_back(new Attribute(VT_FLOAT4, "additive"));

	m_varyings.push_back(new Varying(VT_FLOAT4, "color"));
	m_varyings.push_back(new Varying(VT_FLOAT4, "additive"));
}

std::string& ColorAddMulti::ToStatements(std::string& str) const
{
	if (!m_input) {
		return str;
	}

	std::string s = " \
		vec4 col_add_multi; \
		col_add_multi.xyz = _TMP_.xyz * v_color.xyz; \
		col_add_multi.w = _TMP_.w; \
		col_add_multi *= v_color.w; \
		col_add_multi.xyz += v_additive.xyz * _TMP_.w * v_color.w; ";
	StringHelper::ReplaceAll(s, "_TMP_", m_input->OutputName());
	str += s;
	return str;
}

}
}