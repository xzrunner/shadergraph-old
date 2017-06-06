#ifndef _SHADERLAB_STATISTICS_H_
#define _SHADERLAB_STATISTICS_H_

#include <CU_Singleton.h>

#include <string>

namespace sl
{

class Statistics
{
public:
	void AddDrawCall() { ++m_drawcall; }
	void AddVertices(int count) { m_vertices += count; }

	int GetDrawCall() const { return m_drawcall; }
	int GetVertices() const { return m_vertices; }

	void Print(std::string& str) const;

	void Reset();

private:
	int m_drawcall;
	int m_vertices;

	SINGLETON_DECLARATION(Statistics);

}; // Statistics

}

#endif // _SHADERLAB_STATISTICS_H_