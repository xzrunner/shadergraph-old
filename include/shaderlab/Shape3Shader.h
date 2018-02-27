#ifndef _SHADERLAB_SHAPE3_SHADER_H_
#define _SHADERLAB_SHAPE3_SHADER_H_

#include "shaderlab/ShapeShader.h"

namespace sl
{

class Shape3Shader : public ShapeShader
{
public:
	Shape3Shader(RenderContext& rc);

	virtual void Bind() const override;
	virtual void UnBind() const override;

	void Draw(const float* positions, int count) const;
	void Draw(float x, float y, float z, bool dummy) const;

protected:
	virtual void InitMVP(ObserverMVP* mvp) const override;

}; // Shape3Shader

}

#endif // _SHADERLAB_SHAPE3_SHADER_H_