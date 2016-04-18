#include "Model3Shader.h"
#include "render/RenderContext.h"

#include <render/render.h>

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
												const std::vector<VA_TYPE>& va_types, RenderBuffer* ib)
{
}

}