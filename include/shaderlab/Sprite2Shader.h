#ifndef _SHADERLAB_SPRITE2_SHADER_H_
#define _SHADERLAB_SPRITE2_SHADER_H_

#include "shaderlab/SpriteShader.h"

namespace sl
{

class Sprite2Shader : public SpriteShader
{
public:
	Sprite2Shader(RenderContext& rc);

	virtual bool Commit() const override;

	void DrawQuad(const float* positions, const float* texcoords, int texid) const;

protected:
	virtual void InitMVP(ObserverMVP* mvp) const override;

private:
	struct Vertex
	{
		float vx, vy;
		float tx, ty;
		uint32_t color, additive;
		uint32_t rmap, gmap, bmap;
	};

private:
	Vertex* m_vertex_buf;

}; // Sprite2Shader

}

#endif // _SHADERLAB_SPRITE2_SHADER_H_