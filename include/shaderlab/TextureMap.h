#ifndef _SHADERLAB_PARSER_TEXTURE_MAP_H_
#define _SHADERLAB_PARSER_TEXTURE_MAP_H_

#include "shaderlab/Node.h"

namespace sl
{
namespace parser
{

class TextureMap : public Node
{
public:
	TextureMap();

	virtual CU_STR& ToStatements(CU_STR& str) const override;

	virtual Variable GetOutput() const override;

};  // TextureMap

}
}

#endif // _SHADERLAB_PARSER_TEXTURE_MAP_H_