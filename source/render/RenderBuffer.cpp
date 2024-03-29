#include "shaderlab/RenderBuffer.h"
#include "shaderlab/Buffer.h"

#include <unirender/RenderContext.h>
#include <unirender/Blackboard.h>

namespace sl
{

RenderBuffer::RenderBuffer(ur::RenderContext& rc, RENDER_OBJ_TYPE type, int size, Buffer* buf)
	: m_rc(rc)
	, m_type(type)
	, m_buf(buf)
{
	m_id = m_rc.CreateBuffer((ur::RENDER_OBJ)type, nullptr, size);

	// todo
//	render_set(m_ej_render, m_type, m_id, 0);
}

RenderBuffer::~RenderBuffer()
{
	m_rc.ReleaseBuffer((ur::RENDER_OBJ)m_type, m_id);
	if (m_buf) {
		delete m_buf;
	}
}

void RenderBuffer::Bind()
{
	m_rc.BindBuffer((ur::RENDER_OBJ)m_type, m_id);
}

void RenderBuffer::Update()
{
	if (m_buf->IsDirty()) {
		m_rc.UpdateBuffer(m_id, m_buf->Data(),
			m_buf->Size() * m_buf->Stride());
		m_buf->ResetDirty();
	}
}

}