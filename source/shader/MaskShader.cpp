#include "shaderlab/MaskShader.h"
#include "shaderlab/SubjectMVP2.h"
#include "shaderlab/Utility.h"
#include "shaderlab/ShaderType.h"
#include "shaderlab/ShaderMgr.h"
#include "shaderlab/RenderBuffer.h"
#include "shaderlab/RenderShader.h"
#include "shaderlab/Mask.h"
#include "shaderlab/AttributeNode.h"
#include "shaderlab/VaryingNode.h"
#include "shaderlab/PositionTrans.h"
#include "shaderlab/TextureMap.h"
#include "shaderlab/FragColor.h"
#include "shaderlab/RenderContext.h"

#include <unirender/RenderContext.h>
#include <unirender/Blackboard.h>

namespace sl
{

static const int MAX_COMMBINE = 128;

MaskShader::MaskShader(RenderContext& rc)
	: Shader(rc)
{
	ur::Blackboard::Instance()->GetRenderContext().SetClearFlag(ur::MASKC);

	m_tex = m_tex_mask = 0;

	m_quad_sz = 0;

	InitVAList();
	InitProg();

	m_vertex_buf = new Vertex[MAX_COMMBINE * 4];
}

MaskShader::~MaskShader()
{
	delete m_prog;
}

void MaskShader::Bind() const
{
	m_rc.GetShaderMgr().BindRenderShader(m_prog->GetShader(), MASK);
}

void MaskShader::UnBind() const
{
}

bool MaskShader::Commit() const
{
	auto& ur_rc = ur::Blackboard::Instance()->GetRenderContext();
	ur_rc.BindTexture(m_tex, 0);
	ur_rc.BindTexture(m_tex_mask, 1);

	RenderShader* shader = m_prog->GetShader();
	m_rc.GetShaderMgr().BindRenderShader(shader, MASK);
	shader->Draw(m_vertex_buf, m_quad_sz * 4, nullptr, m_quad_sz * 6);
	m_quad_sz = 0;

	return shader->Commit();
}

void MaskShader::Draw(const float* positions, const float* texcoords,
					  const float* texcoords_mask, int tex, int tex_mask) const
{
	if (m_quad_sz >= MAX_COMMBINE ||
		(m_tex != tex && m_tex != 0) ||
		(m_tex_mask != tex_mask && m_tex_mask != 0)) {
		Commit();
	}
	m_tex = tex;
	m_tex_mask = tex_mask;

	for (int i = 0; i < 4; ++i)
	{
		Vertex* v	= &m_vertex_buf[m_quad_sz * 4 + i];
		v->vx		= positions[i * 2];
		v->vy		= positions[i * 2 + 1];
		v->tx		= texcoords[i * 2];
		v->ty		= texcoords[i * 2 + 1];
		v->tx_mask	= texcoords_mask[i * 2];
		v->ty_mask	= texcoords_mask[i * 2 + 1];
	}
	++m_quad_sz;
}

void MaskShader::InitVAList()
{
	m_va_list[POSITION].Assign("position", 2, sizeof(float), 24, 0);
	m_va_list[TEXCOORD].Assign("texcoord", 2, sizeof(float), 24, 8);
	m_va_list[TEXCOORD_MASK].Assign("texcoord_mask", 2, sizeof(float), 24, 16);
}

void MaskShader::InitProg()
{
	CU_VEC<ur::VertexAttrib> va_list;
	va_list.push_back(m_va_list[POSITION]);
	va_list.push_back(m_va_list[TEXCOORD]);
	va_list.push_back(m_va_list[TEXCOORD_MASK]);

	auto& ur_rc = ur::Blackboard::Instance()->GetRenderContext();
	auto idx_buf = Utility::CreateQuadIndexBuffer(ur_rc, MAX_COMMBINE);
	m_prog = new Program(m_rc, va_list, idx_buf);
}

/************************************************************************/
/* class MaskShader::Program                                            */
/************************************************************************/

MaskShader::Program::Program(RenderContext& rc, const CU_VEC<ur::VertexAttrib>& va_list, const std::shared_ptr<RenderBuffer>& ib)
	: ShaderProgram(rc, MAX_COMMBINE * 4)
{
	Init(va_list, ib);

	rc.GetSubMVP2().Register(GetMVP());

	m_shader->SetDrawMode(ur::DRAW_TRIANGLES);

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

void MaskShader::Program::Init(const CU_VEC<ur::VertexAttrib>& va_list, const std::shared_ptr<RenderBuffer>& ib)
{
	parser::Node* vert = new parser::PositionTrans();
	vert->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT2, "texcoord")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT2, "texcoord")))->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT2, "texcoord_mask")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT2, "texcoord_mask")));

	parser::Node* frag = new parser::VaryingNode(parser::Variable(parser::VT_FLOAT2, "texcoord_mask"));
	frag->Connect(
		new parser::TextureMap())->Connect(
		new parser::Mask())->Connect(
		new parser::FragColor());

	Load(vert, frag, va_list, ib, true);
}

}