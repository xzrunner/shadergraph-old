#ifndef _SHADERLAB_PARSER_BLEND_H_
#define _SHADERLAB_PARSER_BLEND_H_

#include "shaderlab/Node.h"

namespace sl
{
namespace parser
{

/**
 *  @brief
 *    layer blend
 */
class Blend : public Node
{
public:
	Blend();

	virtual CU_STR& GetHeader(CU_STR& str) const override;
	virtual CU_STR& ToStatements(CU_STR& str) const override;

	virtual Variable GetOutput() const override;

}; // Blend

}
}

#endif // _SHADERLAB_PARSER_BLEND_H_