#ifndef _SHADERLAB_SWIRL_PROG_H_
#define _SHADERLAB_SWIRL_PROG_H_

#include "shaderlab/FilterProgram.h"

namespace sl
{

class SwirlProg : public FilterProgram
{
public:
	SwirlProg(ur::RenderContext* rc, int max_vertex, 
		const CU_VEC<ur::VertexAttrib>& va_list, const std::shared_ptr<RenderBuffer>& ib);

	void SetRadius(float radius);
	void SetAngle(float angle);

	void SetCenter(float cnenter[2]);

private:
	int m_radius, m_angle;
	int m_center;

}; // SwirlProg

}

#endif // _SHADERLAB_SWIRL_PROG_H_