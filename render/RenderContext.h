#ifndef _SHADERLAB_RENDER_CONTEXT_H_
#define _SHADERLAB_RENDER_CONTEXT_H_

#include "../utility/typedef.h"

#include <vector>

struct render;

namespace sl
{

class RenderShader;

class RenderContext
{
public:
	RenderContext(int max_texture);
	~RenderContext();

	render* GetEJRender() { return m_ej_render; }

	RenderShader* CreateShader();

	void SetBlend(int m1, int m2);
	void SetBlendEquation(int func);
	void SetDefaultBlend();

	void SetTexture(int id, int channel);
	int  GetTexture() const { return m_textures[0]; }
	void SetTarget(int id) {
		//	render_set(RS->R, TARGET, id, 0);
		m_target = id;	
	}
	int  GetTarget() const {
		//	return render_get(RS->R, TARGET, 0);
		return m_target;
	}

	void BindShader(RenderShader* shader, int type = -1);

	void SetClearFlag(int flag);
	void Clear(unsigned long argb);

	int GetShaderVersion() const;

	void ClearTextureCache();

	void EnableScissor(int enable);
	void SetScissor(int x, int y, int width, int height);

private:
	static const int MAX_TEXTURE_CHANNEL	= 8;
	static const int MAX_SHADER				= 64;

private:
	render* m_ej_render;

	std::vector<RenderShader*> m_shaders;
	RenderShader* m_curr;

	int m_textures[MAX_TEXTURE_CHANNEL];
	int m_blend_src, m_blend_dst;
	int m_blend_func;
	RID m_target;

	int m_clear_mask;

}; // RenderContext

}

#endif // _SHADERLAB_RENDER_CONTEXT_H_