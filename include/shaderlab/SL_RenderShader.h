#ifndef _SHADERLAB_RENDER_SHADER_H_
#define _SHADERLAB_RENDER_SHADER_H_

#include "sl_typedef.h"

#include <memory>

#include <string.h>
#include <assert.h>

namespace ur { class RenderContext; }

namespace sl
{

class Buffer;
class RenderBuffer;
class RenderLayout;

class RenderShader
{
public:
	RenderShader(ur::RenderContext* rc);

	void Load(const char* vs, const char* fs);
	void Unload();

	void SetVertexBuffer(const std::shared_ptr<RenderBuffer>& vb) { m_vb = vb; }
	void SetIndexBuffer(const std::shared_ptr<RenderBuffer>& ib) { m_ib = ib; }
	void SetLayout(const std::shared_ptr<RenderLayout>& lo) { m_layout = lo; }

	const std::shared_ptr<RenderBuffer>& GetVertexBuffer() const { return m_vb; }
	const std::shared_ptr<RenderBuffer>& GetIndexBuffer() const { return m_ib; }

	/**
	 *  @note
	 *    Must only called by ur::RenderContext::BindShader()
	 */
	void Bind();

	bool Commit();

	void SetDrawMode(DRAW_MODE_TYPE dm);

	bool IsUniformChanged() const { return m_uniform_changed; }

	int AddUniform(const char* name, UNIFORM_FORMAT_TYPE t);
	void SetUniform(int index, UNIFORM_FORMAT_TYPE t, const float* v);

	void Draw(const void* vb, int vb_n, const void* ib = nullptr, int ib_n = 0);

private:
	void ApplyUniform();

	static int GetUniformSize(UNIFORM_FORMAT_TYPE t);

private:
	static const int MAX_UNIFORM = 16;

	class Uniform 
	{
	public:
		Uniform();

		bool Same(UNIFORM_FORMAT_TYPE t, const float* v);

		void Assign(int loc, UNIFORM_FORMAT_TYPE type);
		void Assign(UNIFORM_FORMAT_TYPE t, const float* v);

		bool Apply(ur::RenderContext* rc);

	private:
		int m_loc;
		UNIFORM_FORMAT_TYPE m_type;

		bool m_changed;
		float m_value[16];

	}; // Uniform

private:
	ur::RenderContext* m_rc;

	int m_prog;

	int m_texture_number;

	int m_uniform_number;
	Uniform m_uniform[MAX_UNIFORM];
	bool m_uniform_changed;

	std::shared_ptr<RenderBuffer> m_vb, m_ib;
	std::shared_ptr<RenderLayout> m_layout;

	DRAW_MODE_TYPE m_draw_mode;

}; // RenderShader

}

#endif // _SHADERLAB_RENDER_SHADER_H_