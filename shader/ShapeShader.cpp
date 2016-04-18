#include "ShapeShader.h"
#include "ShaderProgram.h"
#include "render/RenderContext.h"
#include "render/RenderShader.h"
#include "parser/PositionTrans.h"
#include "parser/ColorOne.h"
#include "parser/FragColor.h"

namespace sl
{

ShapeShader::ShapeShader(RenderContext* rc)
	: Shader(rc)
	, m_prog(NULL)
	, m_color(0xffffffff)
{
	m_rc->SetClearFlag(MASKC);	
}

ShapeShader::~ShapeShader()
{
	delete m_prog;
}

void ShapeShader::Bind() const
{
	m_rc->BindShader(m_prog->GetShader());
}

void ShapeShader::UnBind() const
{
}

void ShapeShader::Commit() const
{
	m_prog->GetShader()->Commit();
}

void ShapeShader::SetColor(uint32_t color)
{
	m_color = color;
}

void ShapeShader::SetType(int type)
{
	m_prog->GetShader()->SetDrawMode((DRAW_MODE)type);
}

void ShapeShader::InitProg(int position_sz, int max_vertex)
{
	m_prog = new ShaderProgram(m_rc, max_vertex);

	parser::Node* vert = new parser::PositionTrans();
	parser::Node* frag = new parser::ColorOne();
	frag->Connect(new parser::FragColor());

	std::vector<VertexAttrib> va_list;
	va_list.push_back(VertexAttrib("position", position_sz, sizeof(float)));
	va_list.push_back(VertexAttrib("color", 4, sizeof(uint8_t)));

	m_prog->Load(vert, frag, va_list, NULL, true);

	InitMVP(m_prog->GetMVP());
}

} 