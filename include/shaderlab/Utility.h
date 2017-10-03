#ifndef _SHADERLAB_UTILITY_H_
#define _SHADERLAB_UTILITY_H_

#include <memory>

#include <stdint.h>

namespace ur { class RenderContext; }

namespace sl
{

class RenderBuffer;

class Utility
{
public:
	static std::shared_ptr<RenderBuffer> CreateIndexBuffer(ur::RenderContext* rc, int count);
	static std::shared_ptr<RenderBuffer> CreateQuadIndexBuffer(ur::RenderContext* rc, int quad_count);

private:
	static void FillingQuadIndexBuffer(uint16_t* buf, int quad_count);

}; // Utility

}

#endif // _SHADERLAB_UTILITY_H_