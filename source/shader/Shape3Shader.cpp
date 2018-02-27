#include "shaderlab/Shape3Shader.h"
#include "shaderlab/ShaderProgram.h"
#include "shaderlab/SubjectMVP3.h"
#include "shaderlab/RenderShader.h"
#include "shaderlab/StackAllocator.h"
#include "shaderlab/ShaderMgr.h"
#include "shaderlab/RenderContext.h"

#include <unirender/RenderContext.h>

namespace sl
{

static const int MAX_VERTICES = 4096;

Shape3Shader::Shape3Shader(RenderContext& rc)
	: ShapeShader(rc)
{
	rc.GetContext().SetClearFlag(ur::MASKC | ur::MASKD);

	InitProg(3, MAX_VERTICES);
}

void Shape3Shader::Bind() const
{
	ShapeShader::Bind();

	m_rc.GetContext().EnableDepth(true);
	m_rc.GetContext().SetDepthFormat(ur::DEPTH_LESS_EQUAL);
}

void Shape3Shader::UnBind() const
{
	ShapeShader::UnBind();

	m_rc.GetContext().EnableDepth(false);
}

void Shape3Shader::Draw(const float* positions, int count) const
{
	StackAllocator* alloc = StackAllocator::Instance();
	int sz = m_prog->GetVertexSize() * count;
	alloc->Reserve(sz);
	void* buf = alloc->Alloc(sz);
	uint8_t* ptr = (uint8_t*)buf;
 	for (int i = 0; i < count; ++i) 
 	{
 		memcpy(ptr, &positions[i * 3], sizeof(float));
 		ptr += sizeof(float);
 		memcpy(ptr, &positions[i * 3 + 1], sizeof(float));
 		ptr += sizeof(float);
		memcpy(ptr, &positions[i * 3 + 2], sizeof(float));
		ptr += sizeof(float);
 		memcpy(ptr, &m_color, sizeof(m_color));
 		ptr += sizeof(m_color);
 	}
 	m_prog->GetShader()->Draw(buf, count);
	alloc->Free(buf);
}

void Shape3Shader::Draw(float x, float y, float z, bool dummy) const
{
	uint8_t buf[sizeof(float) * 3 + sizeof(int)];
	uint8_t* ptr = buf;
	memcpy(ptr, &x, sizeof(float));
	ptr += sizeof(float);
	memcpy(ptr, &y, sizeof(float));
	ptr += sizeof(float);
	memcpy(ptr, &z, sizeof(float));
	ptr += sizeof(float);
	if (dummy) {
		memset(ptr, 0, sizeof(uint32_t));
	} else {
		memcpy(ptr, &m_color, sizeof(uint32_t));
	}
	ptr += sizeof(uint32_t);
	m_prog->GetShader()->Draw(buf, 1);
}

void Shape3Shader::InitMVP(ObserverMVP* mvp) const
{
	m_rc.GetSubMVP3().Register(mvp);
}

}