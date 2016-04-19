#ifndef _SHADERLAB_RENDER_SHADER_H_
#define _SHADERLAB_RENDER_SHADER_H_

#include <render/render.h>

#include <string.h>
#include <assert.h>

namespace sl
{

class Buffer;
class RenderBuffer;
class RenderLayout;

class RenderShader
{
public:
	RenderShader(render* ej_render);
	~RenderShader();

	void Load(const char* vs, const char* fs);
	void Unload();

	void SetVertexBuffer(RenderBuffer* vb);
	void SetIndexBuffer(RenderBuffer* ib);
	void SetLayout(RenderLayout* lo);

	const RenderBuffer* GetVertexBuffer() const { return m_vb; }
	const RenderBuffer* GetIndexBuffer() const { return m_ib; }

	void Bind();

	void Commit();

	void SetDrawMode(enum DRAW_MODE dm);

	void ApplyUniform();
	bool IsUniformChanged() const { return m_uniform_changed; }

	int AddUniform(const char* name, enum UNIFORM_FORMAT t);
	void SetUniform(int index, enum UNIFORM_FORMAT t, const float* v);

	void Draw(void* vb, int vb_n, void* ib = NULL, int ib_n = 0);

private:
	static int GetUniformSize(enum UNIFORM_FORMAT t);

private:
	static const int MAX_UNIFORM = 16;

	class Uniform 
	{
	public:
		Uniform() : m_loc(0), m_type(UNIFORM_INVALID), m_changed(false) {
			memset(m_value, 0, sizeof(m_value));
		}

		void Assign(int loc, enum UNIFORM_FORMAT type) {
			this->m_loc = loc;
			this->m_type = type;
			m_changed = false;
			memset(m_value, 0, sizeof(m_value));
		}

		bool Assign(enum UNIFORM_FORMAT t, const float* v) {
			assert(t == m_type);
			int n = GetUniformSize(t);
			int change = memcmp(m_value, v, n * sizeof(float));
			if (change != 0) {
				memcpy(m_value, v, n * sizeof(float));
				m_changed = true;
			}
			return m_changed != 0;
		}

		bool Apply(render* ej_render) {
			if (m_changed && m_loc >= 0) {
				render_shader_setuniform(ej_render, m_loc, m_type, m_value);
				return true;
			} else {
				return false;
			}
		}

	private:
		int m_loc;
		enum UNIFORM_FORMAT m_type;

		bool m_changed;
		float m_value[16];

	}; // Uniform

private:
	render* m_ej_render;

	RID m_prog;

	int m_texture_number;

	int m_uniform_number;
	Uniform m_uniform[MAX_UNIFORM];
	bool m_uniform_changed;

	RenderBuffer *m_vb, *m_ib;
	RenderLayout* m_layout;

	enum DRAW_MODE m_draw_mode;

}; // RenderShader

}

#endif // _SHADERLAB_RENDER_SHADER_H_