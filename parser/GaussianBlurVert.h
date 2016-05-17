#ifndef _SHADERLAB_PARSER_BLUR_VERT_H_
#define _SHADERLAB_PARSER_BLUR_VERT_H_

#include "Filter.h"

#define STRINGIFY(A)  #A
#include "gaussian_blur_vert.frag"

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
	GaussianBlurVert() : Filter("_col_gaussian_blur_") {}
	
	virtual std::string& GetHeader(std::string& str) const { 
		str += gaussian_blur_vert_header;
		return str; 
	}

protected:
	virtual const char* GetBody() const {
		return gaussian_blur_vert_body;
	}

}; // GaussianBlurVert

}
}

#endif // _SHADERLAB_PARSER_BLUR_VERT_H_