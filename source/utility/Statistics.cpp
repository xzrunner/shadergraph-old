#include "shaderlab/Statistics.h"

namespace sl
{

SINGLETON_DEFINITION(Statistics);

Statistics::Statistics() 
	: m_drawcall(0)
	, m_vertices(0)
{}

void Statistics::Reset()
{
	m_drawcall = 0;
	m_vertices = 0;
}

void Statistics::Print(std::string& str) const
{
	static char buf[512];
	sprintf(buf, "DC: %d, vertices: %d", m_drawcall, m_vertices);
	str += buf;
}

}