#include "shaderlab/Blend.h"
#include "shaderlab/ParserUniform.h"
#define STRINGIFY(A)  #A
#include "shaderlab/blend.frag"

#include <cpputil/StringHelper.h>

namespace sl
{
namespace parser
{

static const char* OUTPUT_NAME = "_blend_dst_";

Blend::Blend()
{
	m_uniforms.push_back(new Uniform(VT_SAMPLER2D, "texture1"));
	m_uniforms.push_back(new Uniform(VT_INT1, "mode"));
}

CU_STR& Blend::GetHeader(CU_STR& str) const
{
	str += blend_header;
	return str;
}

CU_STR& Blend::ToStatements(CU_STR& str) const
{
	if (!m_input) {
		return str;
	}

	CheckType(m_input->GetOutput(), VT_FLOAT4);

	CU_STR s = blend_body;
	cpputil::StringHelper::ReplaceAll(s, "_SRC_COL_", m_input->GetOutput().GetName());
	cpputil::StringHelper::ReplaceAll(s, "_DST_COL_", OUTPUT_NAME);
	str += s;
	return str;
}

Variable Blend::GetOutput() const
{
	return Variable(VT_FLOAT4, OUTPUT_NAME);
}

}
}
