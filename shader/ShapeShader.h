#ifndef _SHADERLAB_SHAPE_SHADER_H_
#define _SHADERLAB_SHAPE_SHADER_H_

#include "Shader.h"
#include "Uniform.h"

#include <stdint.h>

namespace sl
{

namespace parser { class Shader; }

class RenderShader;
class ObserverMVP;

class ShapeShader : public Shader
{
public:
	ShapeShader(RenderContext* rc);	
	virtual ~ShapeShader();

	virtual void Bind() const;
	virtual void UnBind() const;
	virtual void Commit() const;

	void SetColor(uint32_t color);
	void SetType(int type);

	void Draw(const float* positions, int count) const;
	void Draw(float x, float y, bool dummy) const;

private:
	void Init();

private:
	parser::Shader* m_parser;

	RenderShader* m_shader;

	ObserverMVP* m_mvp;

	int m_vertex_sz;

	uint32_t m_color;

}; // ShapeShader

}

#endif // _SHADERLAB_SHAPE_SHADER_H_