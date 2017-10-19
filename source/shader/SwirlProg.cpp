#include "SwirlProg.h"
#include "SL_RenderShader.h"
#include "Swirl.h"

#include <unirender/ur_typedef.h>

namespace sl
{

SwirlProg::SwirlProg(ur::RenderContext* rc, int max_vertex, 
					 const CU_VEC<ur::VertexAttrib>& va_list, const std::shared_ptr<RenderBuffer>& ib)
	: FilterProgram(rc, max_vertex)
{
	Init(va_list, ib, new parser::Swirl());

	m_radius = m_shader->AddUniform("u_radius", ur::UNIFORM_FLOAT1);
	m_angle = m_shader->AddUniform("u_angle", ur::UNIFORM_FLOAT1);
	m_center = m_shader->AddUniform("u_center", ur::UNIFORM_FLOAT2);
}

void SwirlProg::SetRadius(float radius)
{
	m_shader->SetUniform(m_radius, ur::UNIFORM_FLOAT1, &radius);
}

void SwirlProg::SetAngle(float angle)
{
	m_shader->SetUniform(m_angle, ur::UNIFORM_FLOAT1, &angle);
}

void SwirlProg::SetCenter(float cnenter[2])
{
	m_shader->SetUniform(m_center, ur::UNIFORM_FLOAT2, cnenter);
}

}