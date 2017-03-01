#include "ObserverMVP.h"
#include "SL_RenderShader.h"

#include <unirender/ur_typedef.h>

namespace sl
{

ObserverMVP::ObserverMVP(RenderShader* shader)
	: m_shader(shader)
{
}

void ObserverMVP::SetModelview(const sm::mat4* mat)
{
	m_shader->SetUniform(m_modelview, ur::UNIFORM_FLOAT44, mat->x);
}

void ObserverMVP::SetProjection(const sm::mat4* mat)
{
	m_shader->SetUniform(m_projection, ur::UNIFORM_FLOAT44, mat->x);
}

}