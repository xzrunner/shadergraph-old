#ifndef _SHADERLAB_OUTLINE_PROG_H_
#define _SHADERLAB_OUTLINE_PROG_H_

#include "shaderlab/FilterProgram.h"

namespace sl
{

class OutlineProg : public FilterProgram
{
public:
	OutlineProg(RenderContext& rc, int max_vertex,
		const CU_VEC<ur::VertexAttrib>& va_list, const std::shared_ptr<RenderBuffer>& ib);

}; // OutlineProg

}

#endif // _SHADERLAB_OUTLINE_PROG_H_