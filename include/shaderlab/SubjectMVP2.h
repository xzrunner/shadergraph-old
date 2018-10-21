#ifndef _SHADERLAB_SUBJECT_MVP2_H_
#define _SHADERLAB_SUBJECT_MVP2_H_

#include "shaderlab/SubjectMVP.h"

namespace sl
{

class SubjectMVP2 : public SubjectMVP
{
public:
	SubjectMVP2();

	void NotifyModelview(float x, float y, float sx, float sy);
	void NotifyProjection(int width, int height);

private:
	// mv
	float m_x, m_y, m_sx, m_sy;

	// proj
	int m_width, m_height;

};	// SubjectMVP2

}

#endif // _SHADERLAB_SUBJECT_MVP2_H_