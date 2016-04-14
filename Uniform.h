#ifndef _SHADERLAB_UNIFORM_H_
#define _SHADERLAB_UNIFORM_H_

#include <sm.h>

namespace sl
{

struct UniformMat4
{
	int id;
	union sm_mat4 mat;

	UniformMat4() : id(0) { sm_mat4_identity(&mat); }
};

}

#endif // _SHADERLAB_UNIFORM_H_