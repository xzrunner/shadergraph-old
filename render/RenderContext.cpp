#include "RenderContext.h"
#include "RenderShader.h"
#include "RenderConst.h"
#include "../shader/ShaderMgr.h"
#include "../shader/Shader.h"

#include <render/render.h>
#include <render/blendmode.h>

#include <stddef.h>
#include <string.h>
#include <stdlib.h>

// #define DC_LOG

#ifdef DC_LOG
#include <iostream>
#endif // _DEBUG

namespace sl
{

RenderContext::RenderContext(int max_texture)
{
	struct render_init_args RA;
	// todo: config these args
	RA.max_buffer = 128;
	RA.max_layout = MAX_LAYOUT;
	RA.max_target = 128;
	RA.max_texture = max_texture;
	RA.max_shader = MAX_SHADER;

	int smz = render_size(&RA);
	m_ej_render = (struct render*)malloc(smz);
	m_ej_render = render_init(&RA, m_ej_render, smz);

	m_shaders.reserve(MAX_SHADER);
	m_curr = NULL;

	memset(m_textures, 0, sizeof(m_textures));
	m_blend_src = BLEND_ONE;
	m_blend_dst = BLEND_ONE_MINUS_SRC_ALPHA;
	render_set_blendfunc(m_ej_render, (BLEND_FORMAT)m_blend_src, (BLEND_FORMAT)m_blend_dst);
	m_blend_func = BLEND_FUNC_ADD;
	render_set_blendeq(m_ej_render, (BLEND_FUNC)m_blend_func);
	m_target = render_query_target();

	m_clear_mask = 0;
}

RenderContext::~RenderContext()
{
	for (int i = 0, n = m_shaders.size(); i < n; ++i) {
		if (m_shaders[i]) {
			delete m_shaders[i];
		}
	}
	render_exit(m_ej_render);
	free(m_ej_render);
}

RenderShader* RenderContext::CreateShader()
{
	if (m_shaders.size() < MAX_SHADER) {
		RenderShader* shader = new RenderShader(m_ej_render);
		m_shaders.push_back(shader);
		return shader;
	} else {
		return NULL;
	}
}

void RenderContext::SetBlend(int m1, int m2)
{
	if (m1 == m_blend_src && m2 == m_blend_dst) {
		return;
	}

	ShaderMgr::Instance()->GetShader()->Commit();

	m_blend_src = m1;
	m_blend_dst = m2;
	render_set_blendfunc(m_ej_render, (BLEND_FORMAT)m_blend_src, (BLEND_FORMAT)m_blend_dst);
}

void RenderContext::SetBlendEquation(int func)
{
	if (func == m_blend_func) {
		return;
	}

	ShaderMgr::Instance()->GetShader()->Commit();

	m_blend_func = func;
	render_set_blendeq(m_ej_render, (BLEND_FUNC)m_blend_func);
}

void RenderContext::SetDefaultBlend()
{
	SetBlend(BLEND_ONE, BLEND_ONE_MINUS_SRC_ALPHA);
	SetBlendEquation(BLEND_FUNC_ADD);
}

void RenderContext::SetTexture(int id, int channel)
{
	if (channel < 0 || channel >= MAX_TEXTURE_CHANNEL || m_textures[channel] == id) {
		return;
	}

	if (m_curr) {
#ifdef DC_LOG
		std::cout << "DC: SetTexture\n";
#endif // DC_LOG
		m_curr->Commit();
	}

	m_textures[channel] = id;
	render_set(m_ej_render, TEXTURE, id, channel);
}

void RenderContext::BindShader(RenderShader* shader)
{
	if (m_curr == shader) {
		return;
	}

	if (m_curr && m_curr->IsUniformChanged()) {
#ifdef DC_LOG
		std::cout << "DC: ShaderBind\n";
#endif // DC_LOG
		m_curr->Commit();
	}

	m_curr = shader;
	m_curr->Bind();
}

void RenderContext::SetClearFlag(int flag)
{
	m_clear_mask |= flag;
}

void RenderContext::Clear(unsigned long argb)
{
	render_clear(m_ej_render, CLEAR_MASK(m_clear_mask), argb);
}

int RenderContext::GetShaderVersion() const
{
	return render_version(m_ej_render);
}

void RenderContext::EnableScissor(int enable)
{
	render_enablescissor(m_ej_render, enable);
}

void RenderContext::ClearTextureCache()
{
	render_clear_texture_cache(m_ej_render);
}

}