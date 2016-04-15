#ifndef _SHADERLAB_PARSER_TEXTURE_MAP_H_
#define _SHADERLAB_PARSER_TEXTURE_MAP_H_

#include "Node.h"

namespace sl
{
namespace parser
{

class TextureMap : public Node
{
public:
	TextureMap();

	virtual std::string& ToStatements(std::string& str) const;

	virtual std::string OutputName() const { return "src"; }

};  // TextureMap

}
}

#endif // _SHADERLAB_PARSER_TEXTURE_MAP_H_