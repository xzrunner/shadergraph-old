#include "GaussianBlurVertProg.h"
#include "../render/RenderShader.h"
#include "../parser/GaussianBlurVert.h"

namespace sl
{

GaussianBlurVertProg::GaussianBlurVertProg(RenderContext* rc, int max_vertex, 
										   const std::vector<VertexAttrib>& va_list, RenderBuffer* ib)
	: FilterProgram(rc, max_vertex)
{
	Init(va_list, ib, new parser::GaussianBlurVert());
}

}