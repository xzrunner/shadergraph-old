#ifndef _SHADERLAB_SHADER_H_
#define _SHADERLAB_SHADER_H_

namespace sl
{

class ShaderMgr;

class Shader
{
public:
	Shader(ShaderMgr& shader_mgr) 
		: m_shader_mgr(shader_mgr) {}
	virtual ~Shader() {}
	virtual void Bind() const = 0;
	virtual void UnBind() const = 0;
	virtual bool Commit() const = 0;

protected:
	ShaderMgr& m_shader_mgr;

}; // Shader

}

#endif // _SHADERLAB_SHADER_H_