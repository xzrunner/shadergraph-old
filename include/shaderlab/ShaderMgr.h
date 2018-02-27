#ifndef _SHADERLAB_SHADER_MGR_H_
#define _SHADERLAB_SHADER_MGR_H_

#include "shaderlab/ShaderType.h"

#include <cu/cu_stl.h>

#include <boost/noncopyable.hpp>

#include <stddef.h>

namespace ur { class RenderContext; }

namespace sl
{

class Shader;
class RenderShader;

class ShaderMgr : boost::noncopyable
{
public:
	ShaderMgr(ur::RenderContext& rc);
	~ShaderMgr();

	const ur::RenderContext& GetContext() const { return m_rc; }
	ur::RenderContext& GetContext() { return m_rc; }

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

	RenderShader* CreateRenderShader();
	void BindRenderShader(RenderShader* shader, int type = -1);

	void FlushShader();
	void FlushRenderShader();
	
private:
	ur::RenderContext& m_rc;

	Shader* m_shaders[MAX_SHADER];
	int m_curr_shader;

	CU_VEC<RenderShader*> m_render_shaders;
	RenderShader* m_curr_render_shader;

}; // ShaderMgr

}

#endif // _SHADERLAB_SHADER_MGR_H_