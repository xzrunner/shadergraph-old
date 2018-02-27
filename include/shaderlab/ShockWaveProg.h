#ifndef _SHADERLAB_SHOCK_WAVE_PROG_H_
#define _SHADERLAB_SHOCK_WAVE_PROG_H_

#include "shaderlab/FilterProgram.h"

namespace sl
{

class ShockWaveProg : public FilterProgram
{
public:
	ShockWaveProg(ShaderMgr& shader_mgr, int max_vertex,
		const CU_VEC<ur::VertexAttrib>& va_list, const std::shared_ptr<RenderBuffer>& ib);

	virtual void UpdateTime(float time) override;

	void SetCenter(float center[2]);

	void SetFactor(float params[3]);

private:
	int m_time;

	int m_center, m_params;

}; // ShockWaveProg

}

#endif // _SHADERLAB_SHOCK_WAVE_PROG_H_