#include "c_wrap_sl_deferred.h"

#include "shaderlab/RenderTask.h"
#include "shaderlab/Callback.h"

#include <cooking.h>
#include <cooking/RecordedOp.h>

namespace sl
{

extern "C"
void sl_render_clear_deferred(unsigned long argb) {
	cooking::RecordedOp* op = new (RenderTask::GetDisplayList().GetAlloc()) cooking::RenderClearOp(argb);
	RenderTask* task = RenderTask::Create(op);
	Callback::SubmitTask(task);
}

}