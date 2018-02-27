#ifndef _SHADERLAB_GRAY_PROG_H_
#define _SHADERLAB_GRAY_PROG_H_

#include "shaderlab/FilterProgram.h"

namespace sl
{

class GrayProg : public FilterProgram
{
public:
	GrayProg(RenderContext& rc, int max_vertex,
		const CU_VEC<ur::VertexAttrib>& va_list, 
		const std::shared_ptr<RenderBuffer>& ib, parser::Node* pre = nullptr);

}; // GrayProg

}

#endif // _SHADERLAB_GRAY_PROG_H_