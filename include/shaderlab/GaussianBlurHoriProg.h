#ifndef _SHADERLAB_GAUSSIAN_BLUR_HORI_PROG_H_
#define _SHADERLAB_GAUSSIAN_BLUR_HORI_PROG_H_

#include "shaderlab/FilterProgram.h"

namespace sl
{

class GaussianBlurHoriProg : public FilterProgram
{
public:
	GaussianBlurHoriProg(RenderContext& rc, int max_vertex,
		const CU_VEC<ur::VertexAttrib>& va_list, const std::shared_ptr<RenderBuffer>& ib);

	void SetTexWidth(float width);

private:
	int m_tex_width_id;
	float m_tex_width_val;

}; // GaussianBlurHoriProg

}

#endif // _SHADERLAB_GAUSSIAN_BLUR_HORI_PROG_H_