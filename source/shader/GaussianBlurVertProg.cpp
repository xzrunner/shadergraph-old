#include "shaderlab/GaussianBlurVertProg.h"
#include "shaderlab/RenderShader.h"
#include "shaderlab/GaussianBlurVert.h"

#include <unirender/typedef.h>

namespace sl
{

GaussianBlurVertProg::GaussianBlurVertProg(RenderContext& rc, int max_vertex,
										   const CU_VEC<ur::VertexAttrib>& va_list, const std::shared_ptr<RenderBuffer>& ib)
	: FilterProgram(rc, max_vertex)
	, m_tex_height_val(0)
{
	Init(va_list, ib, new parser::GaussianBlurVert());

	m_tex_height_id = m_shader->AddUniform("u_tex_height", ur::UNIFORM_FLOAT1);
}

void GaussianBlurVertProg::SetTexHeight(float height)
{
	if (height != m_tex_height_val) {
		m_tex_height_val = height;
		m_shader->SetUniform(m_tex_height_id, ur::UNIFORM_FLOAT1, &height);
	}
}

}