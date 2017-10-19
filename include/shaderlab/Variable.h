#ifndef _SHADERLAB_PARSER_VARIABLE_H_
#define _SHADERLAB_PARSER_VARIABLE_H_

#include "VariableType.h"

#include <cu/cu_stl.h>

namespace sl
{
namespace parser
{

class Variable
{
public:
	Variable(VariableType type) 
		: m_type(type) {}
	Variable(VariableType type, CU_STR name) 
		: m_type(type), m_name(name) {}
	Variable(const Variable& var)
		: m_type(var.m_type), m_name(var.m_name) {}
	virtual ~Variable() {}

	virtual CU_STR& ToStatement(CU_STR& str) const { return str; }

	VariableType GetType() const { return m_type; }
	const char* GetName() const { return m_name.c_str(); }

	void SetName(const CU_STR& name) { m_name = name; }

protected:
	VariableType m_type;
	CU_STR m_name;

}; // Variable

}
}

#endif // _SHADERLAB_PARSER_VARIABLE_H_