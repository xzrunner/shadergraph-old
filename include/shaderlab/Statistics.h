#ifndef _SHADERLAB_STATISTICS_H_
#define _SHADERLAB_STATISTICS_H_

#include <cu/cu_stl.h>
#include <cu/cu_macro.h>

namespace sl
{

class Statistics
{
public:
	void AddDrawCall() { ++m_drawcall; }
	void AddVertices(int count) { m_vertices += count; }
    void SetRender(bool b) {m_disable_render = !b;}

	int GetDrawCall() const { return m_drawcall; }
	int GetLastDrawCall() const { return m_last_drawcall; }
	int GetVertices() const { return m_vertices; }
    bool GetRender() const {return !m_disable_render; }

	void Print(CU_STR& str) const;

	void Reset();

private:
	int m_drawcall, m_max_drawcall, m_last_drawcall;
	int m_vertices, m_max_vertices;
    bool m_disable_render;

	CU_SINGLETON_DECLARATION(Statistics);

}; // Statistics

}

#endif // _SHADERLAB_STATISTICS_H_