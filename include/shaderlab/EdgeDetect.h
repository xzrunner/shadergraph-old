#ifndef _SHADERLAB_PARSER_EDGE_DETECT_H_
#define _SHADERLAB_PARSER_EDGE_DETECT_H_

#include "shaderlab/Filter.h"
#include "shaderlab/ParserUniform.h"

#define STRINGIFY(A)  #A
#include "shaderlab/edge_detect.frag"

namespace sl
{
namespace parser
{

/**
 *  @brief
 *    edge detect
 *
 *  @remarks
 *    input: uniform float u_blend
 */
class EdgeDetect : public Filter
{
public:
	EdgeDetect() : Filter("_col_edge_detect_") {
		m_uniforms.push_back(new Uniform(VT_FLOAT1, "blend"));
	}

protected:
	virtual CU_STR GetBody() const {
		return edge_detect_body;
	}

}; // EdgeDetect

}
}

#endif // _SHADERLAB_PARSER_EDGE_DETECT_H_