#ifndef _SHADERLAB_SUBJECT_MVP_H_
#define _SHADERLAB_SUBJECT_MVP_H_

#include <sm.h>

#include <set>

namespace sl
{

class ObserverMVP;

class SubjectMVP
{
public:
	void Register(ObserverMVP* observer) { m_observers.insert(observer); }
	void UnRegister(ObserverMVP* observer) { m_observers.erase(observer); }

	void Clear() { 
		m_observers.clear();
		sm_mat4_identity(&m_modelview);
		sm_mat4_identity(&m_projection);
	}

protected:
	std::set<ObserverMVP*> m_observers;

	sm_mat4 m_modelview, m_projection;

};	// SubjectMVP

}

#endif // _SHADERLAB_SUBJECT_MVP_H_
