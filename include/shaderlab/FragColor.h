#ifndef _SHADERLAB_PARSER_FRAG_COLOR_H_
#define _SHADERLAB_PARSER_FRAG_COLOR_H_

#include "shaderlab/Node.h"

#include <stdio.h>

namespace sl
{
namespace parser
{

class FragColor : public Node
{
public:
	virtual CU_STR& ToStatements(CU_STR& str) const override {
		if (m_input) {
			char buf[128];
			sprintf(buf, "gl_FragColor = %s;\n", m_input->GetOutput().GetName());
			str += buf;
		}
		return str;
	}

	virtual Variable GetOutput() const override { return Variable(VT_FLOAT4, "gl_FragColor"); }

}; // FragColor

}
}

#endif // _SHADERLAB_PARSER_FRAG_COLOR_H_