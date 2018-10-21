#ifndef _SHADERLAB_PARSER_POSITION_TRANS_H_
#define _SHADERLAB_PARSER_POSITION_TRANS_H_

#include "shaderlab/Node.h"

namespace sl
{
namespace parser
{

/**
 *  @brief
 *    vertex position
 *
 *  @remarks
 *    input: attribute vec4 position;
 *           uniform mat4 u_projection;
 *           uniform mat4 u_modelview;
 *
 *    output: gl_Position
 */
class PositionTrans : public Node
{
public:
	PositionTrans();

	virtual CU_STR& ToStatements(CU_STR& str) const override;

	virtual Variable GetOutput() const override { return Variable(VT_FLOAT4, "gl_Position"); }

}; // PositionTrans

}
}

#endif // _SHADERLAB_PARSER_POSITION_TRANS_H_