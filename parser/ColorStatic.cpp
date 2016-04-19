#include "ColorStatic.h"
#include "Attribute.h"
#include "Varying.h"

namespace sl
{
namespace parser
{

static const char* OUTPUT_NAME = "_col_static_";

ColorStatic::ColorStatic(float r, float g, float b, float a)
	: m_r(r)
	, m_g(g)
	, m_b(b)
	, m_a(a)	
{
}

std::string& ColorStatic::ToStatements(std::string& str) const
{
	char buf[256];
	sprintf(buf, "vec4 %s = vec4(%f, %f, %f, %f);\n", OUTPUT_NAME, m_r, m_g, m_b, m_a);
	str += buf;
	return str;
}

Variable ColorStatic::GetOutput() const
{
	return Variable(VT_FLOAT4, OUTPUT_NAME);
}

}
}