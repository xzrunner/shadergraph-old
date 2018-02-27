#ifndef _SHADERLAB_SPRITE_SHADER_H_
#define _SHADERLAB_SPRITE_SHADER_H_

#include "shaderlab/Shader.h"

#include <cu/cu_stl.h>
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

class SpriteShader : public Shader
{
public:
	SpriteShader(RenderContext& rc, int position_sz, 
		int max_vertex, bool vertex_index);	
	virtual ~SpriteShader();

	virtual void Bind() const override;
	virtual void UnBind() const override;

	void SetColor(uint32_t color, uint32_t additive);
	void SetColorMap(uint32_t rmap, uint32_t gmap, uint32_t bmap);

	int GetQuadCount() const { return m_quad_sz; }

protected:
	virtual void InitMVP(ObserverMVP* mvp) const = 0;

	void InitProgs();

protected:
	enum PROG_IDX {
		PI_NO_COLOR = 0,
		PI_MULTI_ADD_COLOR,
		PI_MAP_COLOR,
		PI_FULL_COLOR,
		PROG_COUNT
	};

	enum PROG_TYPE {
		PT_NULL				= 0,
		PT_MULTI_ADD_COLOR	= 1,
		PT_MAP_COLOR		= 2,
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

	ShaderProgram* CreateProg(parser::Node* vert, parser::Node* frag, 
		const CU_VEC<VA_TYPE>& va_types, const std::shared_ptr<RenderBuffer>& ib) const;

private:
	void InitVAList(int position_sz);

	void InitNoColorProg(const std::shared_ptr<RenderBuffer>& idx_buf);
	void InitMultiAddColorProg(const std::shared_ptr<RenderBuffer>& idx_buf);
	void InitMapColorProg(const std::shared_ptr<RenderBuffer>& idx_buf);
	void InitFullColorProg(const std::shared_ptr<RenderBuffer>& idx_buf);

protected:
	ShaderProgram* m_programs[PROG_COUNT];

	uint32_t m_color, m_additive;
	uint32_t m_rmap, m_gmap, m_bmap;

	mutable int m_texid;

	mutable int m_quad_sz;

	mutable int m_prog_type;

private:
	int m_max_vertex;
	bool m_vertex_index;

	ur::VertexAttrib m_va_list[VA_MAX_COUNT];

}; // SpriteShader

}

#endif // _SHADERLAB_SPRITE_SHADER_H_