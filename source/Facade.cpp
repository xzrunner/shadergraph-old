#include "shaderlab/Facade.h"
#include "shaderlab/Statistics.h"
#include "shaderlab/RenderTask.h"

namespace sl
{

void Facade::Clear()
{
	Statistics::Instance()->Reset();
}

}