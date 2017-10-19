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

	virtual CU_STR& ToStatements(CU_STR& str) const;

	virtual Variable GetOutput() const;

};  // TextureMap

}
}

#endif // _SHADERLAB_PARSER_TEXTURE_MAP_H_