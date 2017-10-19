#ifndef _SHADERLAB_PARSER_STRING_HELPER_H_
#define _SHADERLAB_PARSER_STRING_HELPER_H_

#include <cu/cu_stl.h>

namespace sl
{
namespace parser
{

class StringHelper
{
public:
	static void ReplaceAll(CU_STR& str, const CU_STR& from, const CU_STR& to);

}; // StringHelper

}
}

#endif // _SHADERLAB_PARSER_STRING_HELPER_H_