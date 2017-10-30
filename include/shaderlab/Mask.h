#ifndef _SHADERLAB_PARSER_MASK_H_
#define _SHADERLAB_PARSER_MASK_H_

#include "shaderlab/Node.h"

namespace sl
{
namespace parser
{

/**
 *  @brief
 *    layer mask
 */
class Mask : public Node
{
public:
	Mask();

	virtual CU_STR& ToStatements(CU_STR& str) const;

	virtual Variable GetOutput() const;

}; // Mask

}
}

#endif // _SHADERLAB_PARSER_MASK_H_