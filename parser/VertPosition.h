#ifndef _SHADERLAB_PARSER_VERT_POSITION_H_
#define _SHADERLAB_PARSER_VERT_POSITION_H_

#include "Node.h"

namespace sl
{
namespace parser
{

class VertPosition : public Node
{
public:
	virtual std::string OutputName() const { return "gl_Position"; }

}; // VertPosition

}
}

#endif // _SHADERLAB_PARSER_VERT_POSITION_H_