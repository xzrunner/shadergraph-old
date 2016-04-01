#ifdef __cplusplus
extern "C"
{
#endif

#ifndef shader_lab_lighting_h
#define shader_lab_lighting_h

struct sm_vec3;
union sm_mat3;
union sm_mat4;
struct ds_array;

void sl_lighting_load();
void sl_lighting_unload();

void sl_lighting_bind();
void sl_lighting_unbind();

void sl_lighting_projection(int width, int height, float near, float far);
void sl_lighting_modelview(const union sm_mat4* mat);

void sl_lighting_set_material(const struct sm_vec3* ambient, const struct sm_vec3* diffuse, 
	                          const struct sm_vec3* specular, float shininess);
void sl_lighting_set_normal_matrix(const union sm_mat3* mat);
void sl_lighting_set_light_position(const struct sm_vec3* pos);

void sl_lighting_draw(struct ds_array* vertices, struct ds_array* indices);

void sl_lighting_commit();

#endif // shader_lab_lighting_h

#ifdef __cplusplus
}
#endif