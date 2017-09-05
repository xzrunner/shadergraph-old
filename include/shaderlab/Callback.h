#ifndef _SHADERLAB_CALLBACK_H_
#define _SHADERLAB_CALLBACK_H_

namespace mt { class Task; }

namespace sl
{

class Callback
{
public:
	struct Funs
	{
		void (*submit_task)(mt::Task*);
	};

	static void RegisterCallback(const Funs& funs);

	//////////////////////////////////////////////////////////////////////////

	static void SubmitTask(mt::Task*);

}; // Callback

}

#endif // _SHADERLAB_CALLBACK_H_