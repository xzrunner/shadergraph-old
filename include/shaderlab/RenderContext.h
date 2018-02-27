#pragma once

#include "shaderlab/ShaderMgr.h"
#include "shaderlab/SubjectMVP2.h"
#include "shaderlab/SubjectMVP3.h"

#include <guard/check.h>

#include <memory>

namespace ur { class RenderContext; }

namespace sl
{

class RenderContext
{
public:
	RenderContext(const std::shared_ptr<ur::RenderContext>& rc)
		: m_rc(rc) {}
	
	ur::RenderContext& GetContext() { 
		GD_ASSERT(m_rc, "null rc");
		return *m_rc; 
	}

	ShaderMgr& GetShaderMgr() { return m_shader_mgr; }

	SubjectMVP2& GetSubMVP2() { return m_mvp2; }
	SubjectMVP3& GetSubMVP3() { return m_mvp3; }

private:
	std::shared_ptr<ur::RenderContext> m_rc = nullptr;

	ShaderMgr m_shader_mgr;

	SubjectMVP2 m_mvp2;
	SubjectMVP3 m_mvp3;

}; // RenderContext

}