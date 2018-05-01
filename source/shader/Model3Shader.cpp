#include "shaderlab/Model3Shader.h"
#include "shaderlab/SubjectMVP3.h"
//#include "shaderlab/ObserverMVP.h"
#include "shaderlab/ShaderProgram.h"
#include "shaderlab/Utility.h"
#include "shaderlab/ShaderType.h"
#include "shaderlab/ShaderMgr.h"
#include "shaderlab/RenderShader.h"
#include "shaderlab/RenderBuffer.h"
#include "shaderlab/PositionTrans.h"
#include "shaderlab/AttributeNode.h"
#include "shaderlab/VaryingNode.h"
#include "shaderlab/FragColor.h"
#include "shaderlab/GouraudShading.h"
#include "shaderlab/VaryingNode.h"
#include "shaderlab/TextureMap.h"
#include "shaderlab/Assign.h"
#include "shaderlab/Mul2.h"
#include "shaderlab/StackAllocator.h"
#include "shaderlab/ObserverMVP.h"
#include "shaderlab/RenderContext.h"

#include <unirender/RenderContext.h>
#include <unirender/Blackboard.h>

namespace sl
{

static const int MAX_VERTICES = 10000;
static const int MAX_INDICES = 20000;

Model3Shader::Model3Shader(RenderContext& rc)
	: Shader(rc)
	, m_curr_shader(-1)
{
	ur::Blackboard::Instance()->GetRenderContext().SetClearFlag(ur::MASKC | ur::MASKD);

	InitCurrStatus();
	InitVAList();
	InitProgs();
}

Model3Shader::~Model3Shader()
{
	for (int i = 0; i < PROG_COUNT; ++i) {
		delete m_programs[i];
	}
}

void Model3Shader::Bind() const
{
	auto& ur_rc = ur::Blackboard::Instance()->GetRenderContext();
//	ur_rc.EnableDepth(true);
	ur_rc.SetDepthFormat(ur::DEPTH_LESS_EQUAL);
}

void Model3Shader::UnBind() const
{
	auto& ur_rc = ur::Blackboard::Instance()->GetRenderContext();
//	ur_rc.EnableDepth(false);
	ur_rc.SetDepthFormat(ur::DEPTH_DISABLE);
}

bool Model3Shader::Commit() const
{
	if (m_curr_shader < 0) {
		return false;
	}

	RenderShader* shader = m_programs[m_curr_shader]->GetShader();
	m_rc.GetShaderMgr().BindRenderShader(shader, MODEL3);
	return shader->Commit();
}

void Model3Shader::SetMaterial(const Material& material)
{
	if (m_curr_material == material) {
		return;
	}	
	Commit();
	m_curr_material = material;
	
	m_shading_uniforms.SetMaterial(m_programs[PI_GOURAUD_SHADING]->GetShader(), 
		material.ambient, material.diffuse, material.specular, material.shininess);
	m_shading_uniforms.SetMaterial(m_programs[PI_GOURAUD_TEXTURE]->GetShader(), 
		material.ambient, material.diffuse, material.specular, material.shininess);
	if (material.tex_id >= 0) {
		//m_rc.GetContext().EnableDepth(true);
		//m_rc.GetContext().SetDepthFormat(ur::DEPTH_LESS_EQUAL);
		ur::Blackboard::Instance()->GetRenderContext().BindTexture(material.tex_id, 0);
	}
}

void Model3Shader::SetLightPosition(const sm::vec3& pos)
{
	if (m_curr_light_pos == pos) {
		return;
	}
	Commit();
	m_curr_light_pos = pos;

	m_programs[PI_GOURAUD_SHADING]->GetShader()->SetUniform(m_shading_uniforms.light_position, ur::UNIFORM_FLOAT3, &pos.x);
	m_programs[PI_GOURAUD_TEXTURE]->GetShader()->SetUniform(m_shading_uniforms.light_position, ur::UNIFORM_FLOAT3, &pos.x);
}

void Model3Shader::SetNormalMatrix(const sm::mat4& mat)
{
	if (m_curr_normal_mat == mat) {
		return;
	}
	Commit();
	m_curr_normal_mat = mat;

	sm::mat3 mat3(mat);
	m_programs[PI_GOURAUD_SHADING]->GetShader()->SetUniform(m_shading_uniforms.normal_matrix, ur::UNIFORM_FLOAT33, mat3.x);
	m_programs[PI_GOURAUD_TEXTURE]->GetShader()->SetUniform(m_shading_uniforms.normal_matrix, ur::UNIFORM_FLOAT33, mat3.x);
}

void Model3Shader::SetModelview(const sm::mat4& mat)
{
	if (m_curr_mv == mat) {
		return;
	}
	Commit();
	m_curr_mv = mat;

	for (int i = 0; i < PROG_COUNT; ++i) {
		m_programs[i]->GetMVP()->SetModelview(&mat);
	}
}

void Model3Shader::Draw(const float* vertices, size_t vertices_n, 
	                    const uint16_t* indices, size_t indices_n,
	                    bool has_normal, bool has_texcoord) const
{
	int stride = 3;
	if (has_normal) {
		stride += 3;
	}
	if (has_texcoord) {
		stride += 2;
	}
	assert(vertices_n % stride == 0);
	
	PROG_IDX idx = PI_STATIC_COLOR;
	if (!has_normal && !has_texcoord) idx = PI_STATIC_COLOR;
	if ( has_normal && !has_texcoord) idx = PI_GOURAUD_SHADING;
	if (!has_normal &&  has_texcoord) idx = PI_TEXTURE_MAP;
	if ( has_normal &&  has_texcoord) idx = PI_GOURAUD_TEXTURE;
	if (idx != m_curr_shader) {
		Commit();
		m_curr_shader = idx;
		m_rc.GetShaderMgr().BindRenderShader(m_programs[idx]->GetShader(), MODEL3);
	}

	RenderShader* shader = m_programs[m_curr_shader]->GetShader();
	auto& vb = shader->GetVertexBuffer();
	auto& ib = shader->GetIndexBuffer();
	int vn = vertices_n / stride,
		in = indices_n;
	if (vb->Size() + vn > vb->Capacity() || 
		ib->Size() + in > ib->Capacity()) {
		Commit();
	}
	if (vn > vb->Capacity() || in > ib->Capacity()) {
		return;
	}

	shader->Draw(vertices, vn, indices, in);
}

void Model3Shader::InitCurrStatus()
{
	static const float max = std::numeric_limits<float>::max();

	m_curr_material.ambient.Set(max, max, max);
	m_curr_material.diffuse.Set(max, max, max);
	m_curr_material.specular.Set(max, max, max);
	m_curr_material.shininess = max;
	m_curr_material.tex_id = -1;

	m_curr_light_pos.Set(max, max, max);

	memset(m_curr_normal_mat.x, 0xff, sizeof(m_curr_normal_mat.x));

	memset(m_curr_mv.x, 0xff, sizeof(m_curr_mv.x));
}

void Model3Shader::InitVAList()
{
	m_va_list[POSITION].Assign("position", 3, sizeof(float));
	m_va_list[TEXCOORD].Assign("texcoord", 2, sizeof(float));
	m_va_list[NORMAL].Assign("normal", 3, sizeof(float));	
}

void Model3Shader::InitProgs()
{
	auto& ur_rc = ur::Blackboard::Instance()->GetRenderContext();
	auto idx_buf = Utility::CreateIndexBuffer(ur_rc, MAX_INDICES);
	InitStaticColorProg(idx_buf);
	InitGouraudShadingProg(idx_buf);
 	InitTextureMapProg(idx_buf);
 	InitGouraudTextureProg(idx_buf);
}

void Model3Shader::InitStaticColorProg(const std::shared_ptr<RenderBuffer>& idx_buf)
{
	parser::Node* vert = new parser::PositionTrans();
	parser::Node* frag = new parser::Assign(parser::Variable(parser::VT_FLOAT4, "_col_static_"), 0.5, 0.5, 0, 1);
	frag->Connect(new parser::FragColor());

	CU_VEC<VA_TYPE> va_types;
	va_types.push_back(POSITION);
	m_programs[PI_STATIC_COLOR] = CreateProg(vert, frag, va_types, idx_buf);
}

void Model3Shader::InitGouraudShadingProg(const std::shared_ptr<RenderBuffer>& idx_buf)
{
	CU_STR varying_name = "gouraud_dst";

	parser::Node* vert = new parser::PositionTrans();
	vert->Connect(
		new parser::GouraudShading())->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, varying_name)));

	parser::Node* frag = new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, varying_name));
	frag->Connect(new parser::FragColor());

	CU_VEC<VA_TYPE> va_types;
	va_types.push_back(POSITION);
	va_types.push_back(NORMAL);
	m_programs[PI_GOURAUD_SHADING] = CreateProg(vert, frag, va_types, idx_buf);

	m_shading_uniforms.Init(m_programs[PI_GOURAUD_SHADING]->GetShader());
}

void Model3Shader::InitTextureMapProg(const std::shared_ptr<RenderBuffer>& idx_buf)
{
	parser::Node* vert = new parser::PositionTrans();
	vert->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT2, "texcoord")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT2, "texcoord")));

	parser::Node* frag = new parser::TextureMap();
	frag->Connect(new parser::FragColor());

	CU_VEC<VA_TYPE> va_types;
	va_types.push_back(POSITION);
	va_types.push_back(TEXCOORD);
	m_programs[PI_TEXTURE_MAP] = CreateProg(vert, frag, va_types, idx_buf);
}

// todo
void Model3Shader::InitGouraudTextureProg(const std::shared_ptr<RenderBuffer>& idx_buf)
{
	const char* gouraud_dst_name = "gouraud_dst";

	parser::Node* varying = new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, gouraud_dst_name));
	parser::Node* vert = new parser::PositionTrans();
	vert->Connect(
		new parser::GouraudShading())->Connect(
		varying)->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT2, "texcoord")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT2, "texcoord")));

	parser::Node* tex_map = new parser::TextureMap();
	parser::Node* frag = new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, gouraud_dst_name));
	frag->Connect(tex_map)->Connect(
		new parser::Mul2(parser::Variable(parser::VT_FLOAT4, "tmp"), varying->GetOutput(), tex_map->GetOutput()))->Connect(
		new parser::FragColor());

	CU_VEC<VA_TYPE> va_types;
	va_types.push_back(POSITION);
	va_types.push_back(TEXCOORD);
	va_types.push_back(NORMAL);
	m_programs[PI_GOURAUD_TEXTURE] = CreateProg(vert, frag, va_types, idx_buf);

	m_shading_uniforms.Init(m_programs[PI_GOURAUD_TEXTURE]->GetShader());
}

ShaderProgram* Model3Shader::CreateProg(parser::Node* vert, parser::Node* frag, 
										const CU_VEC<VA_TYPE>& va_types,
										const std::shared_ptr<RenderBuffer>& ib) const
{
	ShaderProgram* prog = new ShaderProgram(m_rc, MAX_VERTICES);

	CU_VEC<ur::VertexAttrib> va_list;
	for (int i = 0, n = va_types.size(); i < n; ++i) {
		va_list.push_back(m_va_list[va_types[i]]);
	}

	prog->Load(vert, frag, va_list, ib, true);

	m_rc.GetSubMVP3().Register(prog->GetMVP());

	prog->GetShader()->SetDrawMode(ur::DRAW_TRIANGLES);

	return prog;
}

/************************************************************************/
/* class Model3Shader::GouraudUniforms                                  */
/************************************************************************/

void Model3Shader::GouraudUniforms::
Init(RenderShader* shader)
{
	diffuse = shader->AddUniform("u_diffuse_material", ur::UNIFORM_FLOAT3);
	ambient = shader->AddUniform("u_ambient_material", ur::UNIFORM_FLOAT3);
	specular = shader->AddUniform("u_specular_material", ur::UNIFORM_FLOAT3);
	shininess = shader->AddUniform("u_shininess", ur::UNIFORM_FLOAT1);

	normal_matrix = shader->AddUniform("u_normal_matrix", ur::UNIFORM_FLOAT33);
	light_position = shader->AddUniform("u_light_position", ur::UNIFORM_FLOAT3);
}

void Model3Shader::GouraudUniforms::
SetMaterial(RenderShader* shader, const sm::vec3& ambient, const sm::vec3& diffuse, 
			const sm::vec3& specular, float shininess) 
{
	shader->SetUniform(this->ambient, ur::UNIFORM_FLOAT3, &ambient.x);
	shader->SetUniform(this->diffuse, ur::UNIFORM_FLOAT3, &diffuse.x);
	shader->SetUniform(this->specular, ur::UNIFORM_FLOAT3, &specular.x);
	shader->SetUniform(this->shininess, ur::UNIFORM_FLOAT1, &shininess);
}

}