#include "RenderShader.h"
#include "RenderBuffer.h"
#include "RenderLayout.h"

#include <iostream>

namespace sl
{

RenderShader::RenderShader(render* ej_render)
	: m_ej_render(ej_render)
{
	m_prog = 0;

	m_texture_number = 0;

	m_uniform_number = 0;
	m_uniform_changed = false;

	m_vb = m_ib = NULL;
	m_layout = NULL;

	m_draw_mode = DRAW_POINTS;
}

RenderShader::~RenderShader()
{
	if (m_vb) m_vb->Release();
	if (m_ib) m_ib->Release();
	if (m_layout) m_layout->Release();
}

void RenderShader::Load(const char* vs, const char* fs)
{
	std::cout << vs << std::endl;
	std::cout << fs << std::endl;

	struct shader_init_args args;
	args.vs = vs;
	args.fs = fs;
	args.texture = 0;
	m_prog = render_shader_create(m_ej_render, &args);
	render_shader_bind(m_ej_render, m_prog);
//	render_shader_bind(m_ej_render, 0);	// ??
	//	S->curr_shader = -1;
}

void RenderShader::Unload()
{
	render_release(m_ej_render, SHADER, m_prog);
}

void RenderShader::SetVertexBuffer(RenderBuffer* vb) 
{ 
	OBJ_ASSIGN(m_vb, vb) 
}

void RenderShader::SetIndexBuffer(RenderBuffer* ib) 
{ 
	OBJ_ASSIGN(m_ib, ib) 
}

void RenderShader::SetLayout(RenderLayout* lo) 
{ 
	OBJ_ASSIGN(m_layout, lo) 
}

void RenderShader::Bind()
{
	render_shader_bind(m_ej_render, m_prog);
	m_vb->Bind();
	if (m_ib) {
		m_ib->Bind();
	}
//	m_layout->Bind();
}

void RenderShader::Commit()
{
	if (!m_vb || m_vb->IsEmpty()) {
		return;
	}
//#ifdef _DEBUG
//	std::cout << "Commit %d" << m_vb->Size() << "\n";
//#endif // _DEBUG

	m_vb->Update();
	if (m_ib) {
		m_ib->Update();
		render_draw_elements(m_ej_render, m_draw_mode, 0, m_ib->Size());
		m_ib->Clear();
	} else {
		render_draw_arrays(m_ej_render, m_draw_mode, 0, m_vb->Size());
	}	
	m_vb->Clear();
}

void RenderShader::ApplyUniform()
{
	for (int i = 0; i < m_uniform_number; ++i) {
		bool changed = m_uniform[i].Apply(m_ej_render);
		if (changed) {
			m_uniform_changed = changed;
		}
	}
}

int RenderShader::AddUniform(const char* name, enum UNIFORM_FORMAT t)
{
	// todo RenderContext::Bind()

	if (m_uniform_number >= MAX_UNIFORM) {
		return -1;
	}
	int loc = render_shader_locuniform(m_ej_render, name);
	int index = m_uniform_number++;
	m_uniform[index].Assign(loc, t);
	return loc < 0 ? -1 : index;
}

void RenderShader::SetUniform(int index, enum UNIFORM_FORMAT t, const float* v)
{
	// todo RenderContext::Bind()

	if (index >= 0 && index < m_uniform_number) {
		bool changed = m_uniform[index].Assign(t, v);
		if (changed) {
			Commit();
			m_uniform_changed = true;
		}
	}
}

void RenderShader::Draw(void* data, int vb_n, int ib_n)
{
	if (m_ib && ib_n > 0 && m_ib->Add(NULL, ib_n)) {
		Commit();
	}
	if (m_vb && vb_n > 0 && m_vb->Add(data, vb_n)) {
		Commit();
	}
}

int RenderShader::GetUniformSize(enum UNIFORM_FORMAT t)
{
	int n = 0;
	switch(t) {
	case UNIFORM_INVALID:
		n = 0;
		break;
	case UNIFORM_FLOAT1:
		n = 1;
		break;
	case UNIFORM_FLOAT2:
		n = 2;
		break;
	case UNIFORM_FLOAT3:
		n = 3;
		break;
	case UNIFORM_FLOAT4:
		n = 4;
		break;
	case UNIFORM_FLOAT33:
		n = 9;
		break;
	case UNIFORM_FLOAT44:
		n = 16;
		break;
	case UNIFORM_INT1:
		n = 1;
		break;
	}
	return n;
}

}