#include "GaussianBlurProg.h"
#include "../render/RenderShader.h"
#include "../parser/GaussianBlur.h"

namespace sl
{

GaussianBlurProg::GaussianBlurProg(RenderContext* rc, int max_vertex, 
								   const std::vector<VertexAttrib>& va_list, RenderBuffer* ib)
	: FilterProgram(rc, max_vertex)
{
	Init(va_list, ib, new parser::GaussianBlur());
}

}