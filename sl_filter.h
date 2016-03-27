#ifdef __cplusplus
extern "C"
{
#endif

#ifndef shader_lab_filter_h
#define shader_lab_filter_h

enum SL_FILTER_MODE {
	SLFM_EDGE_DETECTION = 0,
	SLFM_RELIEF,
	SLFM_OUTLINE,
	SLFM_BLUR,
	SLFM_GRAY,
	SLFM_HEAT_HAZE,
	SLFM_SHOCK_WAVE,
	SLFM_SWIRL,
	SLFM_GAUSSIAN_BLUR,

	SLFM_MAX_COUNT,
};

void sl_filter_load();
void sl_filter_unload();

void sl_filter_bind();
void sl_filter_unbind();

void sl_filter_projection(int width, int height);
void sl_filter_modelview(float x, float y, float sx, float sy);

void sl_filter_set_mode(enum SL_FILTER_MODE mode);

// edge detect
void sl_filter_set_edge_detect_val(float val);
// blur
void sl_filter_set_blur_val(float val);
// heat haze
void sl_filter_set_heat_haze_val(float distortion, float rise);
void sl_filter_set_heat_haze_tex(int tex);
// shock_wave
void sl_filter_set_shock_wave_center(float center[2]);
void sl_filter_set_shock_wave_params(float params[3]);
// swirl
void sl_filter_set_swirl_val(float radius, float angle, float center[2]);

void sl_filter_update(float dt);
void sl_filter_draw(const float* positions, const float* texcoords, int texid);

void sl_filter_commit();

void sl_filter_on_size(int width, int height);

#endif // shader_lab_filter_h

#ifdef __cplusplus
}
#endif