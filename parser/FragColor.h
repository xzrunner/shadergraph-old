#ifndef _SHADERLAB_PARSER_FRAG_COLOR_H_
#define _SHADERLAB_PARSER_FRAG_COLOR_H_

#include "Node.h"

namespace sl
{
namespace parser
{

class FragColor : public Node
{
public:
	virtual std::string& ToStatements(std::string& str) const {
		if (m_input) {
			str += "gl_FragColor = " + m_input->OutputName() + ";"
		}
		return str;
	}

	virtual std::string OutputName() const { return "gl_FragColor"; }

}; // FragColor

}
}

#endif // _SHADERLAB_PARSER_FRAG_COLOR_H_