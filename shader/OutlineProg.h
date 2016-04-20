#ifndef _SHADERLAB_OUTLINE_PROG_H_
#define _SHADERLAB_OUTLINE_PROG_H_

#include "FilterProgram.h"

namespace sl
{

class OutlineProg : public FilterProgram
{
public:
	OutlineProg(RenderContext* rc, int max_vertex, 
		const std::vector<VertexAttrib>& va_list, RenderBuffer* ib);

}; // OutlineProg

}

#endif // _SHADERLAB_OUTLINE_PROG_H_