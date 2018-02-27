#include "shaderlab/ReliefProg.h"
#include "shaderlab/RenderShader.h"
#include "shaderlab/Relief.h"

namespace sl
{

ReliefProg::ReliefProg(ShaderMgr& shader_mgr, int max_vertex,
					   const CU_VEC<ur::VertexAttrib>& va_list, const std::shared_ptr<RenderBuffer>& ib)
	: FilterProgram(shader_mgr, max_vertex)
{
	Init(va_list, ib, new parser::Relief());
}

}