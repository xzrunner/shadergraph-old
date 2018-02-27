#pragma once

#include <cu/cu_macro.h>

#include <memory>

namespace sl
{

class ShaderMgr;

class Blackboard
{
public:
	void SetShaderMgr(const std::shared_ptr<ShaderMgr>& shader_mgr) { 
		m_shader_mgr = shader_mgr; 
	}
	ShaderMgr* GetShaderMgr() {
		return m_shader_mgr.get();
	}

private:
	std::shared_ptr<ShaderMgr> m_shader_mgr = nullptr;

	CU_SINGLETON_DECLARATION(Blackboard);

}; // Blackboard

}