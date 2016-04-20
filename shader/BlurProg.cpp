#include "BlurProg.h"
#include "render/RenderShader.h"
#include "parser/Blur.h"

namespace sl
{

BlurProg::BlurProg(RenderContext* rc, int max_vertex, 
				   const std::vector<VertexAttrib>& va_list, RenderBuffer* ib)
	: FilterProgram(rc, max_vertex)
{
	Init(va_list, ib, new parser::Blur());
	m_radius = m_shader->AddUniform("u_radius", UNIFORM_FLOAT1);
}

void BlurProg::SetRadius(float r)
{
	m_shader->SetUniform(m_radius, UNIFORM_FLOAT1, &r);
}

}