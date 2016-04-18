#ifndef _SHADERLAB_SHADER_PROGRAM_H_
#define _SHADERLAB_SHADER_PROGRAM_H_

#include "render/VertexAttrib.h"

#include <vector>

namespace sl
{

namespace parser { class Shader; class Node; }

class RenderShader;
class RenderContext;
class RenderBuffer;
class ObserverMVP;

class ShaderProgram
{
public:
	ShaderProgram(RenderContext* rc, int max_vertex);
	~ShaderProgram();

	void Load(parser::Node* vert, parser::Node* frag, 
		const std::vector<VertexAttrib>& va_list, 
		RenderBuffer* ib, bool has_mvp);

	RenderShader* GetShader() { return m_shader; }
	int GetVertexSize() const { return m_vertex_sz; }
	ObserverMVP* GetMVP() const { return m_mvp; }

private:
	void Release();

private:
	RenderContext* m_rc;
	int m_max_vertex;

	parser::Shader* m_parser;
	RenderShader* m_shader;
	int m_vertex_sz;

	ObserverMVP* m_mvp;

}; // ShaderProgram

}

#endif // _SHADERLAB_SHADER_PROGRAM_H_