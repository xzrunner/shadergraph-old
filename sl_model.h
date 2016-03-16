#ifdef __cplusplus
extern "C"
{
#endif

#ifndef shader_lab_model_h
#define shader_lab_model_h

void sl_model_load();
void sl_model_unload();

void sl_model_bind();
void sl_model_unbind();

void sl_model_projection(int width, int height, float near, float far);
void sl_model_modelview(float x, float y, float sx, float sy);

void sl_model_draw(const float* positions, const float* texcoords, int texid, int vertices_count);

void sl_model_commit();

#endif // shader_lab_model_h

#ifdef __cplusplus
}
#endif