#include "ShapeShader.h"
#include "ObserverMVP.h"
#include "render/RenderContext.h"
#include "render/RenderBuffer.h"
#include "render/RenderShader.h"
#include "render/RenderLayout.h"
#include "parser/PositionTrans.h"
#include "parser/ColorOne.h"
#include "parser/FragColor.h"
#include "parser/Shader.h"

namespace sl
{

ShapeShader::ShapeShader(RenderContext* rc)
	: Shader(rc)
	, m_shader(NULL)
	, m_vertex_sz(0)
	, m_color(0xffffffff)
	, m_parser(NULL)
	, m_mvp(NULL)
{
	m_rc->SetClearFlag(MASKC);	
}

ShapeShader::~ShapeShader()
{
	delete m_parser;

	m_shader->Unload();
	delete m_shader;

	delete m_mvp;
}

void ShapeShader::Bind() const
{
	m_rc->BindShader(m_shader);
}

void ShapeShader::UnBind() const
{
}

void ShapeShader::Commit() const
{
	m_shader->Commit();
}

void ShapeShader::SetColor(uint32_t color)
{
	m_color = color;
}

void ShapeShader::SetType(int type)
{
	m_shader->SetDrawMode((DRAW_MODE)type);
}

void ShapeShader::InitProg(int position_sz, int max_vertex)
{
	// shader
	parser::Node* vert = new parser::PositionTrans();
	parser::Node* frag = new parser::ColorOne();
	frag->Connect(new parser::FragColor());
	m_parser = new parser::Shader(vert, frag);

	m_shader = m_rc->CreateShader();

	// vertex layout
	std::vector<VertexAttrib> va_list;
	va_list.push_back(VertexAttrib("position", position_sz, sizeof(float)));
	va_list.push_back(VertexAttrib("color", 4, sizeof(uint8_t)));
	RenderLayout* lo = new RenderLayout(m_rc->GetEJRender(), va_list);
	m_shader->SetLayout(lo);
	lo->Release();

	// vertex buffer
	m_vertex_sz = 0;
	for (int i = 0, n = va_list.size(); i < n; ++i) {
		m_vertex_sz += va_list[i].tot_size;
	}
	Buffer* buf = new Buffer(m_vertex_sz, max_vertex);
	RenderBuffer* vb = new RenderBuffer(m_rc->GetEJRender(), VERTEXBUFFER, m_vertex_sz, max_vertex, buf);
	m_shader->SetVertexBuffer(vb);
	vb->Release();

	// create
	m_shader->Load(m_parser->GetVertStr(), m_parser->GetFragStr());

	// uniforms
	m_mvp = new ObserverMVP(m_shader);
	m_mvp->InitModelview(m_shader->AddUniform("u_modelview", UNIFORM_FLOAT44));
	m_mvp->InitProjection(m_shader->AddUniform("u_projection", UNIFORM_FLOAT44));
	InitMVP(m_mvp);
}

} 