#ifndef _SHADERLAB_SHADER_H_
#define _SHADERLAB_SHADER_H_

namespace ur { class IRenderContext; }

namespace sl
{

class Shader
{
public:
	Shader(ur::IRenderContext* rc) : m_rc(rc) {}
	virtual ~Shader() {}
	virtual void Bind() const = 0;
	virtual void UnBind() const = 0;
	virtual void Commit() const = 0;

protected:
	ur::IRenderContext* m_rc;

}; // Shader

}

#endif // _SHADERLAB_SHADER_H_