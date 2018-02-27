#pragma once

#include <guard/check.h>
#include <cu/cu_macro.h>

#include <memory>

namespace sl
{

class RenderContext;

class Blackboard
{
public:
	void SetRenderContext(const std::shared_ptr<RenderContext>& rc) {
		m_rc = rc; 
	}
	RenderContext& GetRenderContext() {
		GD_ASSERT(m_rc, "null rc");
		return *m_rc;
	}

private:
	std::shared_ptr<RenderContext> m_rc = nullptr;

	CU_SINGLETON_DECLARATION(Blackboard);

}; // Blackboard

}