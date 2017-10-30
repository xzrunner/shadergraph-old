#include "shaderlab/RenderLayout.h"

#include <unirender/RenderContext.h>

namespace sl
{

RenderLayout::RenderLayout(ur::RenderContext* rc, const CU_VEC<ur::VertexAttrib>& va_list)
	: m_rc(rc)
{
	m_id = m_rc->CreateVertexLayout(va_list);
}

RenderLayout::~RenderLayout()
{
	m_rc->ReleaseVertexLayout(m_id);
}

void RenderLayout::Bind()
{
	m_rc->BindVertexLayout(m_id);
}

}