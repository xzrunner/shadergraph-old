#ifndef _SHADERLAB_BLEND_SHADER_H_
#define _SHADERLAB_BLEND_SHADER_H_

#include "shaderlab/Shader.h"
#include "shaderlab/BlendMode.h"
#include "shaderlab/ShaderProgram.h"

#include <stdint.h>

namespace sl
{

class ShaderProgram;

class BlendShader : public Shader
{
public:
	BlendShader(RenderContext& rc);
	virtual ~BlendShader();

	virtual void Bind() const override;
	virtual void UnBind() const override;
	virtual bool Commit() const override;

	void SetColor(uint32_t color, uint32_t additive);

	void SetMode(int mode);

	void Draw(const float* positions, const float* texcoords_blend,
		const float* texcoords_base, int tex_blend, int tex_base) const;

private:
	void InitVAList();
	void InitProg();

private:
	enum VA_TYPE {
		POSITION = 0,
		TEXCOORD,
		TEXCOORD_BASE,
		COLOR,
		ADDITIVE,
		VA_MAX_COUNT
	};

	struct Vertex
	{
		float vx, vy;
		float tx_blend, ty_blend;
		float tx_base, ty_base;
		uint32_t color, additive;
	};

	class Program : public ShaderProgram
	{
	public:
		Program(RenderContext& rc, const CU_VEC<ur::VertexAttrib>& va_list,
			const std::shared_ptr<RenderBuffer>& ib);

		void SetMode(int mode);

	private:
		void Init(const CU_VEC<ur::VertexAttrib>& va_list, const std::shared_ptr<RenderBuffer>& ib);

	private:
		int m_mode;

	}; // Program

private:
	ur::VertexAttrib m_va_list[VA_MAX_COUNT];

	Program* m_prog;

	uint32_t m_color, m_additive;

	SL_BLEND_MODE m_curr_mode;

	mutable int m_tex_blend, m_tex_base;

	Vertex* m_vertex_buf;
	mutable int m_quad_sz;

}; // BlendShader

}

#endif // _SHADERLAB_BLEND_SHADER_H_