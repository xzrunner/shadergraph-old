#ifdef __cplusplus
extern "C"
{
#endif

#ifndef shader_lab_shape_h
#define shader_lab_shape_h

void sl_shape_load();
void sl_shape_unload();

void sl_shape_bind();
void sl_shape_unbind();

void sl_shape_projection(int width, int height);
void sl_shape_modelview(float x, float y, float sx, float sy);

void sl_shape_color(int color);
void sl_shape_type(int type);

void sl_shape_draw(const float* positions, int count);

void sl_shape_commit();

#endif // shader_lab_shape_h

#ifdef __cplusplus
}
#endif