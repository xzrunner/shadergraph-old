#include "GaussianBlurVertProg.h"
#include "../render/RenderShader.h"
#include "../parser/GaussianBlurVert.h"

// for UNIFORM_INT1
#include <render/render.h>

namespace sl
{

GaussianBlurVertProg::GaussianBlurVertProg(RenderContext* rc, int max_vertex, 
										   const std::vector<VertexAttrib>& va_list, RenderBuffer* ib)
	: FilterProgram(rc, max_vertex)
	, m_tex_height_val(0)
{
	Init(va_list, ib, new parser::GaussianBlurVert());

	m_tex_height_id = m_shader->AddUniform("u_tex_height", UNIFORM_FLOAT1);
}

void GaussianBlurVertProg::SetTexHeight(float height)
{
	if (height != m_tex_height_val) {
		m_tex_height_val = height;
		m_shader->SetUniform(m_tex_height_id, UNIFORM_FLOAT1, &height);
	}
}

}