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

}