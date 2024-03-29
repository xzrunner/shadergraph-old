#ifndef _SHADERLAB_COL_GRADING_PROG_H_
#define _SHADERLAB_COL_GRADING_PROG_H_

#include "shaderlab/FilterProgram.h"

namespace sl
{

class ColGradingProg : public FilterProgram
{
public:
	ColGradingProg(RenderContext& rc, int max_vertex,
		const CU_VEC<ur::VertexAttrib>& va_list, const std::shared_ptr<RenderBuffer>& ib);

	virtual void Bind() override;

	void SetLUTTex(int tex);

	bool IsTexValid() const { return m_lut_tex != 0; }

private:
	int m_lut_tex;

}; // ColGradingProg

}

#endif // _SHADERLAB_COL_GRADING_PROG_H_