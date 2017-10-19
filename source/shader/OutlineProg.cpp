#include "OutlineProg.h"
#include "SL_RenderShader.h"
#include "Outline.h"

namespace sl
{

OutlineProg::OutlineProg(ur::RenderContext* rc, int max_vertex, 
						 const CU_VEC<ur::VertexAttrib>& va_list, const std::shared_ptr<RenderBuffer>& ib)
	: FilterProgram(rc, max_vertex)
{
	Init(va_list, ib, new parser::Outline());
}

}