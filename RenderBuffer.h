#ifndef _SHADERLAB_RENDER_BUFFER_H_
#define _SHADERLAB_RENDER_BUFFER_H_

#include "Object.h"
#include "Buffer.h"

#include <render/render.h>

namespace sl
{

class Buffer;

class RenderBuffer : public Object
{
public:
	RenderBuffer(render* ej_render, RENDER_OBJ type, int stride, int n, Buffer* buf);
	virtual ~RenderBuffer();

	void Bind() {
		render_set(m_ej_render, m_type, m_id, 0);
	}

	void Update() {
		if (m_buf->IsDirty()) {
			render_buffer_update(m_ej_render, m_id, m_buf->Data(), m_buf->Size());
			m_buf->ResetDirty();
		}
	}

	void Clear() { if (m_buf) { m_buf->Clear(); } }
	int Size() const { return m_buf ? m_buf->Size() : 0; }
	bool IsEmpty() const { return m_buf->IsEmpty(); }
	bool Add(const void* data, int n) { return m_buf->Add(data, n); }

private:
	render* m_ej_render;

	RENDER_OBJ m_type;

	RID m_id;

	Buffer* m_buf;
	
}; // RenderBuffer

}

#endif // _SHADERLAB_RENDER_BUFFER_H_