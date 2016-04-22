#include "SwirlProg.h"
#include "render/RenderShader.h"
#include "parser/Swirl.h"

#include <render/render.h>

namespace sl
{

SwirlProg::SwirlProg(RenderContext* rc, int max_vertex, 
					 const std::vector<VertexAttrib>& va_list, RenderBuffer* ib)
	: FilterProgram(rc, max_vertex)
{
	Init(va_list, ib, new parser::Swirl());

	m_radius = m_shader->AddUniform("u_radius", UNIFORM_FLOAT1);
	m_angle = m_shader->AddUniform("u_angle", UNIFORM_FLOAT1);
	m_center = m_shader->AddUniform("u_center", UNIFORM_FLOAT2);
}

void SwirlProg::SetRadius(float radius)
{
	m_shader->SetUniform(m_radius, UNIFORM_FLOAT1, &radius);
}

void SwirlProg::SetAngle(float angle)
{
	m_shader->SetUniform(m_angle, UNIFORM_FLOAT1, &angle);
}

void SwirlProg::SetCenter(float cnenter[2])
{
	m_shader->SetUniform(m_center, UNIFORM_FLOAT2, cnenter);
}

}