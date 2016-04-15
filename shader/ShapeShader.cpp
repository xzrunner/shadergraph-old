#include "ShapeShader.h"
#include "utility/Buffer.h"
#include "render/RenderContext.h"
#include "render/RenderBuffer.h"
#include "render/RenderShader.h"
#include "render/RenderLayout.h"

namespace sl
{

#define BUFFER_OFFSET(f) ((intptr_t)&(((Vertex*)NULL)->f))

ShapeShader::ShapeShader(RenderContext* rc)
{
	m_shader = rc->CreateShader();

	Buffer* buf = new Buffer(sizeof(struct Vertex), MAX_VERTICES);
	RenderBuffer* vb = new RenderBuffer(rc->GetEJRender(), VERTEXBUFFER, sizeof(struct Vertex), MAX_VERTICES, buf);
	m_shader->SetVertexBuffer(vb);
	vb->Release();

	struct vertex_attrib va[2] = {
		{ "position", 0, 2, sizeof(float), BUFFER_OFFSET(vx) },
		{ "color", 0, 4, sizeof(uint8_t), BUFFER_OFFSET(color) },
	};
	RenderLayout* lo = new RenderLayout(rc->GetEJRender(), sizeof(va)/sizeof(va[0]), va);
	m_shader->SetLayout(lo);
	lo->Release();

	m_shader->Load(shape_vert, shape_frag);

	m_project.id = m_shader->AddUniform("u_projection", UNIFORM_FLOAT44);
	m_modelview.id = m_shader->AddUniform("u_modelview", UNIFORM_FLOAT44);

	m_color = 0xffffffff;

	rc->SetClearFlag(MASKC);
}

ShapeShader::~ShapeShader()
{
	m_shader->Unload();
	delete m_shader;
}

void ShapeShader::Bind() const
{
	m_shader->Bind();
}

void ShapeShader::UnBind() const
{
}

void ShapeShader::Commit() const
{
	
}

void ShapeShader::SetProjection(int width, int height)
{

}

void ShapeShader::SetModelview(float x, float y, float sx, float sy)
{

}

void ShapeShader::SetColor(int color)
{

}

void ShapeShader::SetType(int type)
{

}

void ShapeShader::Draw(const float* positions, int count) const
{

}

void ShapeShader::Draw(float x, float y, bool dummy) const
{

}

void ShapeShader::InitParser()
{
	parser::Node* vert = new parser::PositionTrans();
	parser::Node* frag = new parser::ColorOne();
	frag->Connect(new parser::FragColor());
	m_parser = new parser::Shader(vert, frag);
}

}