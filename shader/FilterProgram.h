#ifndef _SHADERLAB_FILTER_PROGRAM_H_
#define _SHADERLAB_FILTER_PROGRAM_H_

#include "ShaderProgram.h"

namespace sl
{

class FilterProgram : public ShaderProgram
{
public:
	FilterProgram(RenderContext* rc, int max_vertex);
	
	virtual void UpdateTime(float time) {}

	virtual void BindTexture() {}

protected:
	void Init(const std::vector<VertexAttrib>& va_list, RenderBuffer* ib, 
		parser::Node* pn);

}; // FilterProgram

}

#endif] // _SHADERLAB_FILTER_PROGRAM_H_