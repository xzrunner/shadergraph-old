#include "StackAllocator.h"

namespace sl
{

StackAllocator* StackAllocator::m_instance = NULL;

StackAllocator* StackAllocator::Instance()
{
	if (!m_instance) {
		m_instance = new StackAllocator;
	}
	return m_instance;
}

void StackAllocator::Alloc(int sz)
{
	if (m_sz < sz) {
		delete[] m_buf;
		m_buf = new uint8_t[sz];
		m_sz = sz;
	}
}

}