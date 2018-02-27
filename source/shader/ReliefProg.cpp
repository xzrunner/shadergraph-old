#include "shaderlab/ReliefProg.h"
#include "shaderlab/RenderShader.h"
#include "shaderlab/Relief.h"

namespace sl
{

ReliefProg::ReliefProg(RenderContext& rc, int max_vertex,
					   const CU_VEC<ur::VertexAttrib>& va_list, const std::shared_ptr<RenderBuffer>& ib)
	: FilterProgram(rc, max_vertex)
{
	Init(va_list, ib, new parser::Relief());
}

}