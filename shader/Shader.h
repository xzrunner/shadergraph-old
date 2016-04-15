#ifndef _SHADERLAB_SHADER_H_
#define _SHADERLAB_SHADER_H_

namespace sl
{

class Shader
{
public:
	virtual ~Shader() {}
	virtual void Bind() const = 0;
	virtual void UnBind() const = 0;
	virtual void Commit() const = 0;

}; // Shader

}

#endif // _SHADERLAB_SHADER_H_