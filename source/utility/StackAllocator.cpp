#include "StackAllocator.h"

namespace sl
{

StackAllocator* StackAllocator::m_instance = nullptr;

StackAllocator* StackAllocator::Instance()
{
	if (!m_instance) {
		m_instance = new StackAllocator;
	}
	return m_instance;
}

}