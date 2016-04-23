#include "EdgeDetectProg.h"
#include "../render/RenderShader.h"
#include "../parser/EdgeDetect.h"

#include <render/render.h>

namespace sl
{

EdgeDetectProg::EdgeDetectProg(RenderContext* rc, int max_vertex, 
							   const std::vector<VertexAttrib>& va_list, RenderBuffer* ib)
	: FilterProgram(rc, max_vertex)
{
	Init(va_list, ib, new parser::EdgeDetect());
	m_blend = m_shader->AddUniform("u_blend", UNIFORM_FLOAT1);
}

void EdgeDetectProg::SetBlend(float blend)
{
	m_shader->SetUniform(m_blend, UNIFORM_FLOAT1, &blend);
}

}