#ifndef _SHADERLAB_SUBJECT_MVP2_H_
#define _SHADERLAB_SUBJECT_MVP2_H_

#include <sm.h>

#include <set>

namespace sl
{

class ObserverMVP;

class SubjectMVP2
{
public:
	void Register(ObserverMVP* observer) { m_observers.insert(observer); }
	void UnRegister(ObserverMVP* observer) { m_observers.erase(observer); }

	void NotifyModelview(float x, float y, float sx, float sy);
	void NotifyProjection(int width, int height);

	static SubjectMVP2* Instance();

private:
	SubjectMVP2();

private:
	std::set<ObserverMVP*> m_observers;

	union sm_mat4 m_modelview, m_projection;

private:
	static SubjectMVP2* m_instance;

};	// SubjectMVP2
	
}

#endif // _SHADERLAB_SUBJECT_MVP2_H_