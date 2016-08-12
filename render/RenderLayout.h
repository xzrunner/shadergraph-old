#ifndef _SHADERLAB_RENDER_LAYOUT_H_
#define _SHADERLAB_RENDER_LAYOUT_H_

#include "VertexAttrib.h"
#include "../utility/typedef.h"

#include <CU_RefCountObj.h>

#include <vector>

struct render;

namespace sl
{

class RenderLayout : public cu::RefCountObj
{
public:
	RenderLayout(render* ej_render, const std::vector<VertexAttrib>& va_list);
	virtual ~RenderLayout();

	void Bind();

private:
	render* m_ej_render;

	RID m_id;

}; // RenderLayout

}

#endif // _SHADERLAB_RENDER_LAYOUT_H_