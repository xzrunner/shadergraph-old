#include "shaderlab/Sprite2Shader.h"
#include "shaderlab/SubjectMVP2.h"
#include "shaderlab/ShaderProgram.h"
#include "shaderlab/ShaderType.h"
#include "shaderlab/ShaderMgr.h"
#include "shaderlab/RenderShader.h"
#include "shaderlab/RenderBuffer.h"
#include "shaderlab/StackAllocator.h"
#include "shaderlab/config.h"
#ifndef SL_DISABLE_STATISTICS
#include "shaderlab/StatDrawCall.h"
#endif // SL_DISABLE_STATISTICS
#include "shaderlab/RenderContext.h"

#include <unirender/RenderContext.h>
#include <unirender/Blackboard.h>

#include <assert.h>

namespace sl
{

static const int MAX_COMMBINE = 1024;

Sprite2Shader::Sprite2Shader(RenderContext& rc)
	: SpriteShader(rc, 2, MAX_COMMBINE * 4, true)
{
	InitProgs();
	m_vertex_buf = new Vertex[MAX_COMMBINE * 4];
}

bool Sprite2Shader::Commit() const
{
	if (m_quad_sz == 0) {
		return false;
	}

	ur::Blackboard::Instance()->GetRenderContext().BindTexture(m_texid, 0, false);

	ShaderProgram* prog = nullptr;
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

	int vertex_sz = prog->GetVertexSize();
	int vb_count = m_quad_sz * 4;
	int buf_sz = vertex_sz * vb_count;
	StackAllocator* alloc = StackAllocator::Instance();
	alloc->Reserve(buf_sz);
	void* buf = alloc->Alloc(buf_sz);
	uint8_t* ptr = (uint8_t*)buf;
	if (m_prog_type == PT_MAP_COLOR) {
		for (int i = 0; i < vb_count; ++i) {
			memcpy(ptr, &m_vertex_buf[i].vx, sizeof(float) * 4);
			ptr += sizeof(float) * 4;
			memcpy(ptr, &m_vertex_buf[i].rmap, sizeof(uint32_t) * 3);
			ptr += sizeof(uint32_t) * 3;
		}
	} else {
		for (int i = 0; i < vb_count; ++i) {
			memcpy(ptr, &m_vertex_buf[i].vx, vertex_sz);
			ptr += vertex_sz;
		}
	}

	RenderShader* shader = prog->GetShader();
	m_rc.GetShaderMgr().BindRenderShader(shader, SPRITE2);
	shader->Draw(buf, vb_count, nullptr, m_quad_sz * 6);
	alloc->Free(buf);

	m_quad_sz = 0;

	m_prog_type = 0;

	return shader->Commit();
}

void Sprite2Shader::DrawQuad(const float* positions, const float* texcoords, int texid) const
{
	if (m_quad_sz >= MAX_COMMBINE || (m_texid != texid && m_texid != 0)) {
		bool changed = Commit();
#ifndef SL_DISABLE_STATISTICS
		if (changed) {
			if (m_quad_sz >= MAX_COMMBINE) {
				StatDrawCall::Instance()->AddFull();
			} else {
				StatDrawCall::Instance()->AddTex();
			}
		}
#endif // SL_DISABLE_STATISTICS
	}
	m_texid = texid;

	bool has_multi_add = (m_color != 0xffffffff) || (m_additive != 0);
	bool has_map = ((m_rmap & 0x00ffffff) != 0x000000ff) || ((m_gmap & 0x00ffffff) != 0x0000ff00) || ((m_bmap & 0x00ffffff) != 0x00ff0000);
	if (has_multi_add) {
		m_prog_type |= PT_MULTI_ADD_COLOR;
	}
	if (has_map) {
		m_prog_type |= PT_MAP_COLOR;
	}

	for (int i = 0; i < 4; ++i)
	{
		Vertex* v	= &m_vertex_buf[m_quad_sz * 4 + i];
		v->vx		= positions[i * 2];
		v->vy		= positions[i * 2 + 1];
		v->tx		= texcoords[i * 2];
		v->ty		= texcoords[i * 2 + 1];
		v->color	= m_color;
		v->additive = m_additive;
		v->rmap		= m_rmap;
		v->gmap		= m_gmap;
		v->bmap		= m_bmap;
	}
	++m_quad_sz;
}

void Sprite2Shader::InitMVP(ObserverMVP* mvp) const
{
	m_rc.GetSubMVP2().Register(mvp);
}

}