#ifndef _SHADERLAB_PARSER_MUL1_H_
#define _SHADERLAB_PARSER_MUL1_H_

#include "shaderlab/Node.h"

#include <stdio.h>

namespace sl
{
namespace parser
{

class Mul1 : public Node
{
public:
	Mul1(Variable left, Variable right)
		: m_left(left), m_right(right) {}

	virtual CU_STR& ToStatements(CU_STR& str) const override {
		CheckType(m_left, m_right);
		char buf[128];
		sprintf(buf, "%s *= %s;\n", m_left.GetName(), m_right.GetName());
		str += buf;
		return str;
	}

	virtual Variable GetOutput() const override {
		return m_left;
	}

private:
	Variable m_left, m_right;

}; // Mul1

}
}

#endif // _SHADERLAB_PARSER_MUL1_H_