#include "ColorOne.h"
#include "Attribute.h"
#include "Varying.h"

namespace sl
{
namespace parser
{

ColorOne::ColorOne()
{
	m_attributes.push_back(new Attribute(VT_FLOAT4, "color"));
	m_varyings.push_back(new Varying(VT_FLOAT4, "color"));
}

std::string& ColorOne::ToStatements(std::string& str) const
{
	str += "vec4 _col_one_ = v_color;\n";
	return str;
}

std::string ColorOne::OutputName() const 
{ 
	return "_col_one_"; 
}

}
}