#include "shaderlab/Statistics.h"


#include <stdio.h>

namespace sl
{

CU_SINGLETON_DEFINITION(Statistics);

Statistics::Statistics()
	: m_drawcall(0)
	, m_max_drawcall(0)
	, m_last_drawcall(0)
	, m_vertices(0)
	, m_max_vertices(0)
{}

void Statistics::Reset()
{
	if (m_drawcall > m_max_drawcall) {
		m_max_drawcall = m_drawcall;
	}
	if (m_vertices > m_max_vertices) {
		m_max_vertices = m_vertices;
	}

	m_last_drawcall = m_drawcall;

	m_drawcall = 0;
	m_vertices = 0;
}

void Statistics::Print(CU_STR& str) const
{
	static char buf[512];
	sprintf(buf, "DC: %d, vertices: %d, max DC %d, max vert %d",
		m_drawcall, m_vertices, m_max_drawcall, m_max_vertices);
	str += buf;
}

}