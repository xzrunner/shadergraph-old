#ifndef _SHADERLAB_GRAY_PROG_H_
#define _SHADERLAB_GRAY_PROG_H_

#include "FilterProgram.h"

namespace sl
{

class GrayProg : public FilterProgram
{
public:
	GrayProg(ur::RenderContext* rc, int max_vertex, 
		const std::vector<ur::VertexAttrib>& va_list, 
		const std::shared_ptr<RenderBuffer>& ib, parser::Node* pre = nullptr);

}; // GrayProg

}

#endif // _SHADERLAB_GRAY_PROG_H_