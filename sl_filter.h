#ifdef __cplusplus
extern "C"
{
#endif

#ifndef shader_lab_filter_h
#define shader_lab_filter_h

#include "sl_blend.h"

enum SL_FILTER_MODE {
	SLFM_EDGE_DETECTION = 0,
	SLFM_RELIEF,
	SLFM_OUTLINE,
	SLFM_BLUR,

	SLFM_MAX_COUNT,
};

void sl_filter_load();
void sl_filter_unload();

void sl_filter_bind();
void sl_filter_unbind();

void sl_filter_projection(int width, int height);
void sl_filter_modelview(float x, float y, float sx, float sy);

void sl_filter_set_mode(enum SL_BLEND_MODE mode);

void sl_filter_draw(const float* positions, const float* texcoords, int texid);

void sl_filter_commit();

#endif // shader_lab_filter_h

#ifdef __cplusplus
}
#endif