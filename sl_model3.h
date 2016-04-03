#ifdef __cplusplus
extern "C"
{
#endif

#ifndef shader_lab_model3_h
#define shader_lab_model3_h

struct sm_vec3;
union sm_mat3;
union sm_mat4;
struct ds_array;

void sl_model3_load();
void sl_model3_unload();

void sl_model3_bind();
void sl_model3_unbind();

void sl_model3_projection(float aspect);
void sl_model3_modelview(const union sm_mat4* mat);

void sl_model3_set_material(const struct sm_vec3* ambient, const struct sm_vec3* diffuse, 
	                          const struct sm_vec3* specular, float shininess);
void sl_model3_set_normal_matrix(const union sm_mat3* mat);
void sl_model3_set_light_position(const struct sm_vec3* pos);

void sl_model3_draw(struct ds_array* vertices, struct ds_array* indices);

void sl_model3_commit();

#endif // shader_lab_model3_h

#ifdef __cplusplus
}
#endif