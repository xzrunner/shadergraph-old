#include "SubjectMVP3.h"
#include "ObserverMVP.h"

namespace sl
{

SubjectMVP3* SubjectMVP3::m_instance = NULL;

SubjectMVP3* SubjectMVP3::Instance()
{
	if (!m_instance) {
		m_instance = new SubjectMVP3;
	}
	return m_instance;
}

SubjectMVP3::SubjectMVP3()
{
	sm_mat4_identity(&m_modelview);
	sm_mat4_identity(&m_projection);
}

void SubjectMVP3::NotifyModelview(const sm_mat4* mat)
{
	if (!mat) {
		return;
	}
	memcpy(&m_modelview, mat, sizeof(*mat));
	std::set<ObserverMVP*>::iterator itr = m_observers.begin();
	for ( ; itr != m_observers.end(); ++itr) {
		(*itr)->SetModelview(&m_modelview);
	}
}

void SubjectMVP3::NotifyProjection(const sm_mat4* mat)
{
	if (!mat) {
		return;
	}
	memcpy(&m_projection, mat, sizeof(*mat));
	std::set<ObserverMVP*>::iterator itr = m_observers.begin();
	for ( ; itr != m_observers.end(); ++itr) {
		(*itr)->SetProjection(&m_projection);
	}
}

}