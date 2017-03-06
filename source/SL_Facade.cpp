#include "SL_Facade.h"
#include "Statistics.h"

namespace sl
{

void Facade::Clear()
{
	Statistics::Instance()->Reset();
}

}