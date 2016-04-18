#ifndef _SHADERLAB_MODEL3_SHADER_H_
#define _SHADERLAB_MODEL3_SHADER_H_

#include "Shader.h"
#include "render/VertexAttrib.h"

//#include <render/render.h>

#include <vector>

#include <stdint.h>

namespace sl
{

namespace parser { class Shader; class Node; }

class RenderShader;
class RenderBuffer;
class ObserverMVP;

class Model3Shader : public Shader
{
public:
	Model3Shader(RenderContext* rc);
	virtual ~Model3Shader();

	virtual void Bind() const;
	virtual void UnBind() const;
	virtual void Commit() const;

	void SetMaterial();
	void SetLightPosition();

	void Draw(const float* vertices, const uint16_t* indices,
		bool has_normal, bool has_texcoord) const;

private:
	void InitVAList();

	void InitStaticColorProg();
	void InitGouraudShadingProg();
	void InitTextureMapProg();
	void InitGouraudTextureProg();

private:
	static const int PROG_COUNT = 4;

	enum PROG_IDX {
		PI_STATIC_COLOR = 0,
		PI_GOURAUD_SHADING,
		PI_TEXTURE_MAP,
		PI_GOURAUD_TEXTURE,
	};

	struct Program
	{
		parser::Shader* parser;
		RenderShader* shader;
		ObserverMVP* mvp;
		int vertex_sz;

		Program(parser::Shader* parser, RenderShader* shader) 
			: parser(parser), shader(shader), mvp(NULL), vertex_sz(0) {}
	};

	enum VA_TYPE {
		POSITION = 0,
		TEXCOORD,
		NORMAL,
		VA_MAX_COUNT
	};

	Program* CreateProg(parser::Node* vert, parser::Node* frag, 
		const std::vector<VA_TYPE>& va_types, RenderBuffer* ib) const;

private:
	VertexAttrib m_va_list[VA_MAX_COUNT];

}; // Model3Shader

}

#endif // _SHADERLAB_MODEL3_SHADER_H_