#ifndef _SHADERLAB_RENDER_BUFFER_H_
#define _SHADERLAB_RENDER_BUFFER_H_

#include "../utility/Buffer.h"
#include "../utility/typedef.h"

#include <CU_RefCountObj.h>

namespace ur { class IRenderContext; }

namespace sl
{

class Buffer;

class RenderBuffer : public cu::RefCountObj
{
public:
	RenderBuffer(ur::IRenderContext* rc, RENDER_OBJ_TYPE type, int stride, int n, Buffer* buf);
	virtual ~RenderBuffer();

	void Bind();
	void Update();

	void Clear() { if (m_buf) { m_buf->Clear(); } }
	int Size() const { return m_buf ? m_buf->Size() : 0; }
	int Capacity() const { return m_buf ? m_buf->Capacity() : 0; }
	bool IsEmpty() const { return m_buf->IsEmpty(); }
	bool Add(const void* data, int n) { return m_buf->Add(data, n); }

private:
	ur::IRenderContext* m_rc;

	RENDER_OBJ_TYPE m_type;

	RID m_id;

	Buffer* m_buf;
	
}; // RenderBuffer

}

#endif // _SHADERLAB_RENDER_BUFFER_H_