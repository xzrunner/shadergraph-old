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
 *
 *  @remarks
 *    input: varying vec4 v_rmap;
 *           varying vec4 v_gmap;
 *           varying vec4 v_bmap;
 */
class Blend : public Node
{
public:
	Blend();

	virtual std::string& GetHeader(std::string& str) const;
	virtual std::string& ToStatements(std::string& str) const;
	
	virtual Variable GetOutput() const;

}; // Blend

}
}

#endif // _SHADERLAB_PARSER_BLEND_H_