#include "ObserverMVP.h"
#include "../render/RenderShader.h"

#include <render/render.h>

namespace sl
{

ObserverMVP::ObserverMVP(RenderShader* shader)
	: m_shader(shader)
{
}

void ObserverMVP::SetModelview(const sm::mat4* mat)
{
	m_shader->SetUniform(m_modelview, UNIFORM_FLOAT44, mat->x);
}

void ObserverMVP::SetProjection(const sm::mat4* mat)
{
	m_shader->SetUniform(m_projection, UNIFORM_FLOAT44, mat->x);
}

}