#include "shaderlab/ParserShader.h"
#include "shaderlab/Node.h"
#include "shaderlab/Attribute.h"
#include "shaderlab/Varying.h"
#include "shaderlab/Uniform.h"

namespace sl
{
namespace parser
{

static const int SHADER_MAX_STR_LEN = 1024;

Shader::Shader(const Node* vert, const Node* frag)
	: m_vert_head(vert)
	, m_frag_head(frag)
{
	ParserVert();
	ParserFrag();
}

Shader::~Shader()
{
	ReleaseNodes(m_vert_head);
	ReleaseNodes(m_frag_head);
}

void Shader::ParserVert()
{
	m_vert_str.clear();
	m_vert_str.reserve(SHADER_MAX_STR_LEN);

	CU_VEC<const Variable*> varyings;
	GetVariables(m_vert_head, IOT_ATTRIBUTE, m_attributes);
	GetVariables(m_vert_head, IOT_VARYING, varyings);
	GetVariables(m_vert_head, IOT_UNIFORM, m_vert_uniforms);

	for (int i = 0, n = m_attributes.size(); i < n; ++i) {
		const Variable* attr = m_attributes[i];
		attr->ToStatement(m_vert_str);
	}
	for (int i = 0, n = varyings.size(); i < n; ++i) {
		varyings[i]->ToStatement(m_vert_str);
	}
	for (int i = 0, n = m_vert_uniforms.size(); i < n; ++i) {
		m_vert_uniforms[i]->ToStatement(m_vert_str);
	}

	const Node* node = m_vert_head;
	while (node) {
		node->GetHeader(m_vert_str);
		node = node->Next();
	}

	m_vert_str += "void main() \n{\n";

	node = m_vert_head;
	while (node) {
		node->ToStatements(m_vert_str);
		node = node->Next();
	}

	m_vert_str += "}\n";
}

void Shader::ParserFrag()
{
	CU_VEC<const Variable*> varyings;
	GetVariables(m_frag_head, IOT_VARYING, varyings);
	GetVariables(m_frag_head, IOT_UNIFORM, m_frag_uniforms);

	for (int i = 0, n = varyings.size(); i < n; ++i) {
		varyings[i]->ToStatement(m_frag_str);
	}
	for (int i = 0, n = m_frag_uniforms.size(); i < n; ++i) {
		m_frag_uniforms[i]->ToStatement(m_frag_str);
	}

	const Node* node = m_frag_head;
	while (node) {
		node->GetHeader(m_frag_str);
		node = node->Next();
	}

	m_frag_str += "void main() \n{\n";

	node = m_frag_head;
	while (node) {
		node->ToStatements(m_frag_str);
		node = node->Next();
	}

	m_frag_str += "}";
}

void Shader::GetVariables(const Node* head, IOType type, CU_VEC<const Variable*>& variables)
{
	const Node* node = head;
	while (node) {
		node->GetVariables(type, variables);
		node = node->Next();
	}
}

void Shader::ReleaseNodes(const Node* head)
{
	const Node *curr = head, *next = curr->Next();
	while (curr) {
		delete curr;
		if (!next) {
			break;
		}
		curr = next;
		next = curr->Next();
	}
}

}
}