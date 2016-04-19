#include "Model3Shader.h"
#include "SubjectMVP3.h"
#include "ShaderProgram.h"
#include "render/RenderContext.h"
#include "render/RenderShader.h"
#include "parser/PositionTrans.h"
#include "parser/ColorStatic.h"
#include "parser/FragColor.h"
#include "parser/GouraudShading.h"
#include "parser/VaryingSender.h"
#include "parser/VaryingReceiver.h"
#include "parser/TextureMap.h"

#include <render/render.h>
#include <sm.h>

namespace sl
{

static const int MAX_VERTICES = 10000;
static const int MAX_INDICES = 20000;

Model3Shader::Model3Shader(RenderContext* rc)
	: Shader(rc)
{
	m_rc->SetClearFlag(MASKC | MASKD);

	
}

Model3Shader::~Model3Shader()
{

}

void Model3Shader::Bind() const
{

}

void Model3Shader::UnBind() const
{

}

void Model3Shader::Commit() const
{
	
}

void Model3Shader::SetMaterial()
{

}

void Model3Shader::SetLightPosition()
{

}

void Model3Shader::Draw(const float* vertices, const uint16_t* indices, 
						bool has_normal, bool has_texcoord) const
{
	
}

void Model3Shader::InitVAList()
{
	m_va_list[POSITION].Assign("position", 3, sizeof(float));
	m_va_list[TEXCOORD].Assign("texcoord", 2, sizeof(float));
	m_va_list[NORMAL].Assign("normal", 3, sizeof(float));	
}

void Model3Shader::InitStaticColorProg()
{
	parser::Node* vert = new parser::PositionTrans();
	parser::Node* frag = new parser::ColorStatic(0.5, 0.5, 0, 1);
	frag->Connect(new parser::FragColor());

	std::vector<VA_TYPE> va_types;
	va_types.push_back(POSITION);
	m_programs[PI_STATIC_COLOR] = CreateProg(vert, frag, va_types);
}

void Model3Shader::InitGouraudShadingProg()
{
	std::string varying_name = "gouraud_dst";

	parser::Node* vert = new parser::PositionTrans();
	vert->Connect(
		new parser::GouraudShading())->Connect(
		new parser::VaryingSender(parser::Variable(parser::VT_FLOAT4, varying_name)));

	parser::Node* frag = new parser::VaryingReceiver(parser::Variable(parser::VT_FLOAT4, varying_name));
	frag->Connect(new parser::FragColor());

	std::vector<VA_TYPE> va_types;
	va_types.push_back(POSITION);
	va_types.push_back(NORMAL);
	m_programs[PI_GOURAUD_SHADING] = CreateProg(vert, frag, va_types);
}

void Model3Shader::InitTextureMapProg()
{
	parser::Node* vert = new parser::PositionTrans();

	parser::Node* frag = new parser::TextureMap();

	std::vector<VA_TYPE> va_types;
	va_types.push_back(POSITION);
	va_types.push_back(TEXCOORD);
	m_programs[PI_TEXTURE_MAP] = CreateProg(vert, frag, va_types);
}

void Model3Shader::InitGouraudTextureProg()
{
	parser::Node* vert = new parser::PositionTrans();

//	parser::Node* frag = new parser::
}

ShaderProgram* Model3Shader::CreateProg(parser::Node* vert, parser::Node* frag, 
										const std::vector<VA_TYPE>& va_types) const
{
	ShaderProgram* prog = new ShaderProgram(m_rc, MAX_VERTICES);

	std::vector<VertexAttrib> va_list;
	for (int i = 0, n = va_types.size(); i < n; ++i) {
		va_list.push_back(m_va_list[va_types[i]]);
	}

	prog->Load(vert, frag, va_list, NULL, true);

	SubjectMVP3::Instance()->Register(prog->GetMVP());

	prog->GetShader()->SetDrawMode(DRAW_TRIANGLES);

	return prog;
}

//////////////////////////////////////////////////////////////////////////
// Model3Shader::GouraudUniforms
//////////////////////////////////////////////////////////////////////////

void Model3Shader::GouraudUniforms::
Init(RenderShader* shader)
{
	diffuse = shader->AddUniform("u_diffuse_material", UNIFORM_FLOAT3);
	ambient = shader->AddUniform("u_ambient_material", UNIFORM_FLOAT3);
	specular = shader->AddUniform("u_specular_material", UNIFORM_FLOAT3);
	shininess = shader->AddUniform("u_shininess", UNIFORM_FLOAT1);

	normal_matrix = shader->AddUniform("u_normal_matrix", UNIFORM_FLOAT33);
	light_position = shader->AddUniform("u_light_position", UNIFORM_FLOAT3);
}

void Model3Shader::GouraudUniforms::
SetMaterial(RenderShader* shader, const sm_vec3* ambient, const sm_vec3* diffuse, 
			const sm_vec3* specular, float shininess) 
{
	shader->SetUniform(this->diffuse, UNIFORM_FLOAT3, &ambient->x);
	shader->SetUniform(this->ambient, UNIFORM_FLOAT3, &diffuse->x);
	shader->SetUniform(this->specular, UNIFORM_FLOAT3, &specular->x);
	shader->SetUniform(this->shininess, UNIFORM_FLOAT1, &shininess);
}

}