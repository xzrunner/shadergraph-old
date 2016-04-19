#include "Shape2Shader.h"
#include "ShaderProgram.h"
#include "SubjectMVP2.h"
#include "render/RenderShader.h"
#include "utility/StackAllocator.h"

namespace sl
{

static const int MAX_VERTICES = 4096;

Shape2Shader::Shape2Shader(RenderContext* rc)
	: ShapeShader(rc)
{
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

void Shape2Shader::Draw(float x, float y, bool dummy) const
{
	uint8_t buf[sizeof(float) * 2 + sizeof(int)];
	uint8_t* ptr = buf;
	memcpy(ptr, &x, sizeof(float));
	ptr += sizeof(float);
	memcpy(ptr, &y, sizeof(float));
	ptr += sizeof(float);
	memcpy(ptr, &m_color, sizeof(uint32_t));
	ptr += sizeof(uint32_t);
	m_prog->GetShader()->Draw(buf, 1);
}

void Shape2Shader::InitMVP(ObserverMVP* mvp) const
{
	SubjectMVP2::Instance()->Register(mvp);
}

}