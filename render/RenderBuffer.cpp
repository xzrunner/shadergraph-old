#include "RenderBuffer.h"
#include "../utility/Buffer.h"

#include <render/render.h>

namespace sl
{

RenderBuffer::RenderBuffer(render* ej_render, RENDER_OBJ_TYPE type, int stride, int n, Buffer* buf)
	: m_ej_render(ej_render)
	, m_type(type)
	, m_buf(buf)
{
	m_id = render_buffer_create(ej_render, (enum RENDER_OBJ)type, NULL, n, stride);

	// todo
//	render_set(m_ej_render, m_type, m_id, 0);
}

RenderBuffer::~RenderBuffer()
{
	render_release(m_ej_render, (enum RENDER_OBJ)m_type, m_id);
	if (m_buf) {
		delete m_buf;
	}
}

void RenderBuffer::Bind() 
{
	render_set(m_ej_render, (enum RENDER_OBJ)m_type, m_id, 0);
}

void RenderBuffer::Update() 
{
	if (m_buf->IsDirty()) {
		render_buffer_update(m_ej_render, m_id, m_buf->Data(), m_buf->Size());
		m_buf->ResetDirty();
	}
}

}