#include "shaderlab/TextureMap.h"
#include "shaderlab/Attribute.h"
#include "shaderlab/Varying.h"
#include "shaderlab/ParserUniform.h"

#include <stdio.h>

namespace sl
{
namespace parser
{

static const char* OUTPUT_NAME = "_tex_map_";

TextureMap::TextureMap()
{
	m_attributes.push_back(new Attribute(VT_FLOAT2, "texcoord"));

	m_varyings.push_back(new Varying(VT_FLOAT2, "texcoord"));

	m_uniforms.push_back(new Uniform(VT_SAMPLER2D, "texture0"));
}

CU_STR& TextureMap::ToStatements(CU_STR& str) const
{
	char buf[128];
	sprintf(buf, "vec4 %s = texture2D(u_texture0, v_texcoord);\n", OUTPUT_NAME);
	str += buf;
	return str;
}

Variable TextureMap::GetOutput() const
{
	return Variable(VT_FLOAT4, OUTPUT_NAME);
}

}
}