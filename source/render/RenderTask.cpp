//#include "shaderlab/RenderTask.h"
//
////#include <cooking/RecordedOp.h>
//
//#include <assert.h>
//
//namespace sl
//{
//
////cooking::DisplayList RenderTask::m_dlist;
//mt::TaskQueue RenderTask::m_freelist;
//
//RenderTask::RenderTask(cooking::RecordedOp* op)
//	: m_op(op)
//{
//}
//
//void RenderTask::Run()
//{
//	cooking::RenderTaskMgr::Instance()->AddResult(this);
//}
//
//void RenderTask::Flush()
//{
//	if (!m_op) {
//		return;
//	}
//
////	m_op->Replay();
//	m_op = nullptr;
//}
//
//RenderTask* RenderTask::Create(cooking::RecordedOp* op)
//{
//	cooking::RenderTaskMgr::Instance()->AddCount();
//
//	mt::Task* t = m_freelist.Front();
//	RenderTask* tt = static_cast<RenderTask*>(t);
//	if (!t) {
//		tt = new RenderTask(op);
//	} else {
//		m_freelist.Pop();
//		assert(!tt->m_op);
//		tt->m_op = op;
//	}
//	return tt;
//}
//
//void RenderTask::FlushShared()
//{
////	m_dlist.Clear();
//}
//
//}