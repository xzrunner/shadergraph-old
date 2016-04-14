#ifndef _SHADERLAB_SHAPE_SHADER_H_
#define _SHADERLAB_SHAPE_SHADER_H_

#include "Shader.h"
#include "Uniform.h"

#include <stdint.h>

namespace sl
{

class RenderContext;
class RenderShader;

class ShapeShader : public Shader
{
public:
	ShapeShader(RenderContext* rc);	
	virtual ~ShapeShader();

	virtual void Bind() const;
	virtual void UnBind() const;
	virtual void Commit() const;

private:
	static const int MAX_VERTICES = 4096;

private:
	struct Vertex
	{
		float vx, vy;
		uint32_t color;
	}; // Vertex

private:
	RenderShader* m_shader;

	UniformMat4 m_modelview, m_project;

	int m_color;

}; // ShapeShader

}

#endif // _SHADERLAB_SHAPE_SHADER_H_