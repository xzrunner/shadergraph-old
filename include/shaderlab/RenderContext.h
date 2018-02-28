#pragma once

#include "shaderlab/ShaderMgr.h"
#include "shaderlab/SubjectMVP2.h"
#include "shaderlab/SubjectMVP3.h"

#include <guard/check.h>

#include <memory>

namespace sl
{

class RenderContext
{
public:
	ShaderMgr& GetShaderMgr() { return m_shader_mgr; }

	SubjectMVP2& GetSubMVP2() { return m_mvp2; }
	SubjectMVP3& GetSubMVP3() { return m_mvp3; }

private:
	ShaderMgr m_shader_mgr;

	SubjectMVP2 m_mvp2;
	SubjectMVP3 m_mvp3;

}; // RenderContext

}