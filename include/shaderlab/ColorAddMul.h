#ifndef _SHADERLAB_PARSER_COLOR_ADD_MUL_H_
#define _SHADERLAB_PARSER_COLOR_ADD_MUL_H_

#include "shaderlab/Node.h"

namespace sl
{
namespace parser
{

/**
 *  @brief
 *    add and mul
 *
 *  @remarks
 *    input: varying vec4 v_color;
 *           varying vec4 v_additive;
 *    
 *    output: gl_Position
 */
class ColorAddMul : public Node
{
public:
	ColorAddMul();

	virtual CU_STR& ToStatements(CU_STR& str) const;
	
	virtual Variable GetOutput() const;

}; // ColorAddMul

}
}

#endif // _SHADERLAB_PARSER_COLOR_ADD_MUL_H_