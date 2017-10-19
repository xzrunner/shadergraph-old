#include "EdgeDetectProg.h"
#include "SL_RenderShader.h"
#include "EdgeDetect.h"

#include <unirender/ur_typedef.h>

namespace sl
{

EdgeDetectProg::EdgeDetectProg(ur::RenderContext* rc, int max_vertex, 
							   const CU_VEC<ur::VertexAttrib>& va_list, const std::shared_ptr<RenderBuffer>& ib)
	: FilterProgram(rc, max_vertex)
{
	Init(va_list, ib, new parser::EdgeDetect());
	m_blend = m_shader->AddUniform("u_blend", ur::UNIFORM_FLOAT1);
}

void EdgeDetectProg::SetBlend(float blend)
{
	m_shader->SetUniform(m_blend, ur::UNIFORM_FLOAT1, &blend);
}

}