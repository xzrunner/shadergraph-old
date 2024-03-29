#ifndef _SHADERLAB_PARSER_BURNING_MAP_H_
#define _SHADERLAB_PARSER_BURNING_MAP_H_

#include "shaderlab/Filter.h"
#include "shaderlab/ParserUniform.h"

#define STRINGIFY(A)  #A
#include "shaderlab/burning_map.frag"

namespace sl
{
namespace parser
{

/**
 *  @brief
 *    burning map
 */
class BurningMap : public Filter
{
public:
	BurningMap() : Filter("_burning_map_") {
		m_uniforms.push_back(new Uniform(VT_SAMPLER2D, "texture1"));
		m_uniforms.push_back(new Uniform(VT_SAMPLER2D, "height_map_tex"));
		m_uniforms.push_back(new Uniform(VT_SAMPLER2D, "border_gradient_tex"));
		m_uniforms.push_back(new Uniform(VT_FLOAT1, "lifetime"));
		m_uniforms.push_back(new Uniform(VT_FLOAT1, "time"));
	}

	virtual CU_STR& GetHeader(CU_STR& str) const override {
		str += burning_map_header;
		return str;
	}

protected:
	virtual CU_STR GetBody() const override {
		return burning_map_body;
	}

}; // BurningMap

}
}

#endif // _SHADERLAB_PARSER_BURNING_MAP_H_