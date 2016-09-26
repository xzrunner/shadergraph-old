#ifndef _SHADERLAB_COL_GRADING_PROG_H_
#define _SHADERLAB_COL_GRADING_PROG_H_

#include "FilterProgram.h"

namespace sl
{

class ColGradingProg : public FilterProgram
{
public:
	ColGradingProg(RenderContext* rc, int max_vertex, 
		const std::vector<VertexAttrib>& va_list, RenderBuffer* ib);

	void SetLUTTex(int tex);

private:
	int m_lut_tex;

}; // ColGradingProg

}

#endif // _SHADERLAB_COL_GRADING_PROG_H_