#include "GrayProg.h"
#include "../render/RenderShader.h"
#include "../parser/Gray.h"

namespace sl
{

GrayProg::GrayProg(RenderContext* rc, int max_vertex, 
				   const std::vector<VertexAttrib>& va_list, 
				   RenderBuffer* ib, parser::Node* pre)
	: FilterProgram(rc, max_vertex)
{
	if (pre) {
		Init(va_list, ib, new parser::Gray(), pre);
	} else {
		Init(va_list, ib, new parser::Gray());
	}
}

}