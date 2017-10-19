#include "BlurProg.h"
#include "SL_RenderShader.h"
#include "Blur.h"

#include <unirender/ur_typedef.h>

namespace sl
{

BlurProg::BlurProg(ur::RenderContext* rc, int max_vertex, 
				   const CU_VEC<ur::VertexAttrib>& va_list, const std::shared_ptr<RenderBuffer>& ib)
	: FilterProgram(rc, max_vertex)
{
	Init(va_list, ib, new parser::Blur());
	m_radius = m_shader->AddUniform("u_radius", ur::UNIFORM_FLOAT1);
}

void BlurProg::SetRadius(float r)
{
	m_shader->SetUniform(m_radius, ur::UNIFORM_FLOAT1, &r);
}

}