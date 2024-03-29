#ifndef _SHADERLAB_PARSER_VERT_POSITION_H_
#define _SHADERLAB_PARSER_VERT_POSITION_H_

#include "shaderlab/Node.h"

namespace sl
{
namespace parser
{

class VertPosition : public Node
{
public:
	virtual Variable GetOutput() const override {
		return Variable(VT_FLOAT4, "gl_Position");
	}

}; // VertPosition

}
}

#endif // _SHADERLAB_PARSER_VERT_POSITION_H_