#include "shaderlab/GouraudShading.h"
#include "shaderlab/Attribute.h"
#include "shaderlab/ParserUniform.h"
#include "shaderlab/Varying.h"

#include <cpputil/StringHelper.h>

namespace sl
{
namespace parser
{

static const char* OUTPUT_NAME = "_gouraud_col_";

GouraudShading::GouraudShading()
{
	m_attributes.push_back(new Attribute(VT_FLOAT3, "normal"));

	m_uniforms.push_back(new Uniform(VT_FLOAT3, "diffuse_material"));
	m_uniforms.push_back(new Uniform(VT_FLOAT3, "ambient_material"));
	m_uniforms.push_back(new Uniform(VT_FLOAT3, "specular_material"));
	m_uniforms.push_back(new Uniform(VT_FLOAT1, "shininess"));

	m_uniforms.push_back(new Uniform(VT_MAT3, "normal_matrix"));
	m_uniforms.push_back(new Uniform(VT_FLOAT3, "light_position"));
}

CU_STR& GouraudShading::ToStatements(CU_STR& str) const
{
	CU_STR s = "\
		vec3 eye_normal = normalize(u_normal_matrix * normal);\n \
		\
		vec4 pos4 = u_modelview * position;\n \
		vec3 pos3 = pos4.xyz / pos4.w;\n \
		vec3 light_dir = normalize(u_light_position - pos3);\n \
		\
		float diff = max(0.0, dot(eye_normal, light_dir));\n \
		\
		vec4 _TMP_ = vec4(diff * u_diffuse_material, 1);\n \
		\
		_TMP_.rgb += u_ambient_material;\n \
		\
		vec3 reflection = normalize(reflect(-light_dir, eye_normal));\n \
		float spec = max(0.0, dot(eye_normal, reflection));\n \
		spec = pow(spec, u_shininess);\n \
		_TMP_.rgb += spec * u_specular_material;\n ";
	cpputil::StringHelper::ReplaceAll(s, "_TMP_", OUTPUT_NAME);
	str += s;
	return str;
}

Variable GouraudShading::GetOutput() const
{
	return Variable(VT_FLOAT4, OUTPUT_NAME);
}

const char* GouraudShading::GetSpecularName() const
{
	return "spec";
}

}
}