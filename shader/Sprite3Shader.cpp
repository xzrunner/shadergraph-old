#include "Sprite3Shader.h"
#include "SubjectMVP3.h"
#include "render/RenderShader.h"
#include "render/RenderBuffer.h"
#include "render/RenderContext.h"
#include "utility/StackAllocator.h"

#include <assert.h>

namespace sl
{

static const int MAX_VERTICES = 4096;

Sprite3Shader::Sprite3Shader(RenderContext* rc)
	: SpriteShader(rc, 3, MAX_VERTICES, false)
{
	InitProgs();
	m_vertex_buf = new Vertex[MAX_VERTICES];
}

void Sprite3Shader::Commit() const
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

	int vb_count = m_quad_sz * 6;
	StackAllocator* alloc = StackAllocator::Instance();
	int buf_sz = prog->vertex_sz * vb_count;
	alloc->Reserve(buf_sz);
	void* buf = alloc->Alloc(buf_sz);
	uint8_t* ptr = (uint8_t*)buf;
	if (m_prog_type == PT_MAP_COLOR) {
		for (int i = 0; i < vb_count; ++i) {
			memcpy(ptr, &m_vertex_buf[i].vx, sizeof(float) * 5);
			ptr += sizeof(float) * 5;
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
	prog->shader->Draw(buf, vb_count, 0);
	alloc->Free(buf);

	m_quad_sz = 0;

	prog->shader->Commit();
}

void Sprite3Shader::Draw(const float* positions, const float* texcoords, int texid) const
{
	if (m_quad_sz * 6 >= MAX_VERTICES || (m_texid != texid && m_texid != 0)) {
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

	for (int i = 0; i < 6; ++i) 
	{
		Vertex* v = &m_vertex_buf[m_quad_sz * 6 + i];
		v->vx = positions[i * 3];
		v->vy = positions[i * 3 + 1];
		v->vy = positions[i * 3 + 2];
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

void Sprite3Shader::InitMVP(ObserverMVP* mvp) const
{
	SubjectMVP3::Instance()->Register(mvp);
}

}