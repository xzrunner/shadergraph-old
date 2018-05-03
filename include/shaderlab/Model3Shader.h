#ifndef _SHADERLAB_MODEL3_SHADER_H_
#define _SHADERLAB_MODEL3_SHADER_H_

#include "shaderlab/Shader.h"

#include <SM_Vector.h>
#include <SM_Matrix.h>
#include <unirender/VertexAttrib.h>

#include <memory>

#include <stdint.h>

namespace sl
{

namespace parser { class Shader; class Node; }

class RenderShader;
class RenderBuffer;
class ObserverMVP;
class ShaderProgram;

class Model3Shader : public Shader
{
public:
	struct Material
	{
		Material() : shininess(0), tex_id(0) {}
		Material(const sm::vec3& ambient, const sm::vec3& diffuse,
			const sm::vec3& specular, float shininess, int tex)
			: ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess), tex_id(tex) {}

		bool operator == (const Material& mat) const
		{
			return this->ambient == mat.ambient
				&& this->diffuse == mat.diffuse
				&& this->specular == mat.specular
				&& this->shininess == mat.shininess
				&& this->tex_id == mat.tex_id;
		}

		sm::vec3 ambient;
		sm::vec3 diffuse;
		sm::vec3 specular;
		float    shininess;
		int      tex_id;
	};

public:
	Model3Shader(RenderContext& rc);
	virtual ~Model3Shader();

	virtual void Bind() const override;
	virtual void UnBind() const override;
	virtual bool Commit() const override;

	void SetMaterial(const Material& material);
	void SetLightPosition(const sm::vec3& pos);
	void SetNormalMatrix(const sm::mat4& mat);
	void SetModelview(const sm::mat4& mat);

	void Draw(const float* vertices, size_t vertices_n, 
		const uint16_t* indices, size_t indices_n,
		bool has_normal, bool has_texcoord) const;
	void DrawVAO(unsigned int vao, size_t indices_n,
		bool has_normal, bool has_texcoord) const;

private:
	void InitCurrStatus();
	void InitVAList();
	void InitProgs();

	void InitStaticColorProg(const std::shared_ptr<RenderBuffer>& idx_buf);
	void InitGouraudShadingProg(const std::shared_ptr<RenderBuffer>& idx_buf);
	void InitTextureMapProg(const std::shared_ptr<RenderBuffer>& idx_buf);
	void InitGouraudTextureProg(const std::shared_ptr<RenderBuffer>& idx_buf);

	void ApplyUniform() const;

private:
	enum PROG_IDX {
		PI_STATIC_COLOR = 0,
		PI_GOURAUD_SHADING,
		PI_TEXTURE_MAP,
		PI_GOURAUD_TEXTURE,
		PROG_COUNT,
	};

	enum VA_TYPE {
		POSITION = 0,
		TEXCOORD,
		NORMAL,
		VA_MAX_COUNT
	};

	ShaderProgram* CreateProg(parser::Node* vert, parser::Node* frag, 
		const CU_VEC<VA_TYPE>& va_types, const std::shared_ptr<RenderBuffer>& ib) const;

	struct GouraudUniforms
	{
		int diffuse, ambient, specular, shininess;
		int normal_matrix, light_position;

		void Init(RenderShader* shader);
		void SetMaterial(RenderShader* shader, const sm::vec3& ambient, const sm::vec3& diffuse, 
			const sm::vec3& specular, float shininess);
	};

private:
	ur::VertexAttrib m_va_list[VA_MAX_COUNT];

	ShaderProgram* m_programs[PROG_COUNT];

	GouraudUniforms m_shading_uniforms, m_texture_uniforms;

	mutable int m_curr_shader;

	// curr status
	Material m_curr_material;
	sm::vec3 m_curr_light_pos;
	sm::mat4 m_curr_normal_mat;
	sm::mat4 m_curr_mv;

}; // Model3Shader

}

#endif // _SHADERLAB_MODEL3_SHADER_H_