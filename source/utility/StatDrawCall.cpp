#include "shaderlab/StatDrawCall.h"

#include <string.h>

namespace sl
{

SINGLETON_DEFINITION(StatDrawCall);

StatDrawCall::StatDrawCall()
{
	Reset();
}

void StatDrawCall::Print(std::string& str) const
{
	static char buf[512];
	sprintf(buf, "DC: mode %d, uni %d, ib %d, vb %d, full %d, tex %d, sd %d", 
		m_draw_mode, m_uniform, m_index_buf, m_vertex_buf, m_full, m_tex, m_shader);
	str += buf;
}

void StatDrawCall::Reset()
{
	memset(this, 0, sizeof(*this));
}

}