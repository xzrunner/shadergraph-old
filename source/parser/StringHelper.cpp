#include "shaderlab/StringHelper.h"

namespace sl
{
namespace parser
{

void StringHelper::ReplaceAll(CU_STR& str, const CU_STR& from, const CU_STR& to)
{
	if(from.empty()) {
		return;
	}
	size_t start_pos = 0;
	while((start_pos = str.find(from, start_pos)) != CU_STR::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
}

}
}