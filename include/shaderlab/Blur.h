#ifndef _SHADERLAB_PARSER_BLUR_H_
#define _SHADERLAB_PARSER_BLUR_H_

#include "shaderlab/Filter.h"
#include "shaderlab/ParserUniform.h"

#define STRINGIFY(A)  #A
#include "shaderlab/blur.frag"

namespace sl
{
namespace parser
{

/**
 *  @brief
 *    simple blur
 *
 *  @remarks
 *    input: uniform float u_radius
 */
class Blur : public Filter
{
public:
	Blur() : Filter("_col_blur_") {
		m_uniforms.push_back(new Uniform(VT_FLOAT1, "radius"));
	}
	
protected:
	virtual CU_STR GetBody() const override {
		return blur_body;
	}

}; // Blur

}
}

#endif // _SHADERLAB_PARSER_BLUR_H_