#include "shaderlab/Shape2Shader.h"
#include "shaderlab/ShaderProgram.h"
#include "shaderlab/SubjectMVP2.h"
#include "shaderlab/RenderShader.h"
#include "shaderlab/StackAllocator.h"
#include "shaderlab/ShaderMgr.h"
#include "shaderlab/RenderContext.h"

#include <unirender/RenderContext.h>

namespace sl
{

static const int MAX_VERTICES = 4096;

Shape2Shader::Shape2Shader(RenderContext& rc)
	: ShapeShader(rc)
{
	m_rc.GetContext().SetClearFlag(ur::MASKC);

	InitProg(2, MAX_VERTICES);
}

void Shape2Shader::Draw(const float* positions, int count) const
{
	StackAllocator* alloc = StackAllocator::Instance();
	int sz = m_prog->GetVertexSize() * count;
	alloc->Reserve(sz);
	void* buf = alloc->Alloc(sz);
	uint8_t* ptr = (uint8_t*)buf;
 	for (int i = 0; i < count; ++i) 
 	{
 		memcpy(ptr, &positions[i * 2], sizeof(float));
 		ptr += sizeof(float);
 		memcpy(ptr, &positions[i * 2 + 1], sizeof(float));
 		ptr += sizeof(float);
 		memcpy(ptr, &m_color, sizeof(m_color));
 		ptr += sizeof(m_color);
 	}
 	m_prog->GetShader()->Draw(buf, count);
	alloc->Free(buf);
}

void Shape2Shader::Draw(const float* positions, const uint32_t* colors, int count) const
{
	StackAllocator* alloc = StackAllocator::Instance();
	int sz = m_prog->GetVertexSize() * count;
	alloc->Reserve(sz);
	void* buf = alloc->Alloc(sz);
	uint8_t* ptr = (uint8_t*)buf;
	for (int i = 0; i < count; ++i) 
	{
		memcpy(ptr, &positions[i * 2], sizeof(float));
		ptr += sizeof(float);
		memcpy(ptr, &positions[i * 2 + 1], sizeof(float));
		ptr += sizeof(float);
		memcpy(ptr, &colors[i], sizeof(uint32_t));
		ptr += sizeof(uint32_t);
	}
	m_prog->GetShader()->Draw(buf, count);
	alloc->Free(buf);
}

void Shape2Shader::Draw(float x, float y, bool dummy) const
{
	uint8_t buf[sizeof(float) * 2 + sizeof(int)];
	uint8_t* ptr = buf;
	memcpy(ptr, &x, sizeof(float));
	ptr += sizeof(float);
	memcpy(ptr, &y, sizeof(float));
	ptr += sizeof(float);
	if (dummy) {
		memset(ptr, 0, sizeof(uint32_t));
	} else {
		memcpy(ptr, &m_color, sizeof(uint32_t));
	}
	ptr += sizeof(uint32_t);
	m_prog->GetShader()->Draw(buf, 1);
}

void Shape2Shader::InitMVP(ObserverMVP* mvp) const
{
	m_rc.GetSubMVP2().Register(mvp);
}

}