#include "ShockWaveProg.h"
#include "../render/RenderShader.h"
#include "../parser/ShockWave.h"

#include <render/render.h>

namespace sl
{

ShockWaveProg::ShockWaveProg(RenderContext* rc, int max_vertex, 
							 const std::vector<VertexAttrib>& va_list, RenderBuffer* ib)
	: FilterProgram(rc, max_vertex)
{
	Init(va_list, ib, new parser::ShockWave());

	m_time = m_shader->AddUniform("u_time", UNIFORM_FLOAT1);
	m_center = m_shader->AddUniform("u_center", UNIFORM_FLOAT2);
	m_params = m_shader->AddUniform("u_params", UNIFORM_FLOAT3);
}

void ShockWaveProg::UpdateTime(float time)
{
	m_shader->SetUniform(m_time, UNIFORM_FLOAT1, &time);
}

void ShockWaveProg::SetCenter(float center[2])
{
	m_shader->SetUniform(m_center, UNIFORM_FLOAT2, center);
}

void ShockWaveProg::SetFactor(float params[3])
{
	m_shader->SetUniform(m_params, UNIFORM_FLOAT3, params);
}

}