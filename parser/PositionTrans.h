#ifndef _SHADERLAB_PARSER_POSITION_TRANS_H_
#define _SHADERLAB_PARSER_POSITION_TRANS_H_

#include "Node.h"

namespace sl
{
namespace parser
{

class PositionTrans : public Node
{
public:
	PositionTrans();

	virtual std::string& ToStatements(std::string& str) const;
	
	virtual std::string OutputName() const { return "gl_Position"; }

}; // PositionTrans

}
}

#endif // _SHADERLAB_PARSER_POSITION_TRANS_H_