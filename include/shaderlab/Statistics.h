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
    void SetRender(bool b) {m_disable_render = !b;}

	int GetDrawCall() const { return m_drawcall; }
	int GetVertices() const { return m_vertices; }
    bool GetRender() const {return !m_disable_render; }

	void Print(std::string& str) const;

	void Reset();

private:
	int m_drawcall, m_max_drawcall;
	int m_vertices, m_max_vertices;
    bool m_disable_render;

	SINGLETON_DECLARATION(Statistics);

}; // Statistics

}

#endif // _SHADERLAB_STATISTICS_H_