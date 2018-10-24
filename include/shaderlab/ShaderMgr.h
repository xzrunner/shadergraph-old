#ifndef _SHADERLAB_SHADER_MGR_H_
#define _SHADERLAB_SHADER_MGR_H_

#include "shaderlab/ShaderType.h"

#include <cu/cu_stl.h>

#include <boost/noncopyable.hpp>

#include <stddef.h>

namespace sl
{

class Shader;
class RenderShader;
class RenderContext;

class ShaderMgr : boost::noncopyable
{
public:
	ShaderMgr();
	~ShaderMgr();

	void CreateShader(ShaderType type, Shader* shader);
	void ReleaseShader(ShaderType type);

	void SetShader(ShaderType type);
	Shader* GetShader() const {
		return m_curr_shader == -1 ? nullptr : m_shaders[m_curr_shader];
	}
	Shader* GetShader(ShaderType type) const {
		return m_shaders[type];
	}
	ShaderType GetShaderType() const {
		return m_curr_shader == -1 ? MAX_SHADER : (ShaderType)m_curr_shader;
	}

	RenderShader* CreateRenderShader(RenderContext& rc);
	void BindRenderShader(RenderShader* shader, int type = -1);

	void FlushShader();

private:
	Shader* m_shaders[MAX_SHADER];
	int m_curr_shader;

	CU_VEC<RenderShader*> m_render_shaders;
	RenderShader* m_curr_render_shader;

}; // ShaderMgr

}

#endif // _SHADERLAB_SHADER_MGR_H_