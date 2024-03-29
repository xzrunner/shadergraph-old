#ifndef _SHADERLAB_PARSER_SWIRL_H_
#define _SHADERLAB_PARSER_SWIRL_H_

#include "shaderlab/Filter.h"
#include "shaderlab/ParserUniform.h"

#define STRINGIFY(A)  #A
#include "shaderlab/swirl.frag"

namespace sl
{
namespace parser
{

/**
 *  @brief
 *    simple blur
 *
 *  @remarks
 *    input: uniform float u_radius;
 *           uniform float u_angle;
 *           uniform vec2 u_center;
 */
class Swirl : public Filter
{
public:
	Swirl() : Filter("_col_swirl_") {
		m_uniforms.push_back(new Uniform(VT_FLOAT1, "radius"));
		m_uniforms.push_back(new Uniform(VT_FLOAT1, "angle"));
		m_uniforms.push_back(new Uniform(VT_FLOAT2, "center"));
	}

protected:
	virtual CU_STR GetBody() const override {
		return swirl_body;
	}

}; // Swirl

}
}

#endif // _SHADERLAB_PARSER_SWIRL_H_