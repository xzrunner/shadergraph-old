#ifndef _SHADERLAB_SHAPE2_SHADER_H_
#define _SHADERLAB_SHAPE2_SHADER_H_

#include "shaderlab/ShapeShader.h"

namespace sl
{

class Shape2Shader : public ShapeShader
{
public:
	Shape2Shader(RenderContext& rc);

	void Draw(const float* positions, int count) const;
	void Draw(const float* positions, const uint32_t* colors, int count) const;
	void Draw(float x, float y, bool dummy) const;
	void Draw(float x, float y, uint32_t color, bool dummy) const;

protected:
	virtual void InitMVP(ObserverMVP* mvp) const override;

}; // Shape2Shader

}

#endif // _SHADERLAB_SHAPE2_SHADER_H_