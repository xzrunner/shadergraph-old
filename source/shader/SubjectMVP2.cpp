#include "shaderlab/SubjectMVP2.h"
#include "shaderlab/ObserverMVP.h"

#include <stddef.h>

namespace sl
{

SubjectMVP2::SubjectMVP2()
	: m_x(0)
	, m_y(0)
	, m_sx(0)
	, m_sy(0)
	, m_width(0)
	, m_height(0)
{
}

void SubjectMVP2::NotifyModelview(float x, float y, float sx, float sy)
{
	if (m_x == x && m_y == y && m_sx == m_sx && m_sy == sy) {
		return;
	}

	m_x = x;
	m_y = y;
	m_sx = sx;
	m_sy = sy;

	m_modelview = sm::mat4::Scaled(sx, sy, 1);
	m_modelview.Translate(x * sx, y * sy, 0);
	CU_SET<ObserverMVP*>::iterator itr = m_observers.begin();
	for ( ; itr != m_observers.end(); ++itr) {
		(*itr)->SetModelview(&m_modelview);
	}
}

void SubjectMVP2::NotifyProjection(int width, int height)
{
	if (m_width == width && m_height == height) {
		return;
	}

	m_width  = width;
	m_height = height;

	float hw = width * 0.5f;
	float hh = height * 0.5f;
	m_projection = sm::mat4::Orthographic(-hw, hw, -hh, hh, 1, -1);
	CU_SET<ObserverMVP*>::iterator itr = m_observers.begin();
	for ( ; itr != m_observers.end(); ++itr) {
		(*itr)->SetProjection(&m_projection);
	}
}

}