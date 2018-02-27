#ifndef _SHADERLAB_FILTER_PROGRAM_H_
#define _SHADERLAB_FILTER_PROGRAM_H_

#include "shaderlab/ShaderProgram.h"

namespace sl
{

class FilterProgram : public ShaderProgram
{
public:
	FilterProgram(ShaderMgr& shader_mgr, int max_vertex);
	
	virtual void UpdateTime(float time) {}

protected:
	void Init(const CU_VEC<ur::VertexAttrib>& va_list, const std::shared_ptr<RenderBuffer>& ib, 
		parser::Node* pn, parser::Node* pre_pn = nullptr);

}; // FilterProgram

}

#endif // _SHADERLAB_FILTER_PROGRAM_H_