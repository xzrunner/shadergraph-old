#ifndef _SHADERLAB_SHADER_H_
#define _SHADERLAB_SHADER_H_

namespace sl
{

class RenderContext;

class Shader
{
public:
	Shader(RenderContext& rc)
		: m_rc(rc) {}
	virtual ~Shader() {}
	virtual void Bind() const = 0;
	virtual void UnBind() const = 0;
	virtual bool Commit() const = 0;

protected:
	RenderContext& m_rc;

}; // Shader

}

#endif // _SHADERLAB_SHADER_H_