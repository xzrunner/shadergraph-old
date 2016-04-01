#ifdef __cplusplus
extern "C"
{
#endif

#ifndef shader_lab_shader_h
#define shader_lab_shader_h

#include <render/render.h>

struct sl_buffer;

void sl_shader_mgr_create(int max_texture, void (*commit_func)());
void sl_shader_mgr_release();
struct render* sl_shader_get_render();

int  sl_shader_create();
//void sl_shader_reset();
void sl_shader_load(int id, const char* vs, const char* fs);
void sl_shader_unload(int id);

void sl_shader_blend(int m1, int m2);
void sl_shader_defaultblend();

void sl_shader_set_draw_mode(int id, enum DRAW_MODE dm);

void sl_shader_set_texture(int id, int channel);
int  sl_shader_get_texture();
void sl_shader_set_target(int id);
int  sl_shader_get_target();

int  sl_shader_create_vertex_buffer(int n, int stride);
void sl_shader_set_vertex_buffer(int id, int buf_id, struct sl_buffer* buf);
void sl_shader_release_vertex_buffer(int buf_id);

int  sl_shader_create_index_buffer(int n, int stride);
void sl_shader_set_index_buffer(int id, int buf_id, struct sl_buffer* buf);
void sl_shader_release_index_buffer(int buf_id);

void sl_shader_update_buffer(int buf_id, struct sl_buffer* buf);

int sl_shader_create_vertex_layout(int n, struct vertex_attrib* va);
void sl_shader_set_vertex_layout(int id, int layout_id);

void sl_shader_apply_uniform(int id);
void sl_shader_bind(int id);

int  sl_shader_add_uniform(int id, const char* name, enum UNIFORM_FORMAT t);
void sl_shader_set_uniform(int id, int index, enum UNIFORM_FORMAT t, const float* v);
int  sl_shader_uniform_size(enum UNIFORM_FORMAT t);

void sl_shader_draw(int id, void* data, int vb_n, int ib_n);

void sl_shader_clear(unsigned long argb);

// fixme
void sl_shader_commit();

void sl_shader_flush();

int  sl_shader_version();
void sl_shader_scissortest(int enable);

#endif // shader_lab_shader_h

#ifdef __cplusplus
}
#endif