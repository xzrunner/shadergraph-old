#ifndef _SHADERLAB_PARSER_BLUR_VERT_H_
#define _SHADERLAB_PARSER_BLUR_VERT_H_

#include "shaderlab/Filter.h"
#include "shaderlab/ParserUniform.h"

#define STRINGIFY(A)  #A
#include "shaderlab/gaussian_blur_vert.frag"

namespace sl
{
namespace parser
{

/**
 *  @brief
 *    gaussian blur filter
 */
class GaussianBlurVert : public Filter
{
public:
	GaussianBlurVert() 
		: Filter("_col_gaussian_blur_") 
	{
		m_uniforms.push_back(new Uniform(VT_FLOAT1, "tex_height"));
	}
	
	virtual CU_STR& GetHeader(CU_STR& str) const { 
		str += gaussian_blur_vert_header;
		return str; 
	}

protected:
	virtual CU_STR GetBody() const {
		return gaussian_blur_vert_body;
	}

}; // GaussianBlurVert

}
}

#endif // _SHADERLAB_PARSER_BLUR_VERT_H_