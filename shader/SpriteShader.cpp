#include "SpriteShader.h"
#include "ObserverMVP.h"
#include "SubjectMVP2.h"
#include "Utility.h"
#include "render/RenderShader.h"
#include "render/RenderBuffer.h"
#include "render/RenderContext.h"
#include "render/RenderLayout.h"
#include "parser/Shader.h"
#include "parser/PositionTrans.h"
#include "parser/TextureMap.h"
#include "parser/FragColor.h"
#include "parser/ColorAddMulti.h"
#include "parser/ColorMap.h"
#include "utility/StackAllocator.h"

namespace sl
{

static const int MAX_COMMBINE = 1024;

SpriteShader::SpriteShader(RenderContext* rc)
	: Shader(rc)
{
	Init();

	m_color = 0xffffffff;
	m_additive = 0x00000000;
	m_rmap = 0x000000ff;
	m_gmap = 0x0000ff00;
	m_bmap = 0x00ff0000;

	m_texid = 0;

	m_vertex_buf = new Vertex[MAX_COMMBINE * 4];
	m_quad_sz = 0;

	m_prog_type = 0;
}

SpriteShader::~SpriteShader()
{
	for (int i = 0; i < PROG_SIZE; ++i) {
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
}

void SpriteShader::Commit() const
{
	if (m_quad_sz == 0) {
		return;
	}

	m_rc->SetTexture(m_texid, 0);

	Program* prog = NULL;
	switch (m_prog_type)
	{
	case PT_NULL:
		prog = m_programs[PI_NO_COLOR];
		break;
	case PT_MULTI_ADD_COLOR:
		prog = m_programs[PI_MULTI_ADD_COLOR];
		break;
	case PT_MAP_COLOR:
		prog = m_programs[PI_MAP_COLOR];
		break;
	default:
		assert((m_prog_type & PT_MULTI_ADD_COLOR) && (m_prog_type & PT_MAP_COLOR));
		prog = m_programs[PI_FULL_COLOR];
	}

	int vb_count = m_quad_sz * 4;
	StackAllocator* alloc = StackAllocator::Instance();
	int buf_sz = prog->vertex_sz * vb_count;
	alloc->Reserve(buf_sz);
	void* buf = alloc->Alloc(buf_sz);
	uint8_t* ptr = (uint8_t*)buf;
	if (m_prog_type == PT_MAP_COLOR) {
		for (int i = 0; i < vb_count; ++i) {
			memcpy(ptr, &m_vertex_buf[i].vx, sizeof(float) * 4);
			ptr += sizeof(float) * 4;
			memcpy(ptr, &m_vertex_buf[i].color, sizeof(uint32_t) * 2);
			ptr += sizeof(uint32_t) * 2;
		}
	} else {
		for (int i = 0; i < vb_count; ++i) {
			memcpy(ptr, &m_vertex_buf[i].vx, prog->vertex_sz);
			ptr += prog->vertex_sz;
		}
	}
	m_rc->BindShader(prog->shader);
	prog->shader->Draw(buf, vb_count, m_quad_sz * 6);
	alloc->Free(buf);
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

void SpriteShader::Draw(const float* positions, const float* texcoords, int texid) const
{
	if (m_quad_sz >= MAX_COMMBINE || (m_texid != texid && m_texid != 0)) {
		Commit();
	}
	m_texid = texid;

	m_prog_type = PT_NULL;
	bool has_multi_add = (m_color != 0xffffffff) || ((m_additive & 0xffffff) != 0);
	bool has_map = ((m_rmap & 0x00ffffff) != 0x000000ff) || ((m_gmap & 0x00ffffff) != 0x0000ff00) || ((m_bmap & 0x00ffffff) != 0x00ff0000);
	if (has_multi_add) {
		m_prog_type |= PT_MULTI_ADD_COLOR;
	}
	if (has_map) {
		m_prog_type |= PT_MAP_COLOR;
	}

	for (int i = 0; i < 4; ++i) 
	{
		Vertex* v = &m_vertex_buf[m_quad_sz * 4 + i];
		v->vx = positions[i * 2];
		v->vy = positions[i * 2 + 1];
		v->tx = texcoords[i * 2];
		v->ty = texcoords[i * 2 + 1];
		v->color	= m_color;
		v->additive = m_additive;
		v->rmap = m_rmap;
		v->gmap = m_gmap;
		v->bmap = m_bmap;
	}
	++m_quad_sz;
}

void SpriteShader::Init()
{
	RenderBuffer* idx_buf = Utility::CreateQuadIndexBuffer(m_rc, MAX_COMMBINE);

	InitNoColorProg(idx_buf);
	InitMultiAddColorProg(idx_buf);
	InitMapColorProg(idx_buf);
	InitFullColorProg(idx_buf);

	idx_buf->Release();
}

void SpriteShader::InitNoColorProg(RenderBuffer* idx_buf)
{
	parser::Node* vert = new parser::PositionTrans();
	parser::Node* frag = new parser::TextureMap();
	frag->Connect(new parser::FragColor());

	struct vertex_attrib va[2] = {
		{ "position", 0, 2, sizeof(float), 0 },
		{ "texcoord", 0, 2, sizeof(float), sizeof(float) * 2 },
	};

	m_programs[PI_NO_COLOR] = CreateProg(vert, frag, va, sizeof(float) * 4);
}

void SpriteShader::InitMultiAddColorProg(RenderBuffer* idx_buf)
{
	parser::Node* vert = new parser::PositionTrans();
	parser::Node* frag = (
		new parser::TextureMap())->Connect(
		new parser::ColorAddMulti())->Connect(
		new parser::FragColor());

	struct vertex_attrib va[4] = {
		{ "position", 0, 2, sizeof(float), 0 },
		{ "texcoord", 0, 2, sizeof(float), sizeof(float) * 2 },
		{ "color", 0, 4, sizeof(uint8_t), sizeof(float) * 4 },
		{ "additive", 0, 4, sizeof(uint8_t), sizeof(float) * 4 + sizeof(uint32_t) },
	};

	m_programs[PI_MULTI_ADD_COLOR] = CreateProg(vert, frag, va, sizeof(float) * 4 + sizeof(uint32_t) * 2);
}

void SpriteShader::InitMapColorProg(RenderBuffer* idx_buf)
{
	parser::Node* vert = new parser::PositionTrans();
	parser::Node* frag = (
		new parser::TextureMap())->Connect(
		new parser::ColorMap())->Connect(
		new parser::FragColor());

	struct vertex_attrib va[5] = {
		{ "position", 0, 2, sizeof(float), 0 },
		{ "texcoord", 0, 2, sizeof(float), sizeof(float) * 2 },
		{ "rmap", 0, 4, sizeof(uint8_t), sizeof(float) * 4 },
		{ "gmap", 0, 4, sizeof(uint8_t), sizeof(float) * 4 + sizeof(uint32_t) },
		{ "bmap", 0, 4, sizeof(uint8_t), sizeof(float) * 4 + sizeof(uint32_t) * 2 },
	};

	m_programs[PI_MAP_COLOR] = CreateProg(vert, frag, va, sizeof(float) * 4 + sizeof(uint32_t) * 3);
}

void SpriteShader::InitFullColorProg(RenderBuffer* idx_buf)
{
	parser::Node* vert = new parser::PositionTrans();
	parser::Node* frag = (
		new parser::TextureMap())->Connect(
		new parser::ColorMap())->Connect(
		new parser::ColorAddMulti())->Connect(
		new parser::FragColor());

	struct vertex_attrib va[7] = {
		{ "position", 0, 2, sizeof(float), 0 },
		{ "texcoord", 0, 2, sizeof(float), sizeof(float) * 2 },
		{ "color", 0, 4, sizeof(uint8_t), sizeof(float) * 4 },
		{ "additive", 0, 4, sizeof(uint8_t), sizeof(float) * 4 + sizeof(uint32_t) },
		{ "rmap", 0, 4, sizeof(uint8_t), sizeof(float) * 4 + sizeof(uint32_t) * 2 },
		{ "gmap", 0, 4, sizeof(uint8_t), sizeof(float) * 4 + sizeof(uint32_t) * 3 },
		{ "bmap", 0, 4, sizeof(uint8_t), sizeof(float) * 4 + sizeof(uint32_t) * 4 },
	};

	m_programs[PI_FULL_COLOR] = CreateProg(vert, frag, va, sizeof(float) * 4 + sizeof(uint32_t) * 5);
}

SpriteShader::Program* SpriteShader::CreateProg(parser::Node* vert, parser::Node* frag, 
												vertex_attrib* va, int vertex_sz) const
{
	// shader
	parser::Shader* parser = new parser::Shader(vert, frag);
	RenderShader* shader = m_rc->CreateShader();
	Program* prog = new Program(parser, shader);

	// vertex buffer
	prog->vertex_sz = vertex_sz;
	Buffer* buf = new Buffer(vertex_sz, 4 * MAX_COMMBINE);
	RenderBuffer* vb = new RenderBuffer(m_rc->GetEJRender(), VERTEXBUFFER, vertex_sz, 4 * MAX_COMMBINE, buf);
	shader->SetVertexBuffer(vb);
	vb->Release();

	// vertex layout
	RenderLayout* lo = new RenderLayout(m_rc->GetEJRender(), sizeof(va)/sizeof(va[0]), va);
	shader->SetLayout(lo);
	lo->Release();

	// create
	shader->Load(parser->GetVertStr(), parser->GetFragStr());

	// uniforms
	prog->mvp = new ObserverMVP(shader);
	prog->mvp->InitModelview(shader->AddUniform("u_modelview", UNIFORM_FLOAT44));
	prog->mvp->InitProjection(shader->AddUniform("u_projection", UNIFORM_FLOAT44));
	SubjectMVP2::Instance()->Register(prog->mvp);

	return prog;
}

}