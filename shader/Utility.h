#ifndef _SHADERLAB_UTILITY_H_
#define _SHADERLAB_UTILITY_H_

#include <stdint.h>

namespace sl
{

class RenderContext;
class RenderBuffer;

class Utility
{
public:
	static RenderBuffer* CreateQuadIndexBuffer(RenderContext* rc, int quad_count);

private:
	static void FillingQuadIndexBuffer(uint16_t* buf, int quad_count);

}; // Utility

}

#endif // _SHADERLAB_UTILITY_H_