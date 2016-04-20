#ifndef _SHADERLAB_PARSER_BLUR_H_
#define _SHADERLAB_PARSER_BLUR_H_

#include "Filter.h"

#define STRINGIFY(A)  #A
#include "gaussian_blur.frag"

namespace sl
{
namespace parser
{

/**
 *  @brief
 *    gaussian blur filter
 */
class GaussianBlur : public Filter
{
public:
	GaussianBlur() : Filter("_col_gaussian_blur_") {}
	
	virtual std::string& GetHeader(std::string& str) const { 
		str += gaussian_blur_header;
		return str; 
	}

protected:
	virtual const char* GetBody() const {
		return gaussian_blur_body;
	}

}; // GaussianBlur

}
}

#endif // _SHADERLAB_PARSER_BLUR_H_