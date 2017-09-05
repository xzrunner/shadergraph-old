#ifndef _SHADERLAB_RENDER_TASK_H_
#define _SHADERLAB_RENDER_TASK_H_

#include <cooking/RenderTask.h>
#include <cooking/DisplayList.h>

namespace cooking { struct RecordedOp; }

namespace sl
{

class RenderTask : public cooking::RenderTask
{
public:
	virtual void Run();
	virtual void Flush();
	
	static RenderTask* Create(cooking::RecordedOp* op);

	static cooking::DisplayList& GetDisplayList() { return m_dlist; }

	static void Clear();

private:
	RenderTask(cooking::RecordedOp* op);
	virtual ~RenderTask() {}

private:
	cooking::RecordedOp* m_op;

	static cooking::DisplayList m_dlist;
	static mt::TaskQueue m_freelist;

}; // RenderTask

}

#endif // _SHADERLAB_RENDER_TASK_H_