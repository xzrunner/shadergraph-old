#include "SpriteShader.h"
#include "ObserverMVP.h"
#include "Utility.h"
#include "render/RenderContext.h"
#include "render/RenderBuffer.h"
#include "render/RenderShader.h"
#include "render/RenderLayout.h"
#include "parser/Shader.h"
#include "parser/PositionTrans.h"
#include "parser/TextureMap.h"
#include "parser/FragColor.h"
#include "parser/ColorAddMulti.h"
#include "parser/ColorMap.h"
#include "utility/Buffer.h"

namespace sl
{

SpriteShader::SpriteShader(RenderContext* rc, int position_sz, int max_vertex,
						   bool vertex_index)
	: Shader(rc)
	, m_max_vertex(max_vertex)
	, m_vertex_index(vertex_index)
{
	m_rc->SetClearFlag(MASKC);

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
	for (int i = 0; i < PROG_COUNT; ++i) 
	{
		Program* proj = m_programs[i];
		delete proj->parser;
		proj->shader->Unload();
		delete proj->shader;
		delete proj->mvp;
		delete proj;
	}
}

void SpriteShader::Bind() const
{
	// todo bind PT_NO_COLOR ? 
}

void SpriteShader::UnBind() const
{
//	Commit();
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
	RenderBuffer* idx_buf = NULL;
	if (m_vertex_index) {
		idx_buf = Utility::CreateQuadIndexBuffer(m_rc, m_max_vertex / 4);
	}
	InitNoColorProg(idx_buf);
	InitMultiAddColorProg(idx_buf);
	InitMapColorProg(idx_buf);
	InitFullColorProg(idx_buf);
	if (m_vertex_index) {
		idx_buf->Release();
	}
}

void SpriteShader::InitVAList(int position_sz)
{
	m_va_list[POSITION].Assign("position", position_sz, sizeof(float));
	m_va_list[TEXCOORD].Assign("texcoord", 2, sizeof(float));
	m_va_list[COLOR].Assign("color", 4, sizeof(uint8_t));
	m_va_list[ADDITIVE].Assign("additive", 4, sizeof(uint8_t));
	m_va_list[RMAP].Assign("rmap", 4, sizeof(uint8_t));
	m_va_list[GMAP].Assign("gmap", 4, sizeof(uint8_t));
	m_va_list[BMAP].Assign("bmap", 4, sizeof(uint8_t));
}

SpriteShader::Program* SpriteShader::CreateProg(parser::Node* vert, parser::Node* frag, 
												const std::vector<VA_TYPE>& va_types, RenderBuffer* ib) const
{
	// shader
	parser::Shader* parser = new parser::Shader(vert, frag);
	RenderShader* shader = m_rc->CreateShader();
	Program* prog = new Program(parser, shader);

	// vertex layout
	std::vector<VertexAttrib> va_list;
	int vertex_sz = 0;
	for (int i = 0, n = va_types.size(); i < n; ++i) {
		const VertexAttrib& va = m_va_list[va_types[i]];
		va_list.push_back(va);
		vertex_sz += va.tot_size;
	}
	RenderLayout* lo = new RenderLayout(m_rc->GetEJRender(), va_list);
	shader->SetLayout(lo);
	lo->Release();

	// vertex buffer
	prog->vertex_sz = vertex_sz;
	Buffer* buf = new Buffer(vertex_sz, m_max_vertex);
	RenderBuffer* vb = new RenderBuffer(m_rc->GetEJRender(), VERTEXBUFFER, vertex_sz, m_max_vertex, buf);
	shader->SetVertexBuffer(vb);
	vb->Release();

	// index buffer
	if (m_vertex_index) {
		shader->SetIndexBuffer(ib);
	}

	// create
	shader->Load(parser->GetVertStr(), parser->GetFragStr());
	shader->SetDrawMode(DRAW_TRIANGLES);

	// uniforms
	prog->mvp = new ObserverMVP(shader);
	prog->mvp->InitModelview(shader->AddUniform("u_modelview", UNIFORM_FLOAT44));
	prog->mvp->InitProjection(shader->AddUniform("u_projection", UNIFORM_FLOAT44));
	InitMVP(prog->mvp);

	return prog;
}

void SpriteShader::InitNoColorProg(RenderBuffer* idx_buf)
{
	parser::Node* vert = new parser::PositionTrans();
	parser::Node* frag = new parser::TextureMap();
	frag->Connect(new parser::FragColor());

	std::vector<VA_TYPE> va_types;
	va_types.push_back(POSITION);
	va_types.push_back(TEXCOORD);
	m_programs[PI_NO_COLOR] = CreateProg(vert, frag, va_types, idx_buf);
}

void SpriteShader::InitMultiAddColorProg(RenderBuffer* idx_buf)
{
	parser::Node* vert = new parser::PositionTrans();
	parser::Node* frag = new parser::TextureMap();
	frag->Connect(
		new parser::ColorAddMulti())->Connect(
		new parser::FragColor());

	std::vector<VA_TYPE> va_types;
	va_types.push_back(POSITION);
	va_types.push_back(TEXCOORD);
	va_types.push_back(COLOR);
	va_types.push_back(ADDITIVE);
	m_programs[PI_MULTI_ADD_COLOR] = CreateProg(vert, frag, va_types, idx_buf);
}

void SpriteShader::InitMapColorProg(RenderBuffer* idx_buf)
{
	parser::Node* vert = new parser::PositionTrans();
	parser::Node* frag = new parser::TextureMap();
	frag->Connect(
		new parser::ColorMap())->Connect(
		new parser::FragColor());

	std::vector<VA_TYPE> va_types;
	va_types.push_back(POSITION);
	va_types.push_back(TEXCOORD);
	va_types.push_back(RMAP);
	va_types.push_back(GMAP);
	va_types.push_back(BMAP);
	m_programs[PI_MAP_COLOR] = CreateProg(vert, frag, va_types, idx_buf);
}

void SpriteShader::InitFullColorProg(RenderBuffer* idx_buf)
{
	parser::Node* vert = new parser::PositionTrans();
	parser::Node* frag = new parser::TextureMap();
	frag->Connect(
		new parser::ColorMap())->Connect(
		new parser::ColorAddMulti())->Connect(
		new parser::FragColor());

	std::vector<VA_TYPE> va_types;
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