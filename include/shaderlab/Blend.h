#ifndef _SHADERLAB_PARSER_BLEND_H_
#define _SHADERLAB_PARSER_BLEND_H_

#include "Node.h"

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

	virtual CU_STR& GetHeader(CU_STR& str) const;
	virtual CU_STR& ToStatements(CU_STR& str) const;
	
	virtual Variable GetOutput() const;

}; // Blend

}
}

#endif // _SHADERLAB_PARSER_BLEND_H_