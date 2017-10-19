#ifndef _SHADERLAB_PARSER_SHADER_H_
#define _SHADERLAB_PARSER_SHADER_H_

#include "IOType.h"

#include <cu/cu_stl.h>

namespace sl
{
namespace parser
{

class Node;
class Uniform;
class Attribute;
class Variable;

class Shader
{
public:
	Shader(const Node* vert, const Node* frag);
	~Shader();

	const char* GetVertStr() const { return m_vert_str.c_str(); }
	const char* GetFragStr() const { return m_frag_str.c_str(); }

private:
	void ParserVert();
	void ParserFrag();

	static void GetVariables(const Node* head, IOType type, CU_VEC<const Variable*>& variables);

	static void ReleaseNodes(const Node* head);

private:
	const Node *m_vert_head, *m_frag_head;

	CU_STR m_vert_str, m_frag_str;

	CU_VEC<const Variable*> m_vert_uniforms, m_frag_uniforms;
	CU_VEC<const Variable*> m_attributes;

}; // Shader

}
}

#endif // _SHADERLAB_PARSER_SHADER_H_