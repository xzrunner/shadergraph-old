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
	virtual std::string GetBody() const;

}; // EdgeDetect

}
}

#endif // _SHADERLAB_PARSER_EDGE_DETECT_H_