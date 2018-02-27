#ifndef _SHADERLAB_GAUSSIAN_BLUR_VERT_PROG_H_
#define _SHADERLAB_GAUSSIAN_BLUR_VERT_PROG_H_

#include "shaderlab/FilterProgram.h"

namespace sl
{

class GaussianBlurVertProg : public FilterProgram
{
public:
	GaussianBlurVertProg(ShaderMgr& shader_mgr, int max_vertex,
		const CU_VEC<ur::VertexAttrib>& va_list, const std::shared_ptr<RenderBuffer>& ib);

	void SetTexHeight(float height);

private:
	int m_tex_height_id;
	float m_tex_height_val;

}; // GaussianBlurVertProg

}

#endif // _SHADERLAB_GAUSSIAN_BLUR_VERT_PROG_H_