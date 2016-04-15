#include "ShaderMgr.h"
#include "Shader.h"
#include "render/RenderContext.h"

#include <stddef.h>
#include <string.h>

namespace sl
{

ShaderMgr* ShaderMgr::m_instance = NULL;

ShaderMgr* ShaderMgr::Instance()
{
	if (!m_instance) {
		m_instance = new ShaderMgr;
	}
	return m_instance;
}

ShaderMgr::ShaderMgr()
	: m_rc(NULL)
	, m_curr_shader(-1)
{
	memset(m_shaders, 0, sizeof(m_shaders));
}

ShaderMgr::~ShaderMgr()
{
	if (m_rc) {
		delete m_rc;
	}
	for (int i = 0, n = MAX_SHADER; i < n; ++i) {
		if (m_shaders[i]) {
			delete m_shaders[i];
		}
	}
}

void ShaderMgr::CreateContext(int max_texture)
{
	if (m_rc) {
		delete m_rc;
	}
	m_rc = new RenderContext(max_texture);
}

void ShaderMgr::ReleaseContext()
{
	delete m_rc;
	m_rc = NULL;
}

void ShaderMgr::CreateShader(ShaderType type, Shader* shader)
{
	if (m_shaders[type]) {
		delete m_shaders[type];
	}
	m_shaders[type] = shader;
}

void ShaderMgr::SetShader(ShaderType type)
{
	if (type == m_curr_shader) {
		return;
	}

	if (m_shaders[m_curr_shader]) {
		m_shaders[m_curr_shader]->Commit();
		m_shaders[m_curr_shader]->UnBind();
	}
	m_curr_shader = type;
	if (m_shaders[m_curr_shader]) {
		m_shaders[m_curr_shader]->Bind();
	}
}

ShaderType ShaderMgr::GetShader() const
{
	return m_curr_shader == -1 ? MAX_SHADER : (ShaderType)m_curr_shader;
}

}