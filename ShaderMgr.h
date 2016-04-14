#ifndef _SHADERLAB_SHADER_MGR_H_
#define _SHADERLAB_SHADER_MGR_H_

#include "ShaderType.h"

namespace sl
{

class RenderContext;
class Shader;

class ShaderMgr
{
public:
	void CreateContext(int max_texture);
	void ReleaseContext();
//	RenderContext* GetContext() { return m_rc; }

	void CreateShader(ShaderType type, Shader* shader);

	void SetShader(ShaderType type);
	ShaderType GetShader() const;
	
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