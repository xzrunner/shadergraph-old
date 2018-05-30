#include "shaderlab/RenderShader.h"
#include "shaderlab/RenderBuffer.h"
#include "shaderlab/RenderLayout.h"
#include "shaderlab/ShaderMgr.h"
#include "shaderlab/Shader.h"
#include "shaderlab/Statistics.h"
#include "shaderlab/config.h"
#ifndef SL_DISABLE_STATISTICS
#include "shaderlab/StatDrawCall.h"
#endif // SL_DISABLE_STATISTICS
#include "shaderlab/RenderContext.h"

// extern "C" {
// 	#include "shaderlab/adapter/ej_statistics.h"
// }

#include <unirender/RenderContext.h>
#include <unirender/Blackboard.h>

//#define SHADER_LOG

#ifdef SHADER_LOG
#include <iostream>
#endif // SHADER_LOG

namespace sl
{

RenderShader::RenderShader(RenderContext& rc)
	: m_rc(rc)
{
	m_prog = 0;

	m_texture_number = 0;

	m_uniform_number = 0;
	m_uniform_changed = false;

	m_vb = nullptr;
	m_ib = nullptr;
	m_layout = nullptr;

	m_draw_mode = ur::DRAW_POINTS;
}

void RenderShader::Load(const char* vs, const char* fs)
{
#ifdef SHADER_LOG
	std::cout << "================================================== \n";
	std::cout << vs << '\n';
	std::cout << fs << '\n';
	std::cout << "================================================== \n";
#endif // SHADER_LOG

	auto& ur_rc = ur::Blackboard::Instance()->GetRenderContext();
	std::vector<std::string> textures;
	m_prog = ur_rc.CreateShader(vs, fs, textures);
	ur_rc.BindShader(m_prog);
//	render_shader_bind(m_ej_render, 0);	// ??
	//	S->curr_shader = -1;
}

void RenderShader::Unload()
{
	ur::Blackboard::Instance()->GetRenderContext().ReleaseShader(m_prog);
}

void RenderShader::Bind()
{
	ur::Blackboard::Instance()->GetRenderContext().BindShader(m_prog);
	m_vb->Bind();
	if (m_ib) {
		m_ib->Bind();
	}
//	m_layout->Bind();
}

bool RenderShader::Commit()
{
	if (!m_vb || m_vb->IsEmpty()) {
		return false;
	}

	#ifdef DEBUG_RENDER
		// 关闭渲染
		bool b = Statistics::Instance()->GetRender();
		if(!b){
			return false;
		}
	#endif

	ApplyUniform();
	Statistics* stat = Statistics::Instance();

	m_vb->Update();
	if (m_ib) {
		m_ib->Update();
		ur::Blackboard::Instance()->GetRenderContext().DrawElements((ur::DRAW_MODE)m_draw_mode, 0, m_ib->Size());
		stat->AddVertices(m_ib->Size());
		m_ib->Clear();
	} else {
		ur::Blackboard::Instance()->GetRenderContext().DrawArrays((ur::DRAW_MODE)m_draw_mode, 0, m_vb->Size());
		stat->AddVertices(m_vb->Size());
	}
	m_vb->Clear();

	stat->AddDrawCall();

	return true;
}

void RenderShader::SetDrawMode(DRAW_MODE_TYPE dm)
{
	if (m_draw_mode != dm) {
		bool changed = Commit();
#ifndef SL_DISABLE_STATISTICS
		if (changed) {
			StatDrawCall::Instance()->AddDrawMode();
		}
#endif // SL_DISABLE_STATISTICS
		m_draw_mode = dm;
	}
}

int RenderShader::AddUniform(const char* name, UNIFORM_FORMAT_TYPE t)
{
	// todo ur::RenderContext::Bind()

	if (m_uniform_number >= MAX_UNIFORM) {
		return -1;
	}
	int loc = ur::Blackboard::Instance()->GetRenderContext().GetShaderUniform(name);
	int index = m_uniform_number++;
	m_uniform[index].Assign(loc, t);
	return loc < 0 ? -1 : index;
}

void RenderShader::SetUniform(int index, UNIFORM_FORMAT_TYPE t, const float* v)
{
	// todo ur::RenderContext::Bind()

	if (index < 0 || index >= m_uniform_number) {
		return;
	}

	bool same = m_uniform[index].Same(t, v);
	if (same) {
		return;
	}

	m_uniform_changed = true;
	if (Shader* shader = m_rc.GetShaderMgr().GetShader()) {
		bool changed = shader->Commit();
#ifndef SL_DISABLE_STATISTICS
		if (changed) {
			StatDrawCall::Instance()->AddUniform();
		}
#endif // SL_DISABLE_STATISTICS
	}
	m_uniform[index].Assign(t, v);
}

void RenderShader::Draw(const void* vb, int vb_n, const void* ib, int ib_n)
{
	if (m_ib && ib_n > 0 && m_ib->Add(ib, ib_n)) {
		bool changed = Commit();
#ifndef SL_DISABLE_STATISTICS
		if (changed) {
			StatDrawCall::Instance()->AddIndexBuf();
		}
#endif // SL_DISABLE_STATISTICS
	}
	if (m_vb && vb_n > 0 && m_vb->Add(vb, vb_n)) {
		bool changed = Commit();
#ifndef SL_DISABLE_STATISTICS
		if (changed) {
			StatDrawCall::Instance()->AddVertexBuf();
		}
#endif // SL_DISABLE_STATISTICS
	}
}

void RenderShader::ApplyUniform()
{
	auto& ur_rc = ur::Blackboard::Instance()->GetRenderContext();
	for (int i = 0; i < m_uniform_number; ++i)
	{
		bool changed = m_uniform[i].Apply(ur_rc);
		if (changed) {
			m_uniform_changed = changed;
		}
	}
}

int RenderShader::GetUniformSize(UNIFORM_FORMAT_TYPE t)
{
	int n = 0;
	switch(t) {
	case ur::UNIFORM_INVALID:
		n = 0;
		break;
	case ur::UNIFORM_FLOAT1:
		n = 1;
		break;
	case ur::UNIFORM_FLOAT2:
		n = 2;
		break;
	case ur::UNIFORM_FLOAT3:
		n = 3;
		break;
	case ur::UNIFORM_FLOAT4:
		n = 4;
		break;
	case ur::UNIFORM_FLOAT33:
		n = 9;
		break;
	case ur::UNIFORM_FLOAT44:
		n = 16;
		break;
	case ur::UNIFORM_INT1:
		n = 1;
		break;
	}
	return n;
}

/************************************************************************/
/* RenderShader::Uniform                                                */
/************************************************************************/

RenderShader::Uniform::Uniform()
	: m_loc(0), m_type(0), m_changed(false)
{
	memset(m_value, 0, sizeof(m_value));
}

bool RenderShader::Uniform::Same(UNIFORM_FORMAT_TYPE t, const float* v)
{
	assert(t == m_type);
	int n = GetUniformSize(t);
	int change = memcmp(m_value, v, n * sizeof(float));
	if (change != 0) {
		return false;
	} else {
		return true;
	}
}

void RenderShader::Uniform::Assign(int loc, UNIFORM_FORMAT_TYPE type)
{
	this->m_loc = loc;
	this->m_type = type;
	m_changed = false;
	memset(m_value, 0, sizeof(m_value));
}

void RenderShader::Uniform::Assign(UNIFORM_FORMAT_TYPE t, const float* v)
{
	assert(t == m_type);
	int n = GetUniformSize(t);
	memcpy(m_value, v, n * sizeof(float));
	m_changed = true;
}

bool RenderShader::Uniform::Apply(ur::RenderContext& rc)
{
	if (m_changed && m_loc >= 0) {
		rc.SetShaderUniform(m_loc, (ur::UNIFORM_FORMAT)m_type, m_value);
		return true;
	} else {
		return false;
	}
}

}