#ifndef _SHADERLAB_FILTER_SHADER_H_
#define _SHADERLAB_FILTER_SHADER_H_

#include "Shader.h"
#include "FilterMode.h"
#include "render/VertexAttrib.h"

namespace sl
{

class FilterProgram;

class FilterShader : public Shader
{
public:
	FilterShader(RenderContext* rc);
	virtual ~FilterShader();
	
	virtual void Bind() const;
	virtual void UnBind() const;
	virtual void Commit() const;
	virtual void BindTexture();

	void UpdateTime(float dt);

	void SetMode(FILTER_MODE mode);

	void Draw(const float* positions, const float* texcoords, int texid) const;

private:
	void InitVAList();
	void InitProgs();

private:
	enum PROG_IDX {
		PI_EDGE_DETECTION = 0,
		PI_RELIEF,
		PI_OUTLINE,

		PI_GRAY,
		PI_BLUR,
		PI_GAUSSIAN_BLUR,

		PI_HEAT_HAZE,
		PI_SHOCK_WAVE,
		PI_SWIRL,	

		PROG_COUNT
	};

	enum VA_TYPE {
		POSITION = 0,
		TEXCOORD,
		VA_MAX_COUNT
	};

	struct Vertex
	{
		float vx, vy;
		float tx, ty;
	};

private:
	VertexAttrib m_va_list[VA_MAX_COUNT];

	FilterProgram* m_programs[PROG_COUNT];

	float m_time;

	FILTER_MODE m_curr_mode;
	int m_mode2index[256];

	mutable int m_texid;

	Vertex* m_vertex_buf;
	mutable int m_quad_sz;

}; // FilterShader

}

#endif // _SHADERLAB_FILTER_SHADER_H_