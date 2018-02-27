#ifndef _SHADERLAB_RELIEF_PROG_H_
#define _SHADERLAB_RELIEF_PROG_H_

#include "shaderlab/FilterProgram.h"

namespace sl
{

class ReliefProg : public FilterProgram
{
public:
	ReliefProg(ShaderMgr& shader_mgr, int max_vertex,
		const CU_VEC<ur::VertexAttrib>& va_list, const std::shared_ptr<RenderBuffer>& ib);

}; // ReliefProg

}

#endif // _SHADERLAB_RELIEF_PROG_H_