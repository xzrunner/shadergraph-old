#ifndef _SHADERLAB_SPRITE_SHADER_H_
#define _SHADERLAB_SPRITE_SHADER_H_

#include "Shader.h"

#include <render/render.h>

#include <stdint.h>

namespace sl
{

namespace parser { class Shader; class Node; }

class RenderShader;
class RenderBuffer;
class ObserverMVP;

class SpriteShader : public Shader
{
public:
	SpriteShader(RenderContext* rc);	
	virtual ~SpriteShader();

	virtual void Bind() const;
	virtual void UnBind() const;
	virtual void Commit() const;

	void SetColor(uint32_t color, uint32_t additive);
	void SetColorMap(uint32_t rmap, uint32_t gmap, uint32_t bmap);

	void Draw(const float* positions, const float* texcoords, int texid) const;

private:
	void Init();
	void InitNoColorProg(RenderBuffer* idx_buf);
	void InitMultiAddColorProg(RenderBuffer* idx_buf);
	void InitMapColorProg(RenderBuffer* idx_buf);
	void InitFullColorProg(RenderBuffer* idx_buf);

private:
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

	struct Vertex
	{
		float vx, vy;
		float tx, ty;
		uint32_t color, additive;
		uint32_t rmap, gmap, bmap;
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

	Program* CreateProg(parser::Node* vert, parser::Node* frag, int va_count,
		vertex_attrib* va, int vertex_sz, RenderBuffer* ib) const;

private:
	Program* m_programs[PROG_SIZE];

	uint32_t m_color, m_additive;
	uint32_t m_rmap, m_gmap, m_bmap;

	mutable int m_texid;

	Vertex* m_vertex_buf;
	mutable int m_quad_sz;

	mutable int m_prog_type;

}; // SpriteShader

}

#endif // _SHADERLAB_SPRITE_SHADER_H_