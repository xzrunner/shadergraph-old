#ifndef _SHADERLAB_STAT_DRAWCALL_H_
#define _SHADERLAB_STAT_DRAWCALL_H_

#include <cu/cu_stl.h>
#include <cu/cu_macro.h>

namespace sl
{

class StatDrawCall
{
public:
	void Print(CU_STR& str) const;
	void Reset();

 	void AddDrawMode() { ++m_draw_mode; }
	void AddUniform() { ++m_uniform; }
	void AddIndexBuf() { ++m_index_buf; }
	void AddVertexBuf() { ++m_vertex_buf; }
	void AddFull() { ++m_full; }
	void AddTex() { ++m_tex; }
	void AddShader() { ++m_shader; }

private:
	int m_draw_mode;
	int m_uniform;
	int m_index_buf;
	int m_vertex_buf;
	int m_full;
	int m_tex;
	int m_shader;

	CU_SINGLETON_DECLARATION(StatDrawCall);

}; // StatDrawCall

}

#endif // _SHADERLAB_STAT_DRAWCALL_H_