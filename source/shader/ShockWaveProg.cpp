#include "shaderlab/ShockWaveProg.h"
#include "shaderlab/RenderShader.h"
#include "shaderlab/ShockWave.h"

#include <unirender/typedef.h>

namespace sl
{

ShockWaveProg::ShockWaveProg(ShaderMgr& shader_mgr, int max_vertex,
							 const CU_VEC<ur::VertexAttrib>& va_list, const std::shared_ptr<RenderBuffer>& ib)
	: FilterProgram(shader_mgr, max_vertex)
{
	Init(va_list, ib, new parser::ShockWave());

	m_time = m_shader->AddUniform("u_time", ur::UNIFORM_FLOAT1);
	m_center = m_shader->AddUniform("u_center", ur::UNIFORM_FLOAT2);
	m_params = m_shader->AddUniform("u_params", ur::UNIFORM_FLOAT3);
}

void ShockWaveProg::UpdateTime(float time)
{
	m_shader->SetUniform(m_time, ur::UNIFORM_FLOAT1, &time);
}

void ShockWaveProg::SetCenter(float center[2])
{
	m_shader->SetUniform(m_center, ur::UNIFORM_FLOAT2, center);
}

void ShockWaveProg::SetFactor(float params[3])
{
	m_shader->SetUniform(m_params, ur::UNIFORM_FLOAT3, params);
}

}