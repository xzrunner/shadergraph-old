#include "ColorStatic.h"
#include "Attribute.h"
#include "Varying.h"

namespace sl
{
namespace parser
{

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
	sprintf(buf, "vec4 _col_static_ = vec4(%f, %f, %f, %f);\n", m_r, m_g, m_b, m_a);
	str += buf;
	return str;
}

std::string ColorStatic::OutputName() const 
{ 
	return "_col_static_"; 
}

}
}