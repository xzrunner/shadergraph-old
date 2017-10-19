#ifndef _SHADERLAB_PARSER_BLUR_HORI_H_
#define _SHADERLAB_PARSER_BLUR_HORI_H_

#include "Filter.h"
#include "ParserUniform.h"

#define STRINGIFY(A)  #A
#include "gaussian_blur_hori.frag"

namespace sl
{
namespace parser
{

/**
 *  @brief
 *    gaussian blur filter
 */
class GaussianBlurHori : public Filter
{
public:
	GaussianBlurHori() 
		: Filter("_col_gaussian_blur_") 
	{
		m_uniforms.push_back(new Uniform(VT_FLOAT1, "tex_width"));
	}
	
	virtual CU_STR& GetHeader(CU_STR& str) const { 
		str += gaussian_blur_hori_header;
		return str; 
	}

protected:
	virtual CU_STR GetBody() const {
		return gaussian_blur_hori_body;
	}

}; // GaussianBlurHori

}
}

#endif // _SHADERLAB_PARSER_BLUR_HORI_H_