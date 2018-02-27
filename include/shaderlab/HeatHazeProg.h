#ifndef _SHADERLAB_HEAT_HAZE_PROG_H_
#define _SHADERLAB_HEAT_HAZE_PROG_H_

#include "shaderlab/FilterProgram.h"

namespace sl
{

class HeatHazeProg : public FilterProgram
{
public:
	HeatHazeProg(ShaderMgr& shader_mgr, int max_vertex,
		const CU_VEC<ur::VertexAttrib>& va_list, const std::shared_ptr<RenderBuffer>& ib);

	virtual void UpdateTime(float time) override;

	void SetFactor(float distortion, float rise);

	void SetDistortionMapTex(int tex);

private:
	int m_time;
	int m_distortion_factor, m_rise_factor;

	int m_distortion_map_tex;

}; // HeatHazeProg

}

#endif // _SHADERLAB_HEAT_HAZE_PROG_H_