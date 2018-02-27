#include "shaderlab/OutlineProg.h"
#include "shaderlab/RenderShader.h"
#include "shaderlab/Outline.h"

namespace sl
{

OutlineProg::OutlineProg(RenderContext& rc, int max_vertex,
						 const CU_VEC<ur::VertexAttrib>& va_list, const std::shared_ptr<RenderBuffer>& ib)
	: FilterProgram(rc, max_vertex)
{
	Init(va_list, ib, new parser::Outline());
}

}