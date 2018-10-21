#ifndef _SHADERLAB_PARSER_COLOR_MAP_H_
#define _SHADERLAB_PARSER_COLOR_MAP_H_

#include "shaderlab/Node.h"

namespace sl
{
namespace parser
{

/**
 *  @brief
 *    transform each r, g, b
 *
 *  @remarks
 *    input: varying vec4 v_rmap;
 *           varying vec4 v_gmap;
 *           varying vec4 v_bmap;
 *
 *    output: gl_Position
 */
class ColorMap : public Node
{
public:
	ColorMap();

	virtual CU_STR& GetHeader(CU_STR& str) const override;
	virtual CU_STR& ToStatements(CU_STR& str) const override;

	virtual Variable GetOutput() const override;

}; // ColorMap

}
}

#endif // _SHADERLAB_PARSER_COLOR_MAP_H_