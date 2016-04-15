#include "RenderBuffer.h"
#include "utility/Buffer.h"

namespace sl
{

RenderBuffer::RenderBuffer(render* ej_render, RENDER_OBJ type, int stride, int n, Buffer* buf)
	: m_ej_render(ej_render)
	, m_type(type)
	, m_buf(buf)
{
	m_id = render_buffer_create(ej_render, type, NULL, n, stride);

	// todo
//	render_set(m_ej_render, m_type, m_id, 0);
}

RenderBuffer::~RenderBuffer()
{
	render_release(m_ej_render, m_type, m_id);
	delete m_buf;
}

}