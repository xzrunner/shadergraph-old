#ifndef _SHADERLAB_RENDER_LAYOUT_H_
#define _SHADERLAB_RENDER_LAYOUT_H_

#include "sl_typedef.h"

#include <cu/cu_stl.h>
#include <unirender/VertexAttrib.h>

namespace ur { class RenderContext; }

namespace sl
{

class RenderLayout
{
public:
	RenderLayout(ur::RenderContext* rc, const CU_VEC<ur::VertexAttrib>& va_list);
	virtual ~RenderLayout();

	void Bind();

private:
	ur::RenderContext* m_rc;

	RID m_id;

}; // RenderLayout

}

#endif // _SHADERLAB_RENDER_LAYOUT_H_