#include "shaderlab/ShaderMgr.h"
#include "shaderlab/Shader.h"
#include "shaderlab/RenderShader.h"
#include "shaderlab/config.h"
#ifndef SL_DISABLE_STATISTICS
#include "shaderlab/StatDrawCall.h"
#endif // SL_DISABLE_STATISTICS

#include <string.h>

namespace sl
{

ShaderMgr::ShaderMgr()
	: m_curr_shader(-1)
{
	memset(m_shaders, 0, sizeof(m_shaders));

	m_render_shaders.reserve(MAX_SHADER);
	m_curr_render_shader = nullptr;
}

ShaderMgr::~ShaderMgr()
{
	for (int i = 0, n = MAX_SHADER; i < n; ++i) {
		if (m_shaders[i]) {
			delete m_shaders[i];
		}
	}
	for (int i = 0, n = m_render_shaders.size(); i < n; ++i) {
		if (m_render_shaders[i]) {
			delete m_render_shaders[i];
		}
	}
}

void ShaderMgr::CreateShader(ShaderType type, Shader* shader)
{
	if (m_shaders[type]) {
		delete m_shaders[type];
	}
	m_shaders[type] = shader;
}

void ShaderMgr::ReleaseShader(ShaderType type)
{
	if (m_shaders[type]) {
		delete m_shaders[type];
		m_shaders[type] = nullptr;
	}
}

void ShaderMgr::SetShader(ShaderType type)
{
	if (type == m_curr_shader) {
		return;
	}

	if (m_curr_shader != -1 && m_shaders[m_curr_shader]) {
		m_shaders[m_curr_shader]->Commit();
		m_shaders[m_curr_shader]->UnBind();
	}
	m_curr_shader = type;
	if (m_shaders[m_curr_shader]) {
		m_shaders[m_curr_shader]->Bind();
	}
}

RenderShader* ShaderMgr::CreateRenderShader(RenderContext& rc)
{
	RenderShader* shader = new RenderShader(rc);
	m_render_shaders.push_back(shader);
	return shader;
}

void ShaderMgr::BindRenderShader(RenderShader* shader, int type)
{
	if (m_curr_render_shader == shader) {
		return;
	}

	if (m_curr_render_shader && m_curr_render_shader->IsUniformChanged()) {
		bool changed = m_curr_render_shader->Commit();
#ifndef SL_DISABLE_STATISTICS
		if (changed) {
			StatDrawCall::Instance()->AddShader();
		}
#endif // SL_DISABLE_STATISTICS
	}

	m_curr_render_shader = shader;
	if (m_curr_render_shader) {
		m_curr_render_shader->Bind();
	}
}

void ShaderMgr::FlushShader()
{
	Shader* shader = GetShader();
	if (shader) {
		shader->Commit();
	}
}

}