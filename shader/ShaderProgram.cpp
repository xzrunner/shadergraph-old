#include "ShaderProgram.h"
#include "ObserverMVP.h"
#include "parser/Shader.h"
#include "render/RenderContext.h"
#include "render/RenderLayout.h"
#include "render/RenderShader.h"
#include "render/RenderBuffer.h"
#include "utility/Buffer.h"

namespace sl
{

ShaderProgram::ShaderProgram(RenderContext* rc, int max_vertex)
	: m_rc(rc)
	, m_max_vertex(max_vertex)
	, m_parser(NULL)
	, m_shader(NULL)
	, m_vertex_sz(0)
	, m_mvp(0)
{
}

ShaderProgram::~ShaderProgram()
{
	Release();
}

void ShaderProgram::Load(parser::Node* vert, parser::Node* frag, 
						 const std::vector<VertexAttrib>& va_list,
						 RenderBuffer* ib, bool has_mvp)
{
	// shader
	m_parser = new parser::Shader(vert, frag);
	m_shader = m_rc->CreateShader();
	
	// vertex layout
	RenderLayout* lo = new RenderLayout(m_rc->GetEJRender(), va_list);
	m_shader->SetLayout(lo);
	lo->Release();

	// vertex buffer
	m_vertex_sz = 0;
	for (int i = 0, n = va_list.size(); i < n; ++i) {
		m_vertex_sz += va_list[i].tot_size;
	}
	Buffer* buf = new Buffer(m_vertex_sz, m_max_vertex);
	RenderBuffer* vb = new RenderBuffer(m_rc->GetEJRender(), VERTEXBUFFER, m_vertex_sz, m_max_vertex, buf);
	m_shader->SetVertexBuffer(vb);
	vb->Release();

	// index buffer
	if (ib) {
		m_shader->SetIndexBuffer(ib);
	}

	// final
	m_shader->Load(m_parser->GetVertStr(), m_parser->GetFragStr());

	// uniforms
	m_mvp = new ObserverMVP(m_shader);
	m_mvp->InitModelview(m_shader->AddUniform("u_modelview", UNIFORM_FLOAT44));
	m_mvp->InitProjection(m_shader->AddUniform("u_projection", UNIFORM_FLOAT44));
}

void ShaderProgram::Release()
{
	delete m_parser;
	m_shader->Unload();
	delete m_shader;
	if (m_mvp) {
		delete m_mvp;
	}
}

}