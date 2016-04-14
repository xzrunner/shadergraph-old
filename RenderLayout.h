#ifndef _SHADERLAB_RENDER_LAYOUT_H_
#define _SHADERLAB_RENDER_LAYOUT_H_

#include "Object.h"

#include <render/render.h>

namespace sl
{

class RenderLayout : public Object
{
public:
	RenderLayout(render* ej_render, int n, struct vertex_attrib* va);
	virtual ~RenderLayout();

	void Bind();

private:
	render* m_ej_render;

	RID m_id;

}; // RenderLayout

}

#endif // _SHADERLAB_RENDER_LAYOUT_H_