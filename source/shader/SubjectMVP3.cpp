#include "shaderlab/SubjectMVP3.h"
#include "shaderlab/ObserverMVP.h"

#include <string.h>

namespace sl
{

void SubjectMVP3::NotifyModelview(const sm::mat4& mat)
{
	m_modelview = mat;
	CU_SET<ObserverMVP*>::iterator itr = m_observers.begin();
	for ( ; itr != m_observers.end(); ++itr) {
		(*itr)->SetModelview(&m_modelview);
	}
}

void SubjectMVP3::NotifyProjection(const sm::mat4& mat)
{
	m_projection = mat;
	CU_SET<ObserverMVP*>::iterator itr = m_observers.begin();
	for ( ; itr != m_observers.end(); ++itr) {
		(*itr)->SetProjection(&m_projection);
	}
}

}