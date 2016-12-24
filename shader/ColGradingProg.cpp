#include "ColGradingProg.h"
#include "../render/RenderShader.h"
#include "../parser/ColorGrading.h"

#include <render/render.h>

namespace sl
{

ColGradingProg::ColGradingProg(RenderContext* rc, int max_vertex, 
						   const std::vector<VertexAttrib>& va_list, 
						   RenderBuffer* ib)
	: FilterProgram(rc, max_vertex)
	, m_lut_tex(0)
{
	Init(va_list, ib, new parser::ColorGrading());

	int tex0 = m_shader->AddUniform("u_texture0", UNIFORM_INT1);
	if (tex0 >= 0) {
		float sample = 0;
		m_shader->SetUniform(tex0, UNIFORM_INT1, &sample);
	}
	int tex1 = m_shader->AddUniform("u_col_lut", UNIFORM_INT1);
	if (tex1 >= 0) {
		float sample = 1;
		m_shader->SetUniform(tex1, UNIFORM_INT1, &sample);
	}
}

void ColGradingProg::Bind()
{
	m_rc->SetTexture(m_lut_tex, 1);
}

void ColGradingProg::SetLUTTex(int tex)
{
	m_lut_tex = tex;
	m_rc->SetTexture(m_lut_tex, 1);
}

}