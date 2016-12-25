#include "OutlineProg.h"
#include "../render/RenderShader.h"
#include "../parser/Outline.h"

namespace sl
{

OutlineProg::OutlineProg(ur::IRenderContext* rc, int max_vertex, 
						 const std::vector<ur::VertexAttrib>& va_list, RenderBuffer* ib)
	: FilterProgram(rc, max_vertex)
{
	Init(va_list, ib, new parser::Outline());
}

}