#include "shaderlab/ShapeShader.h"
#include "shaderlab/ShaderProgram.h"
#include "shaderlab/ShaderType.h"
#include "shaderlab/ShaderMgr.h"
#include "shaderlab/RenderShader.h"
#include "shaderlab/PositionTrans.h"
#include "shaderlab/VaryingNode.h"
#include "shaderlab/FragColor.h"
#include "shaderlab/AttributeNode.h"

#include <unirender/RenderContext.h>

namespace sl
{

ShapeShader::ShapeShader(ShaderMgr& shader_mgr)
	: Shader(shader_mgr)
	, m_prog(nullptr)
	, m_color(0xffffffff)
{
}

ShapeShader::~ShapeShader()
{
	delete m_prog;
}

void ShapeShader::Bind() const
{
	m_shader_mgr.BindRenderShader(m_prog->GetShader(), SHAPE2);
}

void ShapeShader::UnBind() const
{
}

bool ShapeShader::Commit() const
{
	return m_prog->GetShader()->Commit();
}

void ShapeShader::SetColor(uint32_t color)
{
	m_color = color;
}

void ShapeShader::SetType(int type)
{
	m_prog->GetShader()->SetDrawMode((ur::DRAW_MODE)type);
}

void ShapeShader::InitProg(int position_sz, int max_vertex)
{
	m_prog = new ShaderProgram(m_shader_mgr, max_vertex);

	parser::Node* vert = new parser::PositionTrans();
	vert->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT4, "color")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, "color")));

	parser::Node* frag = new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, "color"));
	frag->Connect(new parser::FragColor());

	CU_VEC<ur::VertexAttrib> va_list;
	va_list.push_back(ur::VertexAttrib("position", position_sz, sizeof(float)));
	va_list.push_back(ur::VertexAttrib("color", 4, sizeof(uint8_t)));

	m_prog->Load(vert, frag, va_list, nullptr, true);

	InitMVP(m_prog->GetMVP());
}

} 