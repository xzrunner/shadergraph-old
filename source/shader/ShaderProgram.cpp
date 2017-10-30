#include "shaderlab/ShaderProgram.h"
#include "shaderlab/ShaderMgr.h"
#include "shaderlab/ObserverMVP.h"
#include "shaderlab/ParserShader.h"
#include "shaderlab/RenderLayout.h"
#include "shaderlab/RenderShader.h"
#include "shaderlab/RenderBuffer.h"
#include "shaderlab/Buffer.h"

#include <unirender/RenderContext.h>

namespace sl
{

ShaderProgram::ShaderProgram(ur::RenderContext* rc, int max_vertex)
	: m_rc(rc)
	, m_max_vertex(max_vertex)
	, m_parser(nullptr)
	, m_shader(nullptr)
	, m_vertex_sz(0)
	, m_mvp(0)
{
}

ShaderProgram::~ShaderProgram()
{
	Release();
}

void ShaderProgram::Load(parser::Node* vert, parser::Node* frag, 
						 const CU_VEC<ur::VertexAttrib>& va_list,
	                     const std::shared_ptr<RenderBuffer>& ib, bool has_mvp)
{
	// shader
	m_parser = new parser::Shader(vert, frag);
	m_shader = ShaderMgr::Instance()->CreateRenderShader();
	
	// vertex layout
	auto lo = std::make_shared<RenderLayout>(m_rc, va_list);
	m_shader->SetLayout(lo);

	// vertex buffer
	m_vertex_sz = 0;
	for (int i = 0, n = va_list.size(); i < n; ++i) {
		m_vertex_sz += va_list[i].tot_size;
	}
	Buffer* buf = new Buffer(m_vertex_sz, m_max_vertex);
	auto vb = std::make_shared<RenderBuffer>(m_rc, ur::VERTEXBUFFER, m_vertex_sz, m_max_vertex, buf);
	m_shader->SetVertexBuffer(vb);

	// index buffer
	if (ib) {
		m_shader->SetIndexBuffer(ib);
	}

	// final
	m_shader->Load(m_parser->GetVertStr(), m_parser->GetFragStr());

	// uniforms
	m_mvp = new ObserverMVP(m_shader);
	m_mvp->InitModelview(m_shader->AddUniform("u_modelview", ur::UNIFORM_FLOAT44));
	m_mvp->InitProjection(m_shader->AddUniform("u_projection", ur::UNIFORM_FLOAT44));
}

void ShaderProgram::Release()
{
	delete m_parser;
	m_shader->Unload();
	if (m_mvp) {
		delete m_mvp;
	}
}

}