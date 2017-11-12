#ifndef _SHADERLAB_PARSER_FILTER_H_
#define _SHADERLAB_PARSER_FILTER_H_

#include "shaderlab/Node.h"

namespace sl
{
namespace parser
{

/**
 *  @brief
 *    frag filter
 *
 *  @remarks
 *    type:    frag
 *    input:   varying vec2 v_texcoord
 *             uniform sampler2D texture0
 *    output:  vec4 color
 *    prev: TextureMap
 */
class Filter : public Node
{
public:
	Filter(const CU_STR& output) : m_output(output) {}
	
	virtual CU_STR& ToStatements(CU_STR& str) const override;

	virtual Variable GetOutput() const override {
		return Variable(VT_FLOAT4, m_output);
	}

protected:
	virtual CU_STR GetBody() const = 0;

private:
	CU_STR m_output;

}; // Filter

}
}

#endif // _SHADERLAB_PARSER_FILTER_H_