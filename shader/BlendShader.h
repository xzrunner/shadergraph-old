#ifndef _SHADERLAB_BLEND_SHADER_H_
#define _SHADERLAB_BLEND_SHADER_H_

#include "Shader.h"

#include <stdint.h>

namespace sl
{

class BlendShader : public Shader
{
public:
	BlendShader(RenderContext* rc);
	virtual ~BlendShader();

	virtual void Bind() const;
	virtual void UnBind() const;
	virtual void Commit() const;
	virtual void BindTexture();

	void SetColor(uint32_t color, uint32_t additive);
	void SetColorMap(uint32_t rmap, uint32_t gmap, uint32_t bmap);

	void SetMode(int mode);

	void Draw(const float* positions, const float* texcoords, 
		const float* texcoords_base, int texid, int texid_base) const;

}; // BlendShader

}

#endif // _SHADERLAB_BLEND_SHADER_H_