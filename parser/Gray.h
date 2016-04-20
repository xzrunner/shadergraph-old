#ifndef _SHADERLAB_PARSER_EDGE_DETECT_H_
#define _SHADERLAB_PARSER_EDGE_DETECT_H_

#include "Filter.h"

namespace sl
{
namespace parser
{

/**
 *  @brief
 *    gray
 */
class Gray : public Filter
{
public:
	Gray() : Filter("_col_gray_") {}

protected:
	virtual const char* GetBody() const {
		return "\
			vec4 tmp = texture2D(u_texture0, v_texcoord);\n\
			float y = dot(tmp.rgb , vec3(0.299, 0.587, 0.114));\n\
			vec4 _DST_COL_ = vec4(y, y, y, tmp.a);\n";
	}

}; // EdgeDetect

}
}

#endif // _SHADERLAB_PARSER_EDGE_DETECT_H_