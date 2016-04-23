#ifndef _SHADERLAB_GAUSSIAN_BLUR_PROG_H_
#define _SHADERLAB_GAUSSIAN_BLUR_PROG_H_

#include "FilterProgram.h"

namespace sl
{

class GaussianBlurProg : public FilterProgram
{
public:
	GaussianBlurProg(RenderContext* rc, int max_vertex, 
		const std::vector<VertexAttrib>& va_list, RenderBuffer* ib);

}; // GaussianBlurProg

}

#endif // _SHADERLAB_GAUSSIAN_BLUR_PROG_H_