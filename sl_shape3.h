#ifdef __cplusplus
extern "C"
{
#endif

#ifndef shader_lab_shape3_h
#define shader_lab_shape3_h

#include <stdbool.h>

void sl_shape3_load();
void sl_shape3_unload();

void sl_shape3_bind();
void sl_shape3_unbind();

void sl_shape3_projection(const union sm_mat4* mat);
void sl_shape3_modelview(const union sm_mat4* mat);

void sl_shape3_color(int color);
void sl_shape3_type(int type);

void sl_shape3_draw(const float* positions, int count);
void sl_shape3_draw_node(float x, float y, float z, bool dummy);

void sl_shape3_commit();

#endif // shader_lab_shape3_h

#ifdef __cplusplus
}
#endif