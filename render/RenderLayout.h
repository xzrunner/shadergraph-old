#ifndef _SHADERLAB_RENDER_LAYOUT_H_
#define _SHADERLAB_RENDER_LAYOUT_H_

#include "../utility/typedef.h"

#include <CU_RefCountObj.h>
#include <unirender/VertexAttrib.h>

#include <vector>

namespace ur { class IRenderContext; }

namespace sl
{

class RenderLayout : public cu::RefCountObj
{
public:
	RenderLayout(ur::IRenderContext* rc, const std::vector<ur::VertexAttrib>& va_list);
	virtual ~RenderLayout();

	void Bind();

private:
	ur::IRenderContext* m_rc;

	RID m_id;

}; // RenderLayout

}

#endif // _SHADERLAB_RENDER_LAYOUT_H_