#ifndef _SHADERLAB_SHAPE_SHADER_H_
#define _SHADERLAB_SHAPE_SHADER_H_

#include "shaderlab/Shader.h"

#include <stdint.h>

namespace sl
{

class ObserverMVP;
class ShaderProgram;

class ShapeShader : public Shader
{
public:
	ShapeShader(RenderContext& rc);
	virtual ~ShapeShader();

	virtual void Bind() const override;
	virtual void UnBind() const override;
	virtual bool Commit() const override;

	void SetColor(uint32_t color);
	void SetType(int type);

protected:
	virtual void InitMVP(ObserverMVP* mvp) const = 0;

	void InitProg(int position_sz, int max_vertex);

protected:
	ShaderProgram* m_prog;

	uint32_t m_color;

}; // ShapeShader

}

#endif // _SHADERLAB_SHAPE_SHADER_H_