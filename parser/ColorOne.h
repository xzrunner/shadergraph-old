#ifndef _SHADERLAB_PARSER_COLOR_ONE_H_
#define _SHADERLAB_PARSER_COLOR_ONE_H_

#include "Node.h"

namespace sl
{
namespace parser
{

// attribute vec4 color
class ColorOne : public Node
{
public:
	ColorOne();

	virtual std::string& ToStatements(std::string& str) const;
	
	virtual std::string OutputName() const;

}; // ColorOne

}
}

#endif // _SHADERLAB_PARSER_COLOR_ONE_H_