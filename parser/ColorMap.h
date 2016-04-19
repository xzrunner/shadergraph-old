#ifndef _SHADERLAB_PARSER_COLOR_MAP_H_
#define _SHADERLAB_PARSER_COLOR_MAP_H_

#include "Node.h"

namespace sl
{
namespace parser
{

// varying vec4 v_rmap
// varying vec4 v_gmap
// varying vec4 v_bmap
class ColorMap : public Node
{
public:
	ColorMap();

	virtual std::string& GetHeader(std::string& str) const;
	virtual std::string& ToStatements(std::string& str) const;
	
	virtual Variable GetOutput() const;

}; // ColorMap

}
}

#endif // _SHADERLAB_PARSER_COLOR_MAP_H_