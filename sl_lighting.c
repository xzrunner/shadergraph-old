#include "sl_lighting.h"
#include "sl_shader.h"
#include "sl_math.h"
#include "sl_typedef.h"
#include "sl_utility.h"
#include "sl_buffer.h"
#include "sl_math.h"

#include <render/render.h>
#include <ds_array.h>

#include <stdlib.h>

#define STRINGIFY(A)  #A
#include "lighting.vert"
#include "lighting.frag"

#define MAX_VERTICES	1024
#define MAX_INDICES		8192

struct vertex {
	float vx, vy, vz;
	float nx, ny, nz;
};

struct shader_state {
	int shader;

	int index_buf_id, vertex_buf_id;
	struct sl_buffer *index_buf, *vertex_buf;

	int projection_id, modelview_id;
	union sl_mat4 modelview_mat, projection_mat;

	int diffuse_id, ambient_id, specular_id, shininess_id;
	int normal_matrix_id, light_position_id;
};

static struct shader_state S;

void 
sl_lighting_load() {
	int s = sl_shader_create();
	if (s < 0) {
		return;
	}

	int index_buf_id = sl_shader_create_index_buffer(MAX_INDICES, sizeof(uint16_t));
	struct sl_buffer* index_buf = sl_buf_create(sizeof(uint16_t), MAX_INDICES);
	sl_shader_set_index_buffer(s, index_buf_id, index_buf);

	int vertex_buf_id = sl_shader_create_vertex_buffer(MAX_VERTICES, sizeof(struct vertex));
	struct sl_buffer* vertex_buf = sl_buf_create(sizeof(struct vertex), MAX_VERTICES);
	sl_shader_set_vertex_buffer(s, vertex_buf_id, vertex_buf);

	struct vertex_attrib va[2] = {
		{ "position", 0, 3, sizeof(float), BUFFER_OFFSET(vertex, vx) },
		{ "normal", 0, 3, sizeof(float), BUFFER_OFFSET(vertex, nx) },
	};
	int layout_id = sl_shader_create_vertex_layout(sizeof(va)/sizeof(va[0]), va);
	sl_shader_set_vertex_layout(s, layout_id);

	sl_shader_load(s, lighting_vert, lighting_frag);

	sl_shader_set_draw_mode(s, DRAW_TRIANGLES);

	S.shader = s;

	S.index_buf_id = index_buf_id;
	S.vertex_buf_id = vertex_buf_id;
	S.index_buf = index_buf;
	S.vertex_buf = vertex_buf;

	S.projection_id = sl_shader_add_uniform(s, "u_projection", UNIFORM_FLOAT44);
	S.modelview_id = sl_shader_add_uniform(s, "u_modelview", UNIFORM_FLOAT44);
	sl_mat4_identity(&S.projection_mat);
	sl_mat4_identity(&S.modelview_mat);

	S.diffuse_id = sl_shader_add_uniform(s, "u_diffuse_material", UNIFORM_FLOAT3);
	S.ambient_id = sl_shader_add_uniform(s, "u_ambient_material", UNIFORM_FLOAT3);
	S.specular_id = sl_shader_add_uniform(s, "u_specular_material", UNIFORM_FLOAT3);
	S.shininess_id = sl_shader_add_uniform(s, "u_shininess", UNIFORM_FLOAT1);
	
	S.normal_matrix_id = sl_shader_add_uniform(s, "u_normal_matrix", UNIFORM_FLOAT33);
	S.light_position_id = sl_shader_add_uniform(s, "u_light_position", UNIFORM_FLOAT3);	
}

void 
sl_lighting_unload() {
	sl_shader_release_index_buffer(S.index_buf_id);
	sl_shader_release_vertex_buffer(S.vertex_buf_id);
	sl_buf_release(S.index_buf);
	sl_buf_release(S.vertex_buf);
	sl_shader_unload(S.shader);
}

void 
sl_lighting_bind() {
	sl_shader_bind(S.shader);
}

void 
sl_lighting_unbind() {
	sl_lighting_commit();
}

void 
sl_lighting_projection(int width, int height, float near, float far) {
	float hw = width * 0.5f;
	float hh = height * 0.5f;
	sl_mat4_perspective(&S.projection_mat, -hw, hw, -hh, hh, 20, 500);

// 	float hh = 1.0f * height / width;
// 	sl_mat4_perspective(&S.projection_mat, -100, 100, -hh*100, hh*100, 100, 300);

	sl_shader_set_uniform(S.shader, S.projection_id, UNIFORM_FLOAT44, S.projection_mat.x);
}

void 
sl_lighting_modelview(const union sl_mat4* mat) {
	memcpy(&S.modelview_mat, mat, sizeof(union sl_mat4));
	sl_shader_set_uniform(S.shader, S.modelview_id, UNIFORM_FLOAT44, S.modelview_mat.x);

	union sl_mat3 mat3;
	sl_mat4_to_mat3(&S.modelview_mat, &mat3);
	sl_lighting_set_normal_matrix(&mat3);
	sl_shader_apply_uniform(S.shader);
}

void 
sl_lighting_set_material(struct sl_vec3* ambient, struct sl_vec3* diffuse, 
                         struct sl_vec3* specular, float shininess) {
	sl_shader_set_uniform(S.shader, S.ambient_id, UNIFORM_FLOAT3, &ambient->x);
	sl_shader_set_uniform(S.shader, S.diffuse_id, UNIFORM_FLOAT3, &diffuse->x);
	sl_shader_set_uniform(S.shader, S.specular_id, UNIFORM_FLOAT3, &specular->x);
	sl_shader_set_uniform(S.shader, S.shininess_id, UNIFORM_FLOAT1, &shininess);

	union sl_mat3 mat3;
	sl_mat4_to_mat3(&S.modelview_mat, &mat3);
	sl_lighting_set_normal_matrix(&mat3);
	sl_shader_apply_uniform(S.shader);
}

void 
sl_lighting_set_normal_matrix(union sl_mat3* mat) {
	sl_shader_set_uniform(S.shader, S.normal_matrix_id, UNIFORM_FLOAT33, mat->x);
}

void 
sl_lighting_set_light_position(struct sl_vec3* pos) {
	sl_shader_set_uniform(S.shader, S.light_position_id, UNIFORM_FLOAT3, &pos->x);	
}

void 
sl_lighting_draw(struct ds_array* vertices, struct ds_array* indices) {
	int vn = ds_array_size(vertices),
		in = ds_array_size(indices);
	if (S.vertex_buf->n + vn > S.vertex_buf->cap ||
		S.index_buf->n + in > S.index_buf->cap) {
		sl_lighting_commit();
	}
	if (vn > S.vertex_buf->cap || in > S.index_buf->cap) {
		return;
	}
	sl_buf_add(S.vertex_buf, ds_array_data(vertices), ds_array_size(vertices));
	sl_buf_add(S.index_buf, ds_array_data(indices), ds_array_size(indices));
}

void 
sl_lighting_commit() {
	if (S.index_buf->n == 0) {
		return;
	}

// 	union sl_mat3 mat3;
// 	sl_mat4_to_mat3(&S.modelview_mat, &mat3);
// 	sl_lighting_set_normal_matrix(&mat3);
// 	sl_shader_apply_uniform(S.shader);

//	sl_shader_draw(S.shader, S.vertex_buf->buf, S.vertex_buf->n, S.index_buf->n);
	sl_shader_flush();
}