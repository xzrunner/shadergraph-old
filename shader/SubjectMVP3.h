#ifndef _SHADERLAB_SUBJECT_MVP3_H_
#define _SHADERLAB_SUBJECT_MVP3_H_

#include <sm.h>

#include <set>

namespace sl
{

class ObserverMVP;

class SubjectMVP3
{
public:
	void Register(ObserverMVP* observer) { m_observers.insert(observer); }
	void UnRegister(ObserverMVP* observer) { m_observers.erase(observer); }

	void NotifyModelview(const sm_mat4* mat);
	void NotifyProjection(const sm_mat4* mat);

	static SubjectMVP3* Instance();

private:
	SubjectMVP3();

private:
	std::set<ObserverMVP*> m_observers;

	sm_mat4 m_modelview, m_projection;

private:
	static SubjectMVP3* m_instance;

};	// SubjectMVP3
	
}

#endif // _SHADERLAB_SUBJECT_MVP3_H_