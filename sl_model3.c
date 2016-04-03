#include "sl_model3.h"
#include "sl_shader.h"
#include "sl_typedef.h"
#include "sl_utility.h"
#include "sl_buffer.h"

#include <ds_array.h>
#include <sm.h>
#include <render/render.h>

#include <stdlib.h>
#include <string.h>

#define STRINGIFY(A)  #A
#include "model3.vert"
#include "model3.frag"

#define MAX_VERTICES	10000
#define MAX_INDICES		20000

struct vertex {
	float vx, vy, vz;
	float nx, ny, nz;
};

struct shader_state {
	int shader;

	int index_buf_id, vertex_buf_id;
	struct sl_buffer *index_buf, *vertex_buf;

	int projection_id, modelview_id;
	union sm_mat4 modelview_mat, projection_mat;

	int diffuse_id, ambient_id, specular_id, shininess_id;
	int normal_matrix_id, light_position_id;
};

static struct shader_state S;

void 
sl_model3_load() {
	int s = sl_shader_create();
	if (s < 0) {
		return;
	}

	int index_buf_id = sl_shader_create_index_buffer(MAX_INDICES, sizeof(uint16_t));
	struct sl_buffer* index_buf = sl_buf_create(sizeof(uint16_t), MAX_INDICES);
	sl_shader_set_index_buffer(s, index_buf_id, index_buf);
	sl_shader_update_buffer(index_buf_id, index_buf);
	index_buf->n = 0;

	int vertex_buf_id = sl_shader_create_vertex_buffer(MAX_VERTICES, sizeof(struct vertex));
	struct sl_buffer* vertex_buf = sl_buf_create(sizeof(struct vertex), MAX_VERTICES);
	sl_shader_set_vertex_buffer(s, vertex_buf_id, vertex_buf);

	struct vertex_attrib va[2] = {
		{ "position", 0, 3, sizeof(float), BUFFER_OFFSET(vertex, vx) },
		{ "normal", 0, 3, sizeof(float), BUFFER_OFFSET(vertex, nx) },
	};
	int layout_id = sl_shader_create_vertex_layout(sizeof(va)/sizeof(va[0]), va);
	sl_shader_set_vertex_layout(s, layout_id);

	sl_shader_load(s, model3_vert, model3_frag);

	sl_shader_set_draw_mode(s, DRAW_TRIANGLES);

	S.shader = s;

	S.index_buf_id = index_buf_id;
	S.vertex_buf_id = vertex_buf_id;
	S.index_buf = index_buf;
	S.vertex_buf = vertex_buf;

	S.projection_id = sl_shader_add_uniform(s, "u_projection", UNIFORM_FLOAT44);
	S.modelview_id = sl_shader_add_uniform(s, "u_modelview", UNIFORM_FLOAT44);
	sm_mat4_identity(&S.projection_mat);
	sm_mat4_identity(&S.modelview_mat);

	S.diffuse_id = sl_shader_add_uniform(s, "u_diffuse_material", UNIFORM_FLOAT3);
	S.ambient_id = sl_shader_add_uniform(s, "u_ambient_material", UNIFORM_FLOAT3);
	S.specular_id = sl_shader_add_uniform(s, "u_specular_material", UNIFORM_FLOAT3);
	S.shininess_id = sl_shader_add_uniform(s, "u_shininess", UNIFORM_FLOAT1);
	
	S.normal_matrix_id = sl_shader_add_uniform(s, "u_normal_matrix", UNIFORM_FLOAT33);
	S.light_position_id = sl_shader_add_uniform(s, "u_light_position", UNIFORM_FLOAT3);	

	sl_shader_set_clear_flag(MASKC | MASKD);
}

void 
sl_model3_unload() {
	sl_shader_release_index_buffer(S.index_buf_id);
	sl_shader_release_vertex_buffer(S.vertex_buf_id);
	sl_buf_release(S.index_buf);
	sl_buf_release(S.vertex_buf);
	sl_shader_unload(S.shader);
}

void 
sl_model3_bind() {
	sl_shader_bind(S.shader);
}

void 
sl_model3_unbind() {
	sl_model3_commit();
}

void 
sl_model3_projection(float aspect) {
	sm_mat4_perspective(&S.projection_mat, -aspect, aspect, -1, 1, 1, 9999);
	sl_shader_set_uniform(S.shader, S.projection_id, UNIFORM_FLOAT44, S.projection_mat.x);
}

void 
sl_model3_modelview(const union sm_mat4* mat) {
	memcpy(&S.modelview_mat, mat, sizeof(union sm_mat4));
	sl_shader_set_uniform(S.shader, S.modelview_id, UNIFORM_FLOAT44, S.modelview_mat.x);

	union sm_mat3 mat3;
	sm_mat4_to_mat3(&mat3, &S.modelview_mat);
	sl_model3_set_normal_matrix(&mat3);
	sl_shader_apply_uniform(S.shader);
}

void 
sl_model3_set_material(const struct sm_vec3* ambient, const struct sm_vec3* diffuse, 
                         const struct sm_vec3* specular, float shininess) {
	sl_shader_set_uniform(S.shader, S.ambient_id, UNIFORM_FLOAT3, &ambient->x);
	sl_shader_set_uniform(S.shader, S.diffuse_id, UNIFORM_FLOAT3, &diffuse->x);
	sl_shader_set_uniform(S.shader, S.specular_id, UNIFORM_FLOAT3, &specular->x);
	sl_shader_set_uniform(S.shader, S.shininess_id, UNIFORM_FLOAT1, &shininess);
	sl_shader_apply_uniform(S.shader);
}

void 
sl_model3_set_normal_matrix(const union sm_mat3* mat) {
	sl_shader_set_uniform(S.shader, S.normal_matrix_id, UNIFORM_FLOAT33, mat->x);
}

void 
sl_model3_set_light_position(const struct sm_vec3* pos) {
	sl_shader_set_uniform(S.shader, S.light_position_id, UNIFORM_FLOAT3, &pos->x);	
}

void 
sl_model3_draw(struct ds_array* vertices, struct ds_array* indices) {
	int vn = ds_array_size(vertices),
		in = ds_array_size(indices);
	if (S.vertex_buf->n + vn > S.vertex_buf->cap ||
		S.index_buf->n + in > S.index_buf->cap) {
		sl_model3_commit();
	}
	if (vn > S.vertex_buf->cap || in > S.index_buf->cap) {
		return;
	}

	// todo: 
	for (int i = 0, n = ds_array_size(indices); i < n; ++i) {
		uint16_t idx = *(uint16_t*)ds_array_fetch(indices, i);
		idx += S.vertex_buf->n;
		sl_buf_add(S.index_buf, &idx, 1);
	}
	sl_buf_add(S.vertex_buf, ds_array_data(vertices), ds_array_size(vertices));
}

void 
sl_model3_commit() {
	if (S.index_buf->n == 0) {
		return;
	}
	struct render* r = sl_shader_get_render();
	render_setdepth(r, DEPTH_LESS_EQUAL);
	sl_shader_flush();
	render_setdepth(r, DEPTH_DISABLE);
}