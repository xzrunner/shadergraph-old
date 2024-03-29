#ifndef _SHADERLAB_PARSER_GRAY_H_
#define _SHADERLAB_PARSER_GRAY_H_

#include "shaderlab/Filter.h"

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
	virtual CU_STR GetBody() const override;

}; // Gray

}
}

#endif // _SHADERLAB_PARSER_GRAY_H_