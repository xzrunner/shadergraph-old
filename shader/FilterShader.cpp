#include "FilterShader.h"
#include "Utility.h"
#include "SubjectMVP2.h"
#include "EdgeDetectProg.h"
#include "ReliefProg.h"
#include "OutlineProg.h"
#include "GrayProg.h"
#include "BlurProg.h"
#include "GaussianBlurProg.h"
#include "HeatHazeProg.h"
#include "ShockWaveProg.h"
#include "SwirlProg.h"
#include "render/RenderContext.h"
#include "render/RenderBuffer.h"
#include "render/RenderShader.h"

#include <render/render.h>

namespace sl
{

static const int MAX_COMMBINE = 1024;

FilterShader::FilterShader(RenderContext* rc)
	: Shader(rc)
	, m_time(0)
	, m_curr_mode(FM_NULL)
	, m_texid(0)
	, m_quad_sz(0)
{
	m_vertex_buf = new Vertex[MAX_COMMBINE * 4];

	m_rc->SetClearFlag(MASKC);

	InitVAList();
	InitProgs();
}

FilterShader::~FilterShader()
{
	for (int i = 0; i < PROG_COUNT; ++i) {
		FilterProgram* prog = m_programs[i];
		if (prog) {
			delete prog;
		}
	}
}

void FilterShader::Bind() const
{
	if (m_curr_mode != FM_NULL) {
		int idx = m_mode2index[m_curr_mode];
		m_programs[idx]->GetShader()->Bind();
	}
}

void FilterShader::UnBind() const
{
}

void FilterShader::Commit() const
{
	if (m_quad_sz == 0 || m_curr_mode == FM_NULL) {
		return;
	}

	m_rc->SetTexture(m_texid, 0);

	int idx = m_mode2index[m_curr_mode];
	FilterProgram* prog = m_programs[idx];
	prog->BindTexture();

	RenderShader* shader = prog->GetShader();
	m_rc->BindShader(shader);
	shader->Draw(m_vertex_buf, m_quad_sz * 4, NULL, m_quad_sz * 6);
	m_quad_sz = 0;

	shader->Commit();
}

void FilterShader::BindTexture()
{
	for (int i = 0; i < PROG_COUNT; ++i) {
		FilterProgram* prog = m_programs[i];
		if (prog) {
			prog->BindTexture();
		}
	}
}

void FilterShader::UpdateTime(float dt)
{
	m_time += dt;
	for (int i = 0; i < PROG_COUNT; ++i) {
		FilterProgram* prog = m_programs[i];
		if (prog) {
			prog->UpdateTime(m_time);
		}
	}
}

void FilterShader::SetMode(FILTER_MODE mode)
{
	if (mode != m_curr_mode) {
		Commit();
		m_curr_mode = mode;
		int idx = m_mode2index[m_curr_mode];
		m_programs[idx]->GetShader()->Bind();
	}
}

void FilterShader::Draw(const float* positions, const float* texcoords, int texid) const
{
	if (m_quad_sz >= MAX_COMMBINE || (m_texid != texid && m_texid != 0)) {
		Commit();
	}
	m_texid = texid;

	for (int i = 0; i < 4; ++i) 
	{
		Vertex* v = &m_vertex_buf[m_quad_sz * 4 + i];
		v->vx = positions[i * 2];
		v->vy = positions[i * 2 + 1];
		v->tx = texcoords[i * 2];
		v->ty = texcoords[i * 2 + 1];
	}
	++m_quad_sz;
}

void FilterShader::InitVAList()
{
	m_va_list[POSITION].Assign("position", 2, sizeof(float));
	m_va_list[TEXCOORD].Assign("texcoord", 2, sizeof(float));
}

void FilterShader::InitProgs()
{
	memset(m_programs, 0, sizeof(m_programs));

	std::vector<VertexAttrib> va_list;
	va_list.push_back(m_va_list[POSITION]);
	va_list.push_back(m_va_list[TEXCOORD]);

	int max_vertex = MAX_COMMBINE * 4;
	RenderBuffer* idx_buf = Utility::CreateQuadIndexBuffer(m_rc, MAX_COMMBINE);

	// edge detect
	EdgeDetectProg* edge_detect		= new EdgeDetectProg(m_rc, max_vertex, va_list, idx_buf);
	edge_detect->SetBlend(0.5f);
	m_programs[PI_EDGE_DETECTION]	= edge_detect;
	// relief
	m_programs[PI_RELIEF]			= new ReliefProg(m_rc, max_vertex, va_list, idx_buf);
	// outline
	m_programs[PI_OUTLINE]			= new OutlineProg(m_rc, max_vertex, va_list, idx_buf);

	// gray
	m_programs[PI_GRAY]				= new GrayProg(m_rc, max_vertex, va_list, idx_buf);
	// blur
	BlurProg* blur					= new BlurProg(m_rc, max_vertex, va_list, idx_buf);
	blur->SetRadius(1);
	m_programs[PI_BLUR]				= blur;
	// gaussian blur
	m_programs[PI_GAUSSIAN_BLUR]	= new GaussianBlurProg(m_rc, max_vertex, va_list, idx_buf);

	// heat haze
	HeatHazeProg* heat_haze			= new HeatHazeProg(m_rc, max_vertex, va_list, idx_buf);
	heat_haze->SetFactor(0.1f, 0.5f);
	m_programs[PI_HEAT_HAZE]		= heat_haze;
	// shock wave
	ShockWaveProg* shock_wave		= new ShockWaveProg(m_rc, max_vertex, va_list, idx_buf);
	{
		float center[2] = { 0.5f, 0.5f };
		shock_wave->SetCenter(center);
		float params[3] = { 10, 0.8f, 0.1f };
		shock_wave->SetFactor(params);
	}
	m_programs[PI_SHOCK_WAVE]		= shock_wave;
	// swirl
	SwirlProg* swirl				= new SwirlProg(m_rc, max_vertex, va_list, idx_buf);
	{
		float center[2] = { 400, 300 };
		swirl->SetCenter(center);
		swirl->SetAngle(0.8f);
		swirl->SetRadius(200);
	}	
	m_programs[PI_SWIRL]			= swirl;

	memset(m_mode2index, 0xff, sizeof(m_mode2index));
	m_mode2index[FM_EDGE_DETECTION]	= PI_EDGE_DETECTION;
	m_mode2index[FM_RELIEF]			= PI_RELIEF;
	m_mode2index[FM_OUTLINE]		= PI_OUTLINE;
	m_mode2index[FM_GRAY]			= PI_GRAY;
	m_mode2index[FM_BLUR]			= PI_BLUR;
	m_mode2index[FM_GAUSSIAN_BLUR]	= PI_GAUSSIAN_BLUR;
	m_mode2index[FM_HEAT_HAZE]		= PI_HEAT_HAZE;
	m_mode2index[FM_SHOCK_WAVE]		= PI_SHOCK_WAVE;
	m_mode2index[FM_SWIRL]			= PI_SWIRL;

	for (int i = 0; i < PROG_COUNT; ++i) {
		ShaderProgram* prog = m_programs[i];
		if (prog) {
			SubjectMVP2::Instance()->Register(prog->GetMVP());
			prog->GetShader()->SetDrawMode(DRAW_TRIANGLES);
		}
	}

	idx_buf->Release();
}

}