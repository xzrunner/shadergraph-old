#include "ColorOne.h"
#include "Attribute.h"
#include "Varying.h"

namespace sl
{
namespace parser
{

static const char* OUTPUT_NAME = "_col_one_";

ColorOne::ColorOne()
{
	m_attributes.push_back(new Attribute(VT_FLOAT4, "color"));
	m_varyings.push_back(new Varying(VT_FLOAT4, "color"));
}

std::string& ColorOne::ToStatements(std::string& str) const
{
	char buf[128];
	sprintf(buf, "vec4 %s = v_color;\n", OUTPUT_NAME);
	str += buf;
	return str;
}

Variable ColorOne::GetOutput() const
{
	return Variable(VT_FLOAT4, OUTPUT_NAME);
}

}
}