#include "shaderlab/PositionTrans.h"
#include "shaderlab/Attribute.h"
#include "shaderlab/ParserUniform.h"

namespace sl
{
namespace parser
{

PositionTrans::PositionTrans()
{
	m_attributes.push_back(new Attribute(VT_FLOAT4, "position"));

	m_uniforms.push_back(new Uniform(VT_MAT4, "projection"));
	m_uniforms.push_back(new Uniform(VT_MAT4, "modelview"));
}

CU_STR& PositionTrans::ToStatements(CU_STR& str) const
{
	str += "gl_Position = u_projection * u_modelview * position;\n";
	return str;
}

}
}