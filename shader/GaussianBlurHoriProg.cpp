#include "GaussianBlurHoriProg.h"
#include "../render/RenderShader.h"
#include "../parser/GaussianBlurHori.h"

namespace sl
{

GaussianBlurHoriProg::GaussianBlurHoriProg(RenderContext* rc, int max_vertex, 
										   const std::vector<VertexAttrib>& va_list, RenderBuffer* ib)
	: FilterProgram(rc, max_vertex)
{
	Init(va_list, ib, new parser::GaussianBlurHori());
}

}