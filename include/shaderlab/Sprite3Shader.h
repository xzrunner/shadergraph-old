#ifndef _SHADERLAB_SPRITE3_SHADER_H_
#define _SHADERLAB_SPRITE3_SHADER_H_

#include "shaderlab/SpriteShader.h"

namespace sl
{

class Sprite3Shader : public SpriteShader
{
public:
	Sprite3Shader(ur::RenderContext* rc);	

	virtual bool Commit() const override;

	void Draw(const float* positions, const float* texcoords, int texid) const;

protected:
	virtual void InitMVP(ObserverMVP* mvp) const override;

private:
	struct Vertex
	{
		float vx, vy, vz;
		float tx, ty;
		uint32_t color, additive;
		uint32_t rmap, gmap, bmap;
	};

private:
	Vertex* m_vertex_buf;

}; // Sprite3Shader

}

#endif // _SHADERLAB_SPRITE3_SHADER_H_