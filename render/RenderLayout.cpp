#include "RenderLayout.h"
#include "RenderConst.h"

#include <render/render.h>

namespace sl
{

RenderLayout::RenderLayout(render* ej_render, const std::vector<VertexAttrib>& va_list)
	: m_ej_render(ej_render)
{
	struct vertex_attrib va[MAX_LAYOUT];
	int offset = 0;
	for (int i = 0, n = va_list.size(); i < n; ++i) 
	{
		const VertexAttrib& src = va_list[i];
		vertex_attrib& dst = va[i];
		dst.name = src.name.c_str();
		dst.vbslot = 0;	// todo
		dst.n = src.n;
		dst.size = src.size;
		dst.offset = offset;
		offset += src.tot_size;
	}

	m_id = render_register_vertexlayout(ej_render, va_list.size(), va);
}

RenderLayout::~RenderLayout()
{
	render_release(m_ej_render, VERTEXLAYOUT, m_id);
}

void RenderLayout::Bind()
{
	render_set(m_ej_render, VERTEXLAYOUT, m_id, 0);
}

}