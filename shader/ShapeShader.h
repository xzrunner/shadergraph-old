#ifndef _SHADERLAB_SHAPE_SHADER_H_
#define _SHADERLAB_SHAPE_SHADER_H_

#include "Shader.h"
#include "Uniform.h"

#include <stdint.h>

namespace sl
{

namespace parser { class Shader; }

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

	void SetProjection(int width, int height);
	void SetModelview(float x, float y, float sx, float sy);

	void SetColor(int color);
	void SetType(int type);

	void Draw(const float* positions, int count) const;
	void Draw(float x, float y, bool dummy) const;

private:
	void InitParser();

private:
	static const int MAX_VERTICES = 4096;

private:
// 	struct Vertex
// 	{
// 		float vx, vy;
// 		uint32_t color;
// 	}; // Vertex

private:
	parser::Shader* m_parser;

	RenderShader* m_shader;

	UniformMat4 m_modelview, m_project;

	int m_color;

}; // ShapeShader

}

#endif // _SHADERLAB_SHAPE_SHADER_H_