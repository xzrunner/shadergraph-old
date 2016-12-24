#include "HeatHazeProg.h"
#include "../render/RenderShader.h"
#include "../parser/HeatHaze.h"

#include <render/render.h>

namespace sl
{

HeatHazeProg::HeatHazeProg(RenderContext* rc, int max_vertex, 
						   const std::vector<VertexAttrib>& va_list, 
						   RenderBuffer* ib)
	: FilterProgram(rc, max_vertex)
	, m_distortion_map_tex(0)
{
	Init(va_list, ib, new parser::HeatHaze());

	m_time = m_shader->AddUniform("u_time", UNIFORM_FLOAT1);
	m_distortion_factor = m_shader->AddUniform("u_distortion_factor", UNIFORM_FLOAT1);
	m_rise_factor = m_shader->AddUniform("u_rise_factor", UNIFORM_FLOAT1);

	int tex0 = m_shader->AddUniform("u_texture0", UNIFORM_INT1);
	if (tex0 >= 0) {
		float sample = 0;
		m_shader->SetUniform(tex0, UNIFORM_INT1, &sample);
	}
	int tex1 = m_shader->AddUniform("u_distortion_map_tex", UNIFORM_INT1);
	if (tex1 >= 0) {
		float sample = 1;
		m_shader->SetUniform(tex1, UNIFORM_INT1, &sample);
	}
}

void HeatHazeProg::UpdateTime(float time)
{
	m_shader->SetUniform(m_time, UNIFORM_FLOAT1, &time);
}

void HeatHazeProg::SetFactor(float distortion, float rise)
{
	m_shader->SetUniform(m_distortion_factor, UNIFORM_FLOAT1, &distortion);
	m_shader->SetUniform(m_rise_factor, UNIFORM_FLOAT1, &rise);
}

void HeatHazeProg::SetDistortionMapTex(int tex)
{
	m_distortion_map_tex = tex;
	m_rc->SetTexture(m_distortion_map_tex, 1);
}

}