#include "RenderLayout.h"

namespace sl
{

RenderLayout::RenderLayout(render* ej_render, int n, struct vertex_attrib* va)
	: m_ej_render(ej_render)
{
	m_id = render_register_vertexlayout(ej_render, n, va);
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