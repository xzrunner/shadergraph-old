#ifndef _SHADERLAB_GAUSSIAN_BLUR_VERT_PROG_H_
#define _SHADERLAB_GAUSSIAN_BLUR_VERT_PROG_H_

#include "FilterProgram.h"

namespace sl
{

class GaussianBlurVertProg : public FilterProgram
{
public:
	GaussianBlurVertProg(RenderContext* rc, int max_vertex, 
		const std::vector<VertexAttrib>& va_list, RenderBuffer* ib);

}; // GaussianBlurVertProg

}

#endif // _SHADERLAB_GAUSSIAN_BLUR_VERT_PROG_H_