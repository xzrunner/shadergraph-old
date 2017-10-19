#include "Mask.h"
#include "SL_StringHelper.h"
#include "ParserUniform.h"

#define STRINGIFY(A)  #A
#include <shaderlab/mask.frag>

namespace sl
{
namespace parser
{

static const char* OUTPUT_NAME = "_mask_dst_";

Mask::Mask()
{
	m_uniforms.push_back(new Uniform(VT_SAMPLER2D, "texture1"));
}

CU_STR& Mask::ToStatements(CU_STR& str) const
{
	if (!m_input) {
		return str;
	}

	CheckType(m_input->GetOutput(), VT_FLOAT4);

	CU_STR s = mask_body;
	StringHelper::ReplaceAll(s, "_SRC_COL_", m_input->GetOutput().GetName());
	StringHelper::ReplaceAll(s, "_DST_COL_", OUTPUT_NAME);
	str += s;
	return str;
}

Variable Mask::GetOutput() const
{
	return Variable(VT_FLOAT4, OUTPUT_NAME); 
}

}
}
