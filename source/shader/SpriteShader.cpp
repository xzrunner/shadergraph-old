#include "shaderlab/SpriteShader.h"
#include "shaderlab/ObserverMVP.h"
#include "shaderlab/Utility.h"
#include "shaderlab/ShaderProgram.h"
#include "shaderlab/ShaderMgr.h"
#include "shaderlab/RenderBuffer.h"
#include "shaderlab/RenderShader.h"
#include "shaderlab/RenderLayout.h"
#include "shaderlab/Shader.h"
#include "shaderlab/PositionTrans.h"
#include "shaderlab/AttributeNode.h"
#include "shaderlab/VaryingNode.h"
#include "shaderlab/TextureMap.h"
#include "shaderlab/FragColor.h"
#include "shaderlab/ColorAddMul.h"
#include "shaderlab/ColorMap.h"
#include "shaderlab/Buffer.h"
#include "shaderlab/RenderContext.h"

#include <unirender/RenderContext.h>
#include <unirender/Blackboard.h>

#include <memory>

namespace sl
{

SpriteShader::SpriteShader(RenderContext& rc, int position_sz,
                           int max_vertex, bool vertex_index)
	: Shader(rc)
	, m_max_vertex(max_vertex)
	, m_vertex_index(vertex_index)
{
	ur::Blackboard::Instance()->GetRenderContext().SetClearFlag(ur::MASKC);

	m_color = 0xffffffff;
	m_additive = 0x00000000;
	m_rmap = 0x000000ff;
	m_gmap = 0x0000ff00;
	m_bmap = 0x00ff0000;

	m_texid = 0;

	m_quad_sz = 0;

	m_prog_type = 0;

	InitVAList(position_sz);
}

SpriteShader::~SpriteShader()
{
	for (int i = 0; i < PROG_COUNT; ++i) {
		delete m_programs[i];
	}
}

void SpriteShader::Bind() const
{
	// todo bind PT_NO_COLOR ?
}

void SpriteShader::UnBind() const
{
//	Commit();

// 	ur::RenderContext* ctx = ShaderMgr::Instance()->GetContext();
// 	ctx->SetDefaultBlend();
}

void SpriteShader::SetColor(uint32_t color, uint32_t additive)
{
	m_color = color;
	m_additive = additive;
}

void SpriteShader::SetColorMap(uint32_t rmap, uint32_t gmap, uint32_t bmap)
{
	m_rmap = rmap;
	m_gmap = gmap;
	m_bmap = bmap;
}

void SpriteShader::InitProgs()
{
	std::shared_ptr<RenderBuffer> idx_buf;
	if (m_vertex_index) {
		auto& ur_rc = ur::Blackboard::Instance()->GetRenderContext();
		idx_buf = Utility::CreateQuadIndexBuffer(ur_rc, m_max_vertex / 4);
	}
	InitNoColorProg(idx_buf);
	InitMultiAddColorProg(idx_buf);
	InitMapColorProg(idx_buf);
	InitFullColorProg(idx_buf);
}

void SpriteShader::InitVAList(int position_sz)
{
	int sz = (position_sz + 2) * 4 + 20;
	int offset = 0;
	m_va_list[POSITION].Assign("position", position_sz, sizeof(float), sz, 0);
	m_va_list[TEXCOORD].Assign("texcoord", 2, sizeof(float), sz, position_sz * 4);
	m_va_list[COLOR].Assign("color", 4, sizeof(uint8_t), sz, position_sz * 4 + 8);
	m_va_list[ADDITIVE].Assign("additive", 4, sizeof(uint8_t), sz, position_sz * 4 + 12);
	m_va_list[RMAP].Assign("rmap", 4, sizeof(uint8_t), sz, position_sz * 4 + 16);
	m_va_list[GMAP].Assign("gmap", 4, sizeof(uint8_t), sz, position_sz * 4 + 20);
	m_va_list[BMAP].Assign("bmap", 4, sizeof(uint8_t), sz, position_sz * 4 + 24);
}

ShaderProgram* SpriteShader::CreateProg(parser::Node* vert, parser::Node* frag,
										const CU_VEC<VA_TYPE>& va_types, const std::shared_ptr<RenderBuffer>& ib) const
{
	ShaderProgram* prog = new ShaderProgram(m_rc, m_max_vertex);

	CU_VEC<ur::VertexAttrib> va_list;
	for (auto type : va_types) {
		va_list.push_back(m_va_list[type]);
	}

	int stride = 0;
	for (auto type : va_types) {
		stride += m_va_list[type].n * m_va_list[type].size;
	}
	int offset = 0;
	for (int i = 0, n = va_list.size(); i < n; ++i) {
		va_list[i].stride = stride;
		va_list[i].offset = offset;
		auto& type = m_va_list[va_types[i]];
		offset += type.n * type.size;
	}

	prog->Load(vert, frag, va_list, ib, true);

	InitMVP(prog->GetMVP());

	prog->GetShader()->SetDrawMode(ur::DRAW_TRIANGLES);

	return prog;
}

void SpriteShader::InitNoColorProg(const std::shared_ptr<RenderBuffer>& idx_buf)
{
	parser::Node* vert = new parser::PositionTrans();
	vert->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT2, "texcoord")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT2, "texcoord")));

	parser::Node* frag = new parser::TextureMap();
	frag->Connect(new parser::FragColor());

	CU_VEC<VA_TYPE> va_types;
	va_types.push_back(POSITION);
	va_types.push_back(TEXCOORD);
	m_programs[PI_NO_COLOR] = CreateProg(vert, frag, va_types, idx_buf);
}

void SpriteShader::InitMultiAddColorProg(const std::shared_ptr<RenderBuffer>& idx_buf)
{
	parser::Node* vert = new parser::PositionTrans();
	vert->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT2, "texcoord")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT2, "texcoord")))->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT4, "color")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, "color")))->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT4, "additive")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, "additive")));

	parser::Node* frag = new parser::TextureMap();
	frag->Connect(
		new parser::ColorAddMul())->Connect(
		new parser::FragColor());

	CU_VEC<VA_TYPE> va_types;
	va_types.push_back(POSITION);
	va_types.push_back(TEXCOORD);
	va_types.push_back(COLOR);
	va_types.push_back(ADDITIVE);
	m_programs[PI_MULTI_ADD_COLOR] = CreateProg(vert, frag, va_types, idx_buf);
}

void SpriteShader::InitMapColorProg(const std::shared_ptr<RenderBuffer>& idx_buf)
{
	parser::Node* vert = new parser::PositionTrans();
	vert->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT2, "texcoord")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT2, "texcoord")))->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT4, "rmap")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, "rmap")))->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT4, "gmap")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, "gmap")))->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT4, "bmap")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, "bmap")));

	parser::Node* frag = new parser::TextureMap();
	frag->Connect(
		new parser::ColorMap())->Connect(
		new parser::FragColor());

	CU_VEC<VA_TYPE> va_types;
	va_types.push_back(POSITION);
	va_types.push_back(TEXCOORD);
	va_types.push_back(RMAP);
	va_types.push_back(GMAP);
	va_types.push_back(BMAP);
	m_programs[PI_MAP_COLOR] = CreateProg(vert, frag, va_types, idx_buf);
}

void SpriteShader::InitFullColorProg(const std::shared_ptr<RenderBuffer>& idx_buf)
{
	parser::Node* vert = new parser::PositionTrans();
	vert->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT2, "texcoord")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT2, "texcoord")))->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT4, "color")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, "color")))->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT4, "additive")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, "additive")))->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT4, "rmap")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, "rmap")))->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT4, "gmap")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, "gmap")))->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT4, "bmap")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, "bmap")));

	parser::Node* frag = new parser::TextureMap();
	frag->Connect(
		new parser::ColorMap())->Connect(
		new parser::ColorAddMul())->Connect(
		new parser::FragColor());

	CU_VEC<VA_TYPE> va_types;
	va_types.push_back(POSITION);
	va_types.push_back(TEXCOORD);
	va_types.push_back(COLOR);
	va_types.push_back(ADDITIVE);
	va_types.push_back(RMAP);
	va_types.push_back(GMAP);
	va_types.push_back(BMAP);
	m_programs[PI_FULL_COLOR] = CreateProg(vert, frag, va_types, idx_buf);
}

}