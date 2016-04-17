#ifndef _SHADERLAB_SPRITE_SHADER_H_
#define _SHADERLAB_SPRITE_SHADER_H_

#include "Shader.h"
#include "render/VertexAttrib.h"

#include <render/render.h>

#include <string>
#include <vector>

namespace sl
{

namespace parser { class Shader; class Node; }

class RenderShader;
class RenderBuffer;
class ObserverMVP;

class SpriteShader : public Shader
{
public:
	SpriteShader(RenderContext* rc, int position_sz, int max_vertex, 
		bool vertex_index);	
	virtual ~SpriteShader();

	virtual void Bind() const;
	virtual void UnBind() const;

	void SetColor(uint32_t color, uint32_t additive);
	void SetColorMap(uint32_t rmap, uint32_t gmap, uint32_t bmap);

protected:
	virtual void InitMVP(ObserverMVP* mvp) const = 0;

	void InitProgs();

protected:
	static const int PROG_SIZE = 4;

	enum PROG_IDX {
		PI_NO_COLOR = 0,
		PI_MULTI_ADD_COLOR,
		PI_MAP_COLOR,
		PI_FULL_COLOR,
	};

	enum PROG_TYPE {
		PT_NULL				= 0,
		PT_MULTI_ADD_COLOR	= 1,
		PT_MAP_COLOR		= 2,
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
		COLOR,
		ADDITIVE,
		RMAP,
		GMAP,
		BMAP,
		VA_MAX_COUNT
	};

	Program* CreateProg(parser::Node* vert, parser::Node* frag, 
		const std::vector<VA_TYPE>& va_types, RenderBuffer* ib) const;

private:
	void InitVAList(int position_sz);

	void InitNoColorProg(RenderBuffer* idx_buf);
	void InitMultiAddColorProg(RenderBuffer* idx_buf);
	void InitMapColorProg(RenderBuffer* idx_buf);
	void InitFullColorProg(RenderBuffer* idx_buf);

protected:
	Program* m_programs[PROG_SIZE];

	uint32_t m_color, m_additive;
	uint32_t m_rmap, m_gmap, m_bmap;

	mutable int m_texid;

	mutable int m_quad_sz;

	mutable int m_prog_type;

private:
	int m_max_vertex;
	bool m_vertex_index;

	VertexAttrib m_va_list[VA_MAX_COUNT];

}; // SpriteShader

}

#endif // _SHADERLAB_SPRITE_SHADER_H_