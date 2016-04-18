#include "GouraudShading.h"
#include "Attribute.h"
#include "Uniform.h"
#include "Varying.h"
#include "StringHelper.h"

namespace sl
{
namespace parser
{

GouraudShading::GouraudShading()
{
	m_attributes.push_back(new Attribute(VT_FLOAT3, "normal"));
	m_uniforms.push_back(new Uniform(VT_FLOAT3, "diffuse_material"));
	m_uniforms.push_back(new Uniform(VT_FLOAT3, "ambient_material"));
	m_uniforms.push_back(new Uniform(VT_FLOAT3, "specular_material"));
	m_uniforms.push_back(new Uniform(VT_FLOAT1, "shininess"));
}

std::string& GouraudShading::ToStatements(std::string& str) const
{
	std::string s = "\
		vec3 eye_normal = u_normal_matrix * normal;\n \
		\
		vec4 pos4 = u_modelview * position;\n \
		vec3 pos3 = pos4.xyz / pos4.w;\n \
		vec3 light_dir = normalize(u_light_position - pos3);\n \
		\
		float diff = max(0.0, dot(eye_normal, light_dir));\n \
		\
		_TMP_ = vec4(diff * u_diffuse_material, 1);\n \
		\
		_TMP_.rgb += u_ambient_material;\n \
		\
		vec3 reflection = normalize(reflect(-light_dir, eye_normal));\n \
		float spec = max(0.0, dot(eye_normal, reflection));\n \
		spec = pow(spec, u_shininess);\n \
		_TMP_.rgb += spec * u_specular_material;\n ";
	StringHelper::ReplaceAll(s, "_TMP_", OutputName());
	str += s;
	return str;
}

std::string GouraudShading::OutputName() const
{
	return "_gouraud_col_";
}

const char* GouraudShading::GetSpecularName() const
{
	return "spec";
}

}
}