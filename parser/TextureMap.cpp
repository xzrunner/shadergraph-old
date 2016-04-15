#include "TextureMap.h"
#include "Attribute.h"
#include "Varying.h"
#include "Uniform.h"

namespace sl
{
namespace parser
{

TextureMap::TextureMap()
{
	m_attributes.push_back(new Attribute(VT_FLOAT2, "texcoord"));

	m_varyings.push_back(new Varying(VT_FLOAT2, "texcoord"));

	m_uniforms.push_back(new Uniform(VT_SAMPLER2D, "texture0"));
}

std::string& TextureMap::ToStatements(std::string& str) const
{
	str += "vec4 src = texture2D(u_texture0, v_texcoord); ";
	return str;
}

}
}