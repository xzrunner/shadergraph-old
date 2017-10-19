#ifndef _SHADERLAB_PARSER_NODE_H_
#define _SHADERLAB_PARSER_NODE_H_

#include "IOType.h"
#include "Variable.h"

#include <cu/cu_stl.h>

namespace sl
{
namespace parser
{

class Attribute;
class Varying;
class Uniform;

class Node
{
public:
	Node();
	virtual ~Node();

	virtual CU_STR& GetHeader(CU_STR& str) const { return str; }
	virtual CU_STR& ToStatements(CU_STR& str) const { return str; }

	virtual Variable GetOutput() const = 0;

	Node* Connect(Node* next);

	void GetVariables(IOType type, CU_VEC<const Variable*>& variables) const;

	const Node* Next() const { return m_output; }

protected:
	static void CheckType(const Variable& left, const Variable& right);
	static void CheckType(const Variable& var, VariableType type);

protected:
	Node *m_input, *m_output;

	CU_VEC<Attribute*> m_attributes;
	CU_VEC<Varying*> m_varyings;
	CU_VEC<Uniform*> m_uniforms;

	//CU_STR m_type;
	//CU_STR m_name;

}; // Node



}
}

#endif // _SHADERLAB_PARSER_NODE_H_