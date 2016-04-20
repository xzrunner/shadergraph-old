#include "OutlineProg.h"
#include "render/RenderShader.h"
#include "parser/Outline.h"

namespace sl
{

OutlineProg::OutlineProg(RenderContext* rc, int max_vertex, 
						 const std::vector<VertexAttrib>& va_list, RenderBuffer* ib)
	: FilterProgram(rc, max_vertex)
{
	Init(va_list, ib, new parser::Outline());
}

}