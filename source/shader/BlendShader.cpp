#include "shaderlab/BlendShader.h"
#include "shaderlab/SubjectMVP2.h"
#include "shaderlab/Utility.h"
#include "shaderlab/ShaderType.h"
#include "shaderlab/ShaderMgr.h"
#include "shaderlab/RenderBuffer.h"
#include "shaderlab/RenderShader.h"
#include "shaderlab/AttributeNode.h"
#include "shaderlab/VaryingNode.h"
#include "shaderlab/PositionTrans.h"
#include "shaderlab/TextureMap.h"
#include "shaderlab/ColorAddMul.h"
#include "shaderlab/Blend.h"
#include "shaderlab/FragColor.h"
#include "shaderlab/RenderContext.h"

#include <unirender/RenderContext.h>
#include <unirender/Blackboard.h>

namespace sl
{

static const int MAX_COMMBINE = 1024;

BlendShader::BlendShader(RenderContext& rc)
	: Shader(rc)
{
	ur::Blackboard::Instance()->GetRenderContext().SetClearFlag(ur::MASKC);

	m_color = 0xffffffff;
	m_additive = 0x00000000;

	m_curr_mode = BM_NULL;

	m_tex_blend = m_tex_base = 0;

	m_quad_sz = 0;

	InitVAList();
	InitProg();

	m_vertex_buf = new Vertex[MAX_COMMBINE * 4];
}

BlendShader::~BlendShader()
{
	delete m_prog;
}

void BlendShader::Bind() const
{
	m_rc.GetShaderMgr().BindRenderShader(m_prog->GetShader(), BLEND);
}

void BlendShader::UnBind() const
{
}

bool BlendShader::Commit() const
{
	if (m_tex_blend == 0 || m_tex_base == 0) {
		return false;
	}

	auto& ur_rc = ur::Blackboard::Instance()->GetRenderContext();
	ur_rc.BindTexture(m_tex_blend, 0, false);
	ur_rc.BindTexture(m_tex_base, 1, false);

	RenderShader* shader = m_prog->GetShader();
	m_rc.GetShaderMgr().BindRenderShader(shader);
	shader->Draw(m_vertex_buf, m_quad_sz * 4, nullptr, m_quad_sz * 6);
	m_quad_sz = 0;

	return shader->Commit();
}

void BlendShader::SetColor(uint32_t color, uint32_t additive)
{
	m_color = color;
	m_additive = additive;
}

void BlendShader::SetMode(int mode)
{
	if (mode != m_curr_mode) {
		Commit();
	}
	m_curr_mode = (SL_BLEND_MODE)mode;
	m_prog->SetMode(mode);
}

void BlendShader::Draw(const float* positions, const float* texcoords_blend,
					   const float* texcoords_base, int tex_blend, int tex_base) const
{
	if (m_quad_sz >= MAX_COMMBINE ||
		(m_tex_blend != tex_blend && m_tex_blend != 0) ||
		(m_tex_base != tex_base && m_tex_base != 0)) {
		Commit();
	}
	m_tex_blend = tex_blend;
	m_tex_base = tex_base;

	for (int i = 0; i < 4; ++i)
	{
		Vertex* v	= &m_vertex_buf[m_quad_sz * 4 + i];
		v->vx		= positions[i * 2];
		v->vy		= positions[i * 2 + 1];
		v->tx_blend = texcoords_blend[i * 2];
		v->ty_blend = texcoords_blend[i * 2 + 1];
		v->tx_base	= texcoords_base[i * 2];
		v->ty_base	= texcoords_base[i * 2 + 1];
		v->color	= m_color;
		v->additive = m_additive;
	}
	++m_quad_sz;
}

void BlendShader::InitVAList()
{
	m_va_list[POSITION].Assign("position", 2, sizeof(float), 32, 0);
	m_va_list[TEXCOORD].Assign("texcoord", 2, sizeof(float), 32, 8);
	m_va_list[TEXCOORD_BASE].Assign("texcoord_base", 2, sizeof(float), 32, 16);
	m_va_list[COLOR].Assign("color", 4, sizeof(uint8_t), 32, 24);
	m_va_list[ADDITIVE].Assign("additive", 4, sizeof(uint8_t), 32, 28);
}

void BlendShader::InitProg()
{
 	CU_VEC<ur::VertexAttrib> va_list;
 	va_list.push_back(m_va_list[POSITION]);
 	va_list.push_back(m_va_list[TEXCOORD]);
 	va_list.push_back(m_va_list[TEXCOORD_BASE]);
 	va_list.push_back(m_va_list[COLOR]);
 	va_list.push_back(m_va_list[ADDITIVE]);

	auto& ur_rc = ur::Blackboard::Instance()->GetRenderContext();
	auto idx_buf = Utility::CreateQuadIndexBuffer(ur_rc, MAX_COMMBINE);
	m_prog = new Program(m_rc, va_list, idx_buf);
}

/************************************************************************/
/* class BlendShader::Program                                           */
/************************************************************************/

BlendShader::Program::Program(RenderContext& rc,
	                          const CU_VEC<ur::VertexAttrib>& va_list,
	                          const std::shared_ptr<RenderBuffer>& ib)
	: ShaderProgram(rc, MAX_COMMBINE * 4)
{
	Init(va_list, ib);

	rc.GetSubMVP2().Register(GetMVP());

	m_shader->SetDrawMode(ur::DRAW_TRIANGLES);

	m_mode = m_shader->AddUniform("u_mode", ur::UNIFORM_INT1);
	int tex0 = m_shader->AddUniform("u_texture0", ur::UNIFORM_INT1);
	if (tex0 >= 0) {
		float sample = 0;
		m_shader->SetUniform(tex0, ur::UNIFORM_INT1, &sample);
	}
	int tex1 = m_shader->AddUniform("u_texture1", ur::UNIFORM_INT1);
	if (tex1 >= 0) {
		float sample = 1;
		m_shader->SetUniform(tex1, ur::UNIFORM_INT1, &sample);
	}
}

void BlendShader::Program::SetMode(int mode)
{
	float m = static_cast<float>(mode);
	m_shader->SetUniform(m_mode, ur::UNIFORM_INT1, &m);
}

void BlendShader::Program::Init(const CU_VEC<ur::VertexAttrib>& va_list,
	                            const std::shared_ptr<RenderBuffer>& ib)
{
	parser::Node* vert = new parser::PositionTrans();
	vert->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT2, "texcoord")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT2, "texcoord")))->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT2, "texcoord_base")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT2, "texcoord_base")))->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT4, "color")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, "color")))->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT4, "additive")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, "additive")));

	parser::Node* frag = new parser::VaryingNode(parser::Variable(parser::VT_FLOAT2, "texcoord_base"));
	frag->Connect(
		new parser::TextureMap())->Connect(
		new parser::ColorAddMul())->Connect(
		new parser::Blend())->Connect(
		new parser::FragColor());

	Load(vert, frag, va_list, ib, true);
}

}