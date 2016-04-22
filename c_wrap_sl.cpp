#include "c_wrap_sl.h"
#include "shader/ShaderMgr.h"
#include "shader/Shape2Shader.h"
#include "shader/Shape3Shader.h"
#include "shader/Sprite2Shader.h"

namespace sl
{

extern "C"
void sl_create(int max_texture)
{
	ShaderMgr::Instance()->CreateContext(max_texture);
}

extern "C"
void sl_release()
{
	ShaderMgr::Instance()->ReleaseContext();
}

extern "C"
void sl_create_shader(enum SHADER_TYPE type)
{
	ShaderMgr* mgr = ShaderMgr::Instance();
	RenderContext* rc = mgr->GetContext();

	ShaderType sl_type = MAX_SHADER;
	Shader* shader = NULL;
	switch (type)
	{
	case ST_SHAPE:
		sl_type = SHAPE2;
		shader = new Shape2Shader(rc);
		break;
	case ST_SPRITE:
		sl_type = SPRITE2;
		shader = new Sprite2Shader(rc);
		break;
	}
	if (shader) {
		mgr->CreateShader(sl_type, shader);
	}
}

extern "C"
void sl_set_shader(enum SHADER_TYPE type)
{
	ShaderType sl_type = MAX_SHADER;
	switch (type)
	{
	case ST_SHAPE:
		sl_type = SHAPE2;
		break;
	case ST_SPRITE:
		sl_type = SPRITE2;
		break;
	}
	if (type != MAX_SHADER) {
		ShaderMgr::Instance()->SetShader(sl_type);
	}
}

extern "C"
void sl_flush() 
{
	Shader* shader = ShaderMgr::Instance()->GetShader();
	if (shader) {
		shader->Commit();
	}
}

extern "C"
void sl_shape2_color(uint32_t color) 
{
	Shape2Shader* shader = static_cast<Shape2Shader*>(
		ShaderMgr::Instance()->GetShader(SHAPE2));
	if (shader) {
		shader->SetColor(color);
	}
}

extern "C"
void sl_shape2_type(int type)
{
	Shape2Shader* shader = static_cast<Shape2Shader*>(
		ShaderMgr::Instance()->GetShader(SHAPE2));
	if (shader) {
		shader->SetType(type);
	}
}

extern "C"
void sl_shape2_draw(const float* positions, int count)
{
	Shape2Shader* shader = static_cast<Shape2Shader*>(
		ShaderMgr::Instance()->GetShader(SHAPE2));
	if (shader) {
		shader->Draw(positions, count);
	}
}

extern "C"
void sl_shape2_draw_node(float x, float y, int dummy)
{
	Shape2Shader* shader = static_cast<Shape2Shader*>(
		ShaderMgr::Instance()->GetShader(SHAPE2));
	if (shader) {
		shader->Draw(x, y, dummy != 0);
	}
}

extern "C"
void sl_shape3_color(uint32_t color) 
{
	Shape3Shader* shader = static_cast<Shape3Shader*>(
		ShaderMgr::Instance()->GetShader(SHAPE3));
	if (shader) {
		shader->SetColor(color);
	}
}

extern "C"
void sl_shape3_type(int type)
{
	Shape3Shader* shader = static_cast<Shape3Shader*>(
		ShaderMgr::Instance()->GetShader(SHAPE3));
	if (shader) {
		shader->SetType(type);
	}
}

extern "C"
void sl_shape3_draw(const float* positions, int count)
{
	Shape3Shader* shader = static_cast<Shape3Shader*>(
		ShaderMgr::Instance()->GetShader(SHAPE3));
	if (shader) {
		shader->Draw(positions, count);
	}
}

extern "C"
void sl_shape3_draw_node(float x, float y, float z, int dummy)
{
	Shape3Shader* shader = static_cast<Shape3Shader*>(
		ShaderMgr::Instance()->GetShader(SHAPE3));
	if (shader) {
		shader->Draw(x, y, z, dummy != 0);
	}
}

extern "C"
void sl_sprite_set_color(uint32_t color, uint32_t additive)
{
	Sprite2Shader* shader = static_cast<Sprite2Shader*>(
		ShaderMgr::Instance()->GetShader(SPRITE2));
	if (shader) {
		shader->SetColor(color, additive);
	}
}

extern "C"
void sl_sprite_set_map_color(uint32_t rmap, uint32_t gmap, uint32_t bmap)
{
	Sprite2Shader* shader = static_cast<Sprite2Shader*>(
		ShaderMgr::Instance()->GetShader(SPRITE2));
	if (shader) {
		shader->SetColorMap(rmap, gmap, bmap);
	}
}

extern "C"
void sl_sprite_draw(const float* positions, const float* texcoords, int texid)
{
	Sprite2Shader* shader = static_cast<Sprite2Shader*>(
		ShaderMgr::Instance()->GetShader(SPRITE2));
	if (shader) {
		shader->Draw(positions, texcoords, texid);
	}
}

}