#ifndef _SHADERLAB_BLUR_PROG_H_
#define _SHADERLAB_BLUR_PROG_H_

#include "shaderlab/FilterProgram.h"

namespace sl
{

class BlurProg : public FilterProgram
{
public:
	BlurProg(ShaderMgr& shader_mgr, int max_vertex,
		const CU_VEC<ur::VertexAttrib>& va_list, const std::shared_ptr<RenderBuffer>& ib);

	void SetRadius(float r);

private:
	int m_radius;

}; // BlurProg

}

#endif // _SHADERLAB_BLUR_PROG_H_