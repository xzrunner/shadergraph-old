#include "Model3Shader.h"
#include "render/RenderContext.h"
#include "render/RenderShader.h"

#include <render/render.h>
#include <sm.h>

namespace sl
{

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

}

void Model3Shader::InitGouraudShadingProg()
{

}

void Model3Shader::InitTextureMapProg()
{

}

void Model3Shader::InitGouraudTextureProg()
{

}

Model3Shader::Program* Model3Shader::CreateProg(parser::Node* vert, parser::Node* frag, 
												const std::vector<VA_TYPE>& va_types, RenderBuffer* ib) const
{
// 	// shader
// 	parser::Shader* parser = new parser::Shader(vert, frag);
// 	RenderShader* shader = m_rc->CreateShader();
// 	Program* prog = new Program(parser, shader);

	return NULL;
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