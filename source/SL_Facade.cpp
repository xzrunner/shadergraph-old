#include "SL_Facade.h"
#include "shaderlab/Statistics.h"

namespace sl
{

void Facade::Clear()
{
	Statistics::Instance()->Reset();
}

}