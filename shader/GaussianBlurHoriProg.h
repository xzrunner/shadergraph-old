#ifndef _SHADERLAB_GAUSSIAN_BLUR_HORI_PROG_H_
#define _SHADERLAB_GAUSSIAN_BLUR_HORI_PROG_H_

#include "FilterProgram.h"

namespace sl
{

class GaussianBlurHoriProg : public FilterProgram
{
public:
	GaussianBlurHoriProg(RenderContext* rc, int max_vertex, 
		const std::vector<VertexAttrib>& va_list, RenderBuffer* ib);

}; // GaussianBlurHoriProg

}

#endif // _SHADERLAB_GAUSSIAN_BLUR_HORI_PROG_H_