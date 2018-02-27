#ifndef _SHADERLAB_SUBJECT_MVP2_H_
#define _SHADERLAB_SUBJECT_MVP2_H_

#include "shaderlab/SubjectMVP.h"

namespace sl
{

class SubjectMVP2 : public SubjectMVP
{
public:
	void NotifyModelview(float x, float y, float sx, float sy);
	void NotifyProjection(int width, int height);

};	// SubjectMVP2
	
}

#endif // _SHADERLAB_SUBJECT_MVP2_H_