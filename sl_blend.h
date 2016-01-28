#ifdef __cplusplus
extern "C"
{
#endif

#ifndef shader_lab_blend_h
#define shader_lab_blend_h

#include <stdint.h>

enum SL_BLEND_MODE {
	// normal
	SLBM_NORMAL		= 0,
	SLBM_DISSOLVE,			// todo

	// darken modes
	SLBM_DARKEN		= 10,
	SLBM_MULTIPLY,
	SLBM_COLOR_BURN,
	SLBM_LINEAR_BURN,
	SLBM_DARKER_COLOR,		// todo

	// lighten modes
	SLBM_LIGHTEN	= 20,
	SLBM_SCREEN,
	SLBM_COLOR_DODGE,
	SLBM_LINEAR_DODGE,
	SLBM_LIGHTER_COLOR,		// todo

	// saturation modes
	SLBM_OVERLAY	= 30,
	SLBM_SOFT_LIGHT,
	SLBM_HARD_LIGHT,
	SLBM_VIVID_LIGHT,
	SLBM_LINEAR_LIGHT,
	SLBM_PIN_LIGHT,
	SLBM_HARD_MIX,

	// substraction modes
	SLBM_DIFFERENCE	= 40,
	SLBM_EXCLUSION,

	// color modes 
	SLBM_HUE		= 50,	// todo
	SLBM_SATURATION,		// todo
	SLBM_COLOR,				// todo
	SLBM_LUMINOSITY,		// todo

	// others
	SLBM_UNKNOWN	= 99,
};

void sl_blend_load();
void sl_blend_unload();

void sl_blend_bind();
void sl_blend_unbind();

void sl_blend_projection(int width, int height);
void sl_blend_modelview(float x, float y, float sx, float sy);

void sl_blend_set_color(uint32_t color, uint32_t additive);
void sl_blend_set_mode(enum SL_BLEND_MODE mode);

void sl_blend_draw(const float* positions, const float* texcoords_blend, 
				   const float* texcoords_base, int tex_blend, int tex_base);

void sl_blend_commit();

#endif // shader_lab_blend_h

#ifdef __cplusplus
}
#endif