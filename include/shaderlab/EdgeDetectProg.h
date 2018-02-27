#ifndef _SHADERLAB_EDGE_DETECT_PROG_H_
#define _SHADERLAB_EDGE_DETECT_PROG_H_

#include "shaderlab/FilterProgram.h"

namespace sl
{

class EdgeDetectProg : public FilterProgram
{
public:
	EdgeDetectProg(RenderContext& rc, int max_vertex,
		const CU_VEC<ur::VertexAttrib>& va_list, const std::shared_ptr<RenderBuffer>& ib);

	void SetBlend(float blend);

private:
	int m_blend;

}; // EdgeDetectProg

}

#endif // _SHADERLAB_EDGE_DETECT_PROG_H_