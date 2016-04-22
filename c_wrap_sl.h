#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _shaderlab_wrap_c_h_
#define _shaderlab_wrap_c_h_

#include <stdint.h>

enum SHADER_TYPE {
	ST_NULL = 0,
	ST_SHAPE,
	ST_SPRITE,
	ST_BLEND,
	ST_FILTER,
	ST_SPRITE3,
	ST_SHAPE3,
};

void sl_create(int max_texture);
void sl_release();

void sl_create_shader(enum SHADER_TYPE type);
void sl_set_shader(enum SHADER_TYPE type);

void sl_flush();

// shape2
void sl_shape2_color(uint32_t color);
void sl_shape2_type(int type);
void sl_shape2_draw(const float* positions, int count);
void sl_shape2_draw_node(float x, float y, int dummy);

// shape3
void sl_shape3_color(uint32_t color);
void sl_shape3_type(int type);
void sl_shape3_draw(const float* positions, int count);
void sl_shape3_draw_node(float x, float y, float z, int dummy);

// sprite
void sl_sprite_set_color(uint32_t color, uint32_t additive);
void sl_sprite_set_map_color(uint32_t rmap, uint32_t gmap, uint32_t bmap);
void sl_sprite_draw(const float* positions, const float* texcoords, int texid);

#endif // _shaderlab_wrap_c_h_

#ifdef __cplusplus
}
#endif