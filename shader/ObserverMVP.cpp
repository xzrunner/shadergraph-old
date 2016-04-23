#include "ObserverMVP.h"
#include "../render/RenderShader.h"

#include <render/render.h>
#include <sm.h>

namespace sl
{

ObserverMVP::ObserverMVP(RenderShader* shader)
	: m_shader(shader)
{
}

void ObserverMVP::SetModelview(const sm_mat4* mat)
{
	m_shader->SetUniform(m_modelview, UNIFORM_FLOAT44, mat->x);
}

void ObserverMVP::SetProjection(const sm_mat4* mat)
{
	m_shader->SetUniform(m_projection, UNIFORM_FLOAT44, mat->x);
}

}