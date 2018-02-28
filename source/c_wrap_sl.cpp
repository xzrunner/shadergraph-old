#include "c_wrap_sl.h"
#include "shaderlab/ShaderMgr.h"
#include "shaderlab/Shape2Shader.h"
#include "shaderlab/Shape3Shader.h"
#include "shaderlab/Sprite2Shader.h"
#include "shaderlab/Sprite3Shader.h"
#include "shaderlab/BlendShader.h"
#include "shaderlab/FilterShader.h"
#include "shaderlab/Model3Shader.h"
#include "shaderlab/MaskShader.h"
#include "shaderlab/SubjectMVP2.h"
#include "shaderlab/SubjectMVP3.h"
#include "shaderlab/HeatHazeProg.h"
#include "shaderlab/BurningMapProg.h"
#include "shaderlab/ColGradingProg.h"
#include "shaderlab/RenderShader.h"
#include "shaderlab/Statistics.h"
#include "shaderlab/Blackboard.h"
#include "shaderlab/ShaderMgr.h"
#include "shaderlab/RenderContext.h"

#include <unirender/RenderContext.h>
#include <unirender/Blackboard.h>
#include <sm_c_vector.h>
#include <sm_c_matrix.h>

namespace sl
{

/**
 *  @brief
 *    common
 */

//extern "C"
//void sl_create(void* render_context)
//{
//	Blackboard::Instance()->GetRenderContext().GetShaderMgr().
//		SetContext(static_cast<ur::RenderContext*>(render_context));
//}

extern "C"
void sl_release()
{
	auto& rc = Blackboard::Instance()->GetRenderContext();
	rc.GetSubMVP2().Clear();
	rc.GetSubMVP3().Clear();
}

extern "C"
void sl_create_shader(enum SHADER_TYPE type)
{
	auto& rc = Blackboard::Instance()->GetRenderContext();

	ShaderType sl_type = MAX_SHADER;
	Shader* shader = nullptr;
	switch (type)
	{
	case ST_SHAPE2:
		sl_type = SHAPE2;
		shader = new Shape2Shader(rc);
		break;
	case ST_SHAPE3:
		sl_type = SHAPE3;
		shader = new Shape3Shader(rc);
		break;
	case ST_SPRITE2:
		sl_type = SPRITE2;
		shader = new Sprite2Shader(rc);
		break;
	case ST_SPRITE3:
		sl_type = SPRITE3;
		shader = new Sprite3Shader(rc);
		break;
	case ST_BLEND:
		sl_type = BLEND;
		shader = new BlendShader(rc);
		break;
	case ST_FILTER:
		sl_type = FILTER;
		shader = new FilterShader(rc);
		break;
	case ST_MODEL3:
		sl_type = MODEL3;
		shader = new Model3Shader(rc);
		break;
	case ST_MASK:
		sl_type = MASK;
		shader = new MaskShader(rc);
		break;
	default:
		break;
	}
	if (shader) {
		rc.GetShaderMgr().CreateShader(sl_type, shader);
	}
}

extern "C"
void sl_release_shader(enum SHADER_TYPE type) {
	Blackboard::Instance()->GetRenderContext().GetShaderMgr().ReleaseShader((ShaderType)type);
}

extern "C"
void sl_set_shader(enum SHADER_TYPE type)
{
	if (type >= 0 && type < ST_MAX_SHADER) {
		Blackboard::Instance()->GetRenderContext().GetShaderMgr().SetShader(ShaderType(type));
	}
}

extern "C"
int  sl_is_shader(enum SHADER_TYPE type) {
	if (type >= 0 && type < ST_MAX_SHADER) {
		return SHADER_TYPE(Blackboard::Instance()->GetRenderContext().GetShaderMgr().GetShaderType()) == type ? 1 : 0;
	} else {
		return 0;
	}	
}

extern "C"
void sl_on_projection2(int w, int h) {
	Blackboard::Instance()->GetRenderContext().GetSubMVP2().NotifyProjection(w, h);
}

extern "C"
void sl_on_projection3(const sm_mat4* mat) {
	Blackboard::Instance()->GetRenderContext().GetSubMVP3().NotifyProjection(sm::mat4(mat->x));
}

extern "C"
void sl_on_modelview2(float x, float y, float sx, float sy) {
	Blackboard::Instance()->GetRenderContext().GetSubMVP2().NotifyModelview(x, y, sx, sy);
}

extern "C"
void sl_on_modelview3(const sm_mat4* mat) {
	Blackboard::Instance()->GetRenderContext().GetSubMVP3().NotifyModelview(sm::mat4(mat->x));

	Model3Shader* shader = static_cast<Model3Shader*>(Blackboard::Instance()->GetRenderContext().GetShaderMgr().GetShader(MODEL3));
	if (shader) {
		shader->SetNormalMatrix(sm::mat4(mat->x));
	}
}

extern "C"
void sl_set_texture(int id) {
	ur::Blackboard::Instance()->GetRenderContext().BindTexture(id, 0);
}

extern "C"
int  sl_get_texture() {
	return ur::Blackboard::Instance()->GetRenderContext().GetCurrTexture();
}

extern "C"
void sl_set_target(int id) {
	ur::Blackboard::Instance()->GetRenderContext().BindRenderTarget(id);
}

extern "C"
void sl_set_blend(int m1, int m2) {
	ur::Blackboard::Instance()->GetRenderContext().SetBlend(m1, m2);
}

extern "C"
void sl_set_default_blend() {
	ur::Blackboard::Instance()->GetRenderContext().SetDefaultBlend();
}

extern "C"
void sl_set_blend_equation(int func) {
	ur::Blackboard::Instance()->GetRenderContext().SetBlendEquation(func);
}

extern "C"
void sl_render_clear(unsigned long argb) {
	ur::Blackboard::Instance()->GetRenderContext().Clear(argb);
}

extern "C"
int  sl_render_version() {
	return ur::Blackboard::Instance()->GetRenderContext().RenderVersion();
}

extern "C"
void sl_enable_scissor(int enable) {
	ur::Blackboard::Instance()->GetRenderContext().EnableScissor(enable);
}

extern "C"
void sl_set_viewport(int x, int y, int w, int h) {
	ur::Blackboard::Instance()->GetRenderContext().SetViewport(x, y, w, h);
}

extern "C"
void sl_flush() 
{
	Shader* shader = Blackboard::Instance()->GetRenderContext().GetShaderMgr().GetShader();
	if (shader) {
		shader->Commit();
	}
}

extern "C"
void* sl_get_render_context()
{
	return &ur::Blackboard::Instance()->GetRenderContext();
}

/**
 *  @brief
 *    shape2 shader
 */

extern "C"
void sl_shape2_color(uint32_t color) 
{
	auto& mgr = Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	if (Shape2Shader* shader = static_cast<Shape2Shader*>(mgr.GetShader(SHAPE2))) {
		shader->SetColor(color);
	}
}

extern "C"
void sl_shape2_type(int type)
{
	auto& mgr = Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	if (Shape2Shader* shader = static_cast<Shape2Shader*>(mgr.GetShader(SHAPE2))) {
		shader->SetType(type);
	}
}

extern "C"
void sl_shape2_draw(const float* positions, int count)
{
	auto& mgr = Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	if (Shape2Shader* shader = static_cast<Shape2Shader*>(mgr.GetShader(SHAPE2))) {
		shader->Draw(positions, count);
	}
}

extern "C"
void sl_shape2_draw_with_color(const float* positions, const uint32_t* colors, int count) {
	auto& mgr = Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	if (Shape2Shader* shader = static_cast<Shape2Shader*>(mgr.GetShader(SHAPE2))) {
		shader->Draw(positions, colors, count);
	}	
}

extern "C"
void sl_shape2_draw_node(float x, float y, int dummy)
{
	auto& mgr = Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	if (Shape2Shader* shader = static_cast<Shape2Shader*>(mgr.GetShader(SHAPE2))) {
		shader->Draw(x, y, dummy != 0);
	}
}

/**
 *  @brief
 *    shape3 shader
 */

extern "C"
void sl_shape3_color(uint32_t color) 
{
	auto& mgr = Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	if (Shape3Shader* shader = static_cast<Shape3Shader*>(mgr.GetShader(SHAPE3))) {
		shader->SetColor(color);
	}
}

extern "C"
void sl_shape3_type(int type)
{
	auto& mgr = Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	if (Shape3Shader* shader = static_cast<Shape3Shader*>(mgr.GetShader(SHAPE3))) {
		shader->SetType(type);
	}
}

extern "C"
void sl_shape3_draw(const float* positions, int count)
{
	auto& mgr = Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	if (Shape3Shader* shader = static_cast<Shape3Shader*>(mgr.GetShader(SHAPE3))) {
		shader->Draw(positions, count);
	}
}

extern "C"
void sl_shape3_draw_node(float x, float y, float z, int dummy)
{
	auto& mgr = Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	if (Shape3Shader* shader = static_cast<Shape3Shader*>(mgr.GetShader(SHAPE3))) {
		shader->Draw(x, y, z, dummy != 0);
	}
}

/**
 *  @brief
 *    sprite2 shader
 */

extern "C"
void sl_sprite2_set_color(uint32_t color, uint32_t additive)
{
	auto& mgr = Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	if (Sprite2Shader* shader = static_cast<Sprite2Shader*>(mgr.GetShader(SPRITE2))) {
		shader->SetColor(color, additive);
	}
}

extern "C"
void sl_sprite2_set_map_color(uint32_t rmap, uint32_t gmap, uint32_t bmap)
{
	auto& mgr = Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	if (Sprite2Shader* shader = static_cast<Sprite2Shader*>(mgr.GetShader(SPRITE2))) {
		shader->SetColorMap(rmap, gmap, bmap);
	}
}

extern "C"
void sl_sprite2_draw(const float* positions, const float* texcoords, int texid)
{
	auto& mgr = Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	if (Sprite2Shader* shader = static_cast<Sprite2Shader*>(mgr.GetShader(SPRITE2))) {
		shader->DrawQuad(positions, texcoords, texid);
	}
}

/**
 *  @brief
 *    sprite3 shader
 */

extern "C"
void sl_sprite3_set_color(uint32_t color, uint32_t additive) {
	auto& mgr = Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	if (Sprite3Shader* shader = static_cast<Sprite3Shader*>(mgr.GetShader(SPRITE3))) {
		shader->SetColor(color, additive);
	}
}

extern "C"
void sl_sprite3_set_map_color(uint32_t rmap, uint32_t gmap, uint32_t bmap) {
	auto& mgr = Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	if (Sprite3Shader* shader = static_cast<Sprite3Shader*>(mgr.GetShader(SPRITE3))) {
		shader->SetColorMap(rmap, gmap, bmap);
	}
}

extern "C"
void sl_sprite3_draw(const float* positions, const float* texcoords, int texid) {
	auto& mgr = Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	if (Sprite3Shader* shader = static_cast<Sprite3Shader*>(mgr.GetShader(SPRITE3))) {
		shader->Draw(positions, texcoords, texid);
	}
}

/**
 *  @brief
 *    filter shader
 */

extern "C"
void sl_filter_set_mode(int mode) {
	auto& mgr = Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	if (FilterShader* shader = static_cast<FilterShader*>(mgr.GetShader(FILTER))) {
		shader->SetMode(FILTER_MODE(mode));
	}
}

extern "C"
void sl_filter_set_heat_haze_factor(float distortion, float rise) {
	auto& mgr = Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	FilterShader* shader = static_cast<FilterShader*>(mgr.GetShader(FILTER));
	if (shader) {
		HeatHazeProg* prog = static_cast<HeatHazeProg*>(shader->GetProgram(FM_HEAT_HAZE));
		if (prog) {
			prog->SetFactor(distortion, rise);
		}
	}
}

extern "C"
void sl_filter_set_heat_haze_texture(int id) {
	auto& mgr = Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	FilterShader* shader = static_cast<FilterShader*>(mgr.GetShader(FILTER));
	if (shader) {
		HeatHazeProg* prog = static_cast<HeatHazeProg*>(shader->GetProgram(FM_HEAT_HAZE));
		if (prog) {
			prog->SetDistortionMapTex(id);
		}
	}
}

extern "C"
void sl_filter_set_burning_map_upper_texture(int id) {
	auto& mgr = Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	FilterShader* shader = static_cast<FilterShader*>(mgr.GetShader(FILTER));
	if (shader) {
		BurningMapProg* prog = static_cast<BurningMapProg*>(shader->GetProgram(FM_BURNING_MAP));
		if (prog) {
			prog->SetUpperTex(id);
		}
	}
}

extern "C"
void sl_filter_set_burning_map_height_texture(int id) {
	auto& mgr = Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	FilterShader* shader = static_cast<FilterShader*>(mgr.GetShader(FILTER));
	if (shader) {
		BurningMapProg* prog = static_cast<BurningMapProg*>(shader->GetProgram(FM_BURNING_MAP));
		if (prog) {
			prog->SetHeightMapTex(id);
		}
	}
}

extern "C"
void sl_filter_set_burning_map_border_texture(int id) {
	auto& mgr = Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	FilterShader* shader = static_cast<FilterShader*>(mgr.GetShader(FILTER));
	if (shader) {
		BurningMapProg* prog = static_cast<BurningMapProg*>(shader->GetProgram(FM_BURNING_MAP));
		if (prog) {
			prog->SetBorderGradientTex(id);
		}
	}
}

extern "C"
void sl_filter_set_col_grading_texture(int id) {
	auto& mgr = Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	FilterShader* shader = static_cast<FilterShader*>(mgr.GetShader(FILTER));
	if (!shader) {
		return;
	}
	{
		ColGradingProg* prog = static_cast<ColGradingProg*>(shader->GetProgram(FM_COL_GRADING));
		if (prog) {
			prog->SetLUTTex(id);
		}
	}
	{
		ColGradingProg* prog = static_cast<ColGradingProg*>(shader->GetProgramWithColor(FM_COL_GRADING));
		if (prog) {
			prog->SetLUTTex(id);
		}
	}
}

extern "C"
void sl_filter_update(float dt) {
	auto& mgr = Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	if (FilterShader* shader = static_cast<FilterShader*>(mgr.GetShader(FILTER))) {
		shader->UpdateTime(dt);
	}
}

extern "C"
void sl_filter_clear_time() {
	auto& mgr = Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	if (FilterShader* shader = static_cast<FilterShader*>(mgr.GetShader(FILTER))) {
		shader->ClearTime();
	}
}

extern "C"
void sl_filter_set_color(uint32_t color, uint32_t additive)
{
	auto& mgr = Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	if (FilterShader* shader = static_cast<FilterShader*>(mgr.GetShader(FILTER))) {
		shader->SetColor(color, additive);
	}
}

extern "C"
void sl_filter_draw(const float* positions, const float* texcoords, int texid) {
	auto& mgr = Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	if (FilterShader* shader = static_cast<FilterShader*>(mgr.GetShader(FILTER))) {
		shader->Draw(positions, texcoords, texid);
	}
}

/**
 *  @brief
 *    blend shader
 */

extern "C"
void sl_blend_set_mode(int mode) {
	auto& mgr = Blackboard::Instance()->GetRenderContext().GetShaderMgr();	
	if (BlendShader* shader = static_cast<BlendShader*>(mgr.GetShader(BLEND))) {
		shader->SetMode(mode);
	}
}

extern "C"
void sl_blend_set_color(uint32_t color, uint32_t additive) {
	auto& mgr = Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	if (BlendShader* shader = static_cast<BlendShader*>(mgr.GetShader(BLEND))) {
		shader->SetColor(color, additive);
	}
}

extern "C"
void sl_blend_draw(const float* positions, const float* texcoords_blend, 
				   const float* texcoords_base, int tex_blend, int tex_base) {
	auto& mgr = Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	if (BlendShader* shader = static_cast<BlendShader*>(mgr.GetShader(BLEND))) {
		shader->Draw(positions, texcoords_blend, texcoords_base, tex_blend, tex_base);
	}
}

/**
 *  @brief
 *    mask shader
 */

extern "C"
void sl_mask_draw(const float* positions, const float* texcoords, 
				  const float* texcoords_mask, int tex, int tex_mask) {
	auto& mgr = Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	if (MaskShader* shader = static_cast<MaskShader*>(mgr.GetShader(MASK))) {
		shader->Draw(positions, texcoords, texcoords_mask, tex, tex_mask);
	}
}

/**
 *  @brief
 *    stat
 */

extern "C"
void sl_stat_set_render(bool b) {
	Statistics::Instance()->SetRender(b);
}

extern "C"
int sl_stat_get_drawcall() {
	return Statistics::Instance()->GetDrawCall();
}

extern "C"
int sl_stat_get_vertices() {
	return Statistics::Instance()->GetVertices();
}

}