#include "shaderlab/HeatHazeProg.h"
#include "shaderlab/RenderShader.h"
#include "shaderlab/HeatHaze.h"
#include "shaderlab/ShaderMgr.h"
#include "shaderlab/RenderContext.h"

#include <unirender/RenderContext.h>

namespace sl
{

HeatHazeProg::HeatHazeProg(RenderContext& rc, int max_vertex,
						   const CU_VEC<ur::VertexAttrib>& va_list, 
						   const std::shared_ptr<RenderBuffer>& ib)
	: FilterProgram(rc, max_vertex)
	, m_distortion_map_tex(0)
{
	Init(va_list, ib, new parser::HeatHaze());

	m_time = m_shader->AddUniform("u_time", ur::UNIFORM_FLOAT1);
	m_distortion_factor = m_shader->AddUniform("u_distortion_factor", ur::UNIFORM_FLOAT1);
	m_rise_factor = m_shader->AddUniform("u_rise_factor", ur::UNIFORM_FLOAT1);

	int tex0 = m_shader->AddUniform("u_texture0", ur::UNIFORM_INT1);
	if (tex0 >= 0) {
		float sample = 0;
		m_shader->SetUniform(tex0, ur::UNIFORM_INT1, &sample);
	}
	int tex1 = m_shader->AddUniform("u_distortion_map_tex", ur::UNIFORM_INT1);
	if (tex1 >= 0) {
		float sample = 1;
		m_shader->SetUniform(tex1, ur::UNIFORM_INT1, &sample);
	}
}

void HeatHazeProg::UpdateTime(float time)
{
	m_shader->SetUniform(m_time, ur::UNIFORM_FLOAT1, &time);
}

void HeatHazeProg::SetFactor(float distortion, float rise)
{
	m_shader->SetUniform(m_distortion_factor, ur::UNIFORM_FLOAT1, &distortion);
	m_shader->SetUniform(m_rise_factor, ur::UNIFORM_FLOAT1, &rise);
}

void HeatHazeProg::SetDistortionMapTex(int tex)
{
	m_distortion_map_tex = tex;
	m_rc.GetContext().BindTexture(m_distortion_map_tex, 1);
}

}