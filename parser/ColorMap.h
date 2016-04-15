#ifndef _SHADERLAB_PARSER_COLOR_MAP_H_
#define _SHADERLAB_PARSER_COLOR_MAP_H_

#include "Node.h"

namespace sl
{
namespace parser
{

class ColorMap : public Node
{
public:
	ColorMap();

	virtual std::string& GetHeader(std::string& str) const;
	virtual std::string& ToStatements(std::string& str) const;
	
	virtual std::string OutputName() const { return "col_map"; }

}; // ColorMap

}
}

#endif // _SHADERLAB_PARSER_COLOR_MAP_H_