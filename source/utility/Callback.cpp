#include "shaderlab/Callback.h"

namespace sl
{

static Callback::Funs FUNS;
	
void Callback::RegisterCallback(const Callback::Funs& funs)
{
	FUNS = funs;
}

void Callback::SubmitTask(mt::Task* task)
{ 
	FUNS.submit_task(task);
}

}