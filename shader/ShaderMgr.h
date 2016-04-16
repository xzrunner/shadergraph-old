#ifndef _SHADERLAB_SHADER_MGR_H_
#define _SHADERLAB_SHADER_MGR_H_

#include "ShaderType.h"

#include <stddef.h>

namespace sl
{

class RenderContext;
class Shader;

class ShaderMgr
{
public:
	void CreateContext(int max_texture);
	void ReleaseContext();
	RenderContext* GetContext() { return m_rc; }

	void CreateShader(ShaderType type, Shader* shader);

	void SetShader(ShaderType type);
	Shader* GetShader() const {
		return m_curr_shader == -1 ? NULL : m_shaders[m_curr_shader];
	}
	Shader* GetShader(ShaderType type) const {
		return m_shaders[type];
	}
	ShaderType GetShaderType() const {
		return m_curr_shader == -1 ? MAX_SHADER : (ShaderType)m_curr_shader;
	}
	
	static ShaderMgr* Instance();

private:
	ShaderMgr();
	~ShaderMgr();

private:
	RenderContext* m_rc;

	Shader* m_shaders[MAX_SHADER];
	int m_curr_shader;

private:
	static ShaderMgr* m_instance;

}; // ShaderMgr

}

#endif // _SHADERLAB_SHADER_MGR_H_