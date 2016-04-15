#ifndef _SHADERLAB_PARSER_COLOR_ADD_MULTI_H_
#define _SHADERLAB_PARSER_COLOR_ADD_MULTI_H_

#include "Node.h"

namespace sl
{
namespace parser
{

class ColorAddMulti : public Node
{
public:
	ColorAddMulti();

	virtual std::string& ToStatements(std::string& str) const;
	
	virtual std::string OutputName() const { return "col_add_multi"; }

}; // ColorAddMulti

}
}

#endif // _SHADERLAB_PARSER_COLOR_ADD_MULTI_H_