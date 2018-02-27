#include "shaderlab/GrayProg.h"
#include "shaderlab/RenderShader.h"
#include "shaderlab/Gray.h"

namespace sl
{

GrayProg::GrayProg(RenderContext& rc, int max_vertex,
				   const CU_VEC<ur::VertexAttrib>& va_list, 
				   const std::shared_ptr<RenderBuffer>& ib, parser::Node* pre)
	: FilterProgram(rc, max_vertex)
{
	if (pre) {
		Init(va_list, ib, new parser::Gray(), pre);
	} else {
		Init(va_list, ib, new parser::Gray());
	}
}

}