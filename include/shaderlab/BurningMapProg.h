#ifndef _SHADERLAB_BURNING_MAP_PROG_H_
#define _SHADERLAB_BURNING_MAP_PROG_H_

#include "shaderlab/FilterProgram.h"

namespace sl
{

class BurningMapProg : public FilterProgram
{
public:
	BurningMapProg(RenderContext& rc, int max_vertex,
		const CU_VEC<ur::VertexAttrib>& va_list, const std::shared_ptr<RenderBuffer>& ib);

	virtual void UpdateTime(float time) override;

	void SetLifeTime(float lifetime);

	void SetUpperTex(int tex);
	void SetHeightMapTex(int tex);
	void SetBorderGradientTex(int tex);

private:
	int m_lifetime, m_time;

	int m_upper_tex, m_height_map_tex, m_border_gradient_tex;

}; // BurningMapProg

}

#endif // _SHADERLAB_BURNING_MAP_PROG_H_