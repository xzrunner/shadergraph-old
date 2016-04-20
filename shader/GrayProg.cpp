#include "GrayProg.h"
#include "render/RenderShader.h"
#include "parser/Gray.h"

namespace sl
{

GrayProg::GrayProg(RenderContext* rc, int max_vertex, 
				   const std::vector<VertexAttrib>& va_list, RenderBuffer* ib)
	: FilterProgram(rc, max_vertex)
{
	Init(va_list, ib, new parser::Gray());
}

}