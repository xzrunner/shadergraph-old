#ifndef _SHADERLAB_GRAY_PROG_H_
#define _SHADERLAB_GRAY_PROG_H_

#include "FilterProgram.h"

namespace sl
{

class GrayProg : public FilterProgram
{
public:
	GrayProg(RenderContext* rc, int max_vertex, 
		const std::vector<VertexAttrib>& va_list, RenderBuffer* ib);

}; // GrayProg

}

#endif // _SHADERLAB_GRAY_PROG_H_