#ifndef _SHADERLAB_PARSER_COLOR_STATIC_H_
#define _SHADERLAB_PARSER_COLOR_STATIC_H_

#include "Node.h"

namespace sl
{
namespace parser
{

class ColorStatic : public Node
{
public:
	ColorStatic(float r, float g, float b, float a);

	virtual std::string& ToStatements(std::string& str) const;
	
	virtual std::string OutputName() const;

private:
	float m_r, m_g, m_b, m_a;

}; // ColorStatic

}
}

#endif // _SHADERLAB_PARSER_COLOR_STATIC_H_