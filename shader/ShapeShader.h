#ifndef _SHADERLAB_SHAPE_SHADER_H_
#define _SHADERLAB_SHAPE_SHADER_H_

#include "Shader.h"
// #include "Uniform.h"

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

protected:
	virtual void InitMVP(ObserverMVP* mvp) const = 0;

	void InitProg(int position_sz, int max_vertex);

protected:
	RenderShader* m_shader;

	int m_vertex_sz;	

	uint32_t m_color;

private:
	parser::Shader* m_parser;

	ObserverMVP* m_mvp;

}; // ShapeShader

}

#endif // _SHADERLAB_SHAPE_SHADER_H_