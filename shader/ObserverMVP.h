#ifndef _SHADERLAB_OBSERVER_MVP_H_
#define _SHADERLAB_OBSERVER_MVP_H_

union sm_mat4;

namespace sl
{

class RenderShader;

class ObserverMVP
{
public:
	ObserverMVP(RenderShader* shader);

	void InitModelview(int id) { m_modelview = id; }
	void InitProjection(int id) { m_projection = id; }

	void SetModelview(const sm_mat4* mat);
	void SetProjection(const sm_mat4* mat);

private:
	RenderShader* m_shader;

	int m_modelview, m_projection;

}; // ObserverMVP

}

#endif // _SHADERLAB_OBSERVER_MVP_H_