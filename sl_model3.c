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
#include "model3_plain.vert"
#include "model3_plain.frag"
#include "model3_normal.vert"
#include "model3_normal.frag"
#include "model3_texture.vert"
#include "model3_texture.frag"
#include "model3_both.vert"
#include "model3_both.frag"

#define MAX_VERTICES	10000
#define MAX_INDICES		20000

#define TYPE_NORMAL		1
#define TYPE_TEXCOORD	2

enum SHADER_IDX {
	IDX_PLAIN = 0,
	IDX_NORMAL,
	IDX_TEXTURE,
	IDX_BOTH,
};

#define MAX_SHADER_COUNT 4

struct vertex_plain {
	struct sm_vec3 pos;
};

struct vertex_normal {
	struct sm_vec3 pos;
	struct sm_vec3 normal;
};

struct vertex_texture {
	struct sm_vec3 pos;
	struct sm_vec2 texcoord;
};

struct vertex_both {
	struct sm_vec3 pos;
	struct sm_vec3 normal;
	struct sm_vec2 texcoord;
};

struct uniform_light {
	int diffuse, ambient, specular, shininess;
	int normal_matrix, light_position;
};

struct shader_state {
	int shader[MAX_SHADER_COUNT];

	int index_buf_id;
	int vertex_buf_id[MAX_SHADER_COUNT];
	struct sl_buffer *index_buf;
	struct sl_buffer *vertex_buf[MAX_SHADER_COUNT];

	int projection_id[MAX_SHADER_COUNT];
	int modelview_id[MAX_SHADER_COUNT];
	union sm_mat4 modelview_mat, projection_mat;

	struct uniform_light u_normal;
	struct uniform_light u_both;

	int curr_shader;
};

static struct shader_state S;

static void
_uniform_light_init(int shader, struct uniform_light* uniform) {
	uniform->diffuse = sl_shader_add_uniform(shader, "u_diffuse_material", UNIFORM_FLOAT3);
	uniform->ambient = sl_shader_add_uniform(shader, "u_ambient_material", UNIFORM_FLOAT3);
	uniform->specular = sl_shader_add_uniform(shader, "u_specular_material", UNIFORM_FLOAT3);
	uniform->shininess = sl_shader_add_uniform(shader, "u_shininess", UNIFORM_FLOAT1);

	uniform->normal_matrix = sl_shader_add_uniform(shader, "u_normal_matrix", UNIFORM_FLOAT33);
	uniform->light_position = sl_shader_add_uniform(shader, "u_light_position", UNIFORM_FLOAT3);
}

static void
_uniform_light_set_material(int shader, const struct uniform_light* uniform, 
							const struct sm_vec3* ambient, const struct sm_vec3* diffuse, 
							const struct sm_vec3* specular, float shininess) {
	sl_shader_set_uniform(shader, uniform->ambient, UNIFORM_FLOAT3, &ambient->x);
	sl_shader_set_uniform(shader, uniform->diffuse, UNIFORM_FLOAT3, &diffuse->x);
	sl_shader_set_uniform(shader, uniform->specular, UNIFORM_FLOAT3, &specular->x);
	sl_shader_set_uniform(shader, uniform->shininess, UNIFORM_FLOAT1, &shininess);
}

static void
_create_plain_shader(int index_buf_id, struct sl_buffer* index_buf) {
	int s = sl_shader_create();
	if (s < 0) {
		return;
	}

	sl_shader_set_index_buffer(s, index_buf_id, index_buf);

	int vertex_buf_id = sl_shader_create_vertex_buffer(MAX_VERTICES, sizeof(struct vertex_plain));
	struct sl_buffer* vertex_buf = sl_buf_create(sizeof(struct vertex_plain), MAX_VERTICES);
	sl_shader_set_vertex_buffer(s, vertex_buf_id, vertex_buf);

	struct vertex_attrib va[1] = {
		{ "position", 0, 3, sizeof(float), BUFFER_OFFSET(vertex_plain, pos.x) },
	};
	int layout_id = sl_shader_create_vertex_layout(sizeof(va)/sizeof(va[0]), va);
	sl_shader_set_vertex_layout(s, layout_id);

	sl_shader_load(s, model3_plain_vert, model3_plain_frag);

	sl_shader_set_draw_mode(s, DRAW_TRIANGLES);

	S.shader[IDX_PLAIN] = s;
	S.vertex_buf_id[IDX_PLAIN] = vertex_buf_id;
	S.vertex_buf[IDX_PLAIN] = vertex_buf;
	S.projection_id[IDX_PLAIN] = sl_shader_add_uniform(s, "u_projection", UNIFORM_FLOAT44);
	S.modelview_id[IDX_PLAIN] = sl_shader_add_uniform(s, "u_modelview", UNIFORM_FLOAT44);
}

static void
_create_normal_shader(int index_buf_id, struct sl_buffer* index_buf) {
	int s = sl_shader_create();
	if (s < 0) {
		return;
	}

	sl_shader_set_index_buffer(s, index_buf_id, index_buf);

	int vertex_buf_id = sl_shader_create_vertex_buffer(MAX_VERTICES, sizeof(struct vertex_normal));
	struct sl_buffer* vertex_buf = sl_buf_create(sizeof(struct vertex_normal), MAX_VERTICES);
	sl_shader_set_vertex_buffer(s, vertex_buf_id, vertex_buf);

	struct vertex_attrib va[2] = {
		{ "position", 0, 3, sizeof(float), BUFFER_OFFSET(vertex_normal, pos.x) },
		{ "normal", 0, 3, sizeof(float), BUFFER_OFFSET(vertex_normal, normal.x) },
	};
	int layout_id = sl_shader_create_vertex_layout(sizeof(va)/sizeof(va[0]), va);
	sl_shader_set_vertex_layout(s, layout_id);

	sl_shader_load(s, model3_normal_vert, model3_normal_frag);

	sl_shader_set_draw_mode(s, DRAW_TRIANGLES);

	S.shader[IDX_NORMAL] = s;
	S.vertex_buf_id[IDX_NORMAL] = vertex_buf_id;
	S.vertex_buf[IDX_NORMAL] = vertex_buf;
	S.projection_id[IDX_NORMAL] = sl_shader_add_uniform(s, "u_projection", UNIFORM_FLOAT44);
	S.modelview_id[IDX_NORMAL] = sl_shader_add_uniform(s, "u_modelview", UNIFORM_FLOAT44);

	_uniform_light_init(s, &S.u_normal);
}

static void
_create_texture_shader(int index_buf_id, struct sl_buffer* index_buf) {
	int s = sl_shader_create();
	if (s < 0) {
		return;
	}

	sl_shader_set_index_buffer(s, index_buf_id, index_buf);

	int vertex_buf_id = sl_shader_create_vertex_buffer(MAX_VERTICES, sizeof(struct vertex_texture));
	struct sl_buffer* vertex_buf = sl_buf_create(sizeof(struct vertex_texture), MAX_VERTICES);
	sl_shader_set_vertex_buffer(s, vertex_buf_id, vertex_buf);

	struct vertex_attrib va[2] = {
		{ "position", 0, 3, sizeof(float), BUFFER_OFFSET(vertex_texture, pos.x) },
		{ "texture", 0, 2, sizeof(float), BUFFER_OFFSET(vertex_texture, texcoord.x) },
	};
	int layout_id = sl_shader_create_vertex_layout(sizeof(va)/sizeof(va[0]), va);
	sl_shader_set_vertex_layout(s, layout_id);

	sl_shader_load(s, model3_texture_vert, model3_texture_frag);

	sl_shader_set_draw_mode(s, DRAW_TRIANGLES);

	S.shader[IDX_TEXTURE] = s;
	S.vertex_buf_id[IDX_TEXTURE] = vertex_buf_id;
	S.vertex_buf[IDX_TEXTURE] = vertex_buf;
	S.projection_id[IDX_TEXTURE] = sl_shader_add_uniform(s, "u_projection", UNIFORM_FLOAT44);
	S.modelview_id[IDX_TEXTURE] = sl_shader_add_uniform(s, "u_modelview", UNIFORM_FLOAT44);
}

static void
_create_both_shader(int index_buf_id, struct sl_buffer* index_buf) {
	int s = sl_shader_create();
	if (s < 0) {
		return;
	}

	sl_shader_set_index_buffer(s, index_buf_id, index_buf);

	int vertex_buf_id = sl_shader_create_vertex_buffer(MAX_VERTICES, sizeof(struct vertex_both));
	struct sl_buffer* vertex_buf = sl_buf_create(sizeof(struct vertex_both), MAX_VERTICES);
	sl_shader_set_vertex_buffer(s, vertex_buf_id, vertex_buf);

	struct vertex_attrib va[3] = {
		{ "position", 0, 3, sizeof(float), BUFFER_OFFSET(vertex_both, pos.x) },
		{ "normal", 0, 3, sizeof(float), BUFFER_OFFSET(vertex_both, normal.x) },
		{ "texcoord", 0, 2, sizeof(float), BUFFER_OFFSET(vertex_both, texcoord.x) },
	};
	int layout_id = sl_shader_create_vertex_layout(sizeof(va)/sizeof(va[0]), va);
	sl_shader_set_vertex_layout(s, layout_id);

	sl_shader_load(s, model3_both_vert, model3_both_frag);

	sl_shader_set_draw_mode(s, DRAW_TRIANGLES);

	S.shader[IDX_BOTH] = s;
	S.vertex_buf_id[IDX_BOTH] = vertex_buf_id;
	S.vertex_buf[IDX_BOTH] = vertex_buf;
	S.projection_id[IDX_BOTH] = sl_shader_add_uniform(s, "u_projection", UNIFORM_FLOAT44);
	S.modelview_id[IDX_BOTH] = sl_shader_add_uniform(s, "u_modelview", UNIFORM_FLOAT44);

	_uniform_light_init(s, &S.u_both);
}

void 
sl_model3_load() {
	int index_buf_id = sl_shader_create_index_buffer(MAX_INDICES, sizeof(uint16_t));
	struct sl_buffer* index_buf = sl_buf_create(sizeof(uint16_t), MAX_INDICES);
	sl_shader_update_buffer(index_buf_id, index_buf);
	index_buf->n = 0;

	_create_plain_shader(index_buf_id, index_buf);
	_create_normal_shader(index_buf_id, index_buf);
	_create_texture_shader(index_buf_id, index_buf);
	_create_both_shader(index_buf_id, index_buf);

	sm_mat4_identity(&S.projection_mat);
	sm_mat4_identity(&S.modelview_mat);

	S.index_buf_id = index_buf_id;
	S.index_buf = index_buf;

	S.curr_shader = -1;

	sl_shader_set_clear_flag(MASKC | MASKD);
}

void 
sl_model3_unload() {
	sl_shader_release_index_buffer(S.index_buf_id);
	sl_buf_release(S.index_buf);
	for (int i = 0; i < MAX_SHADER_COUNT; ++i) {
		int shader = S.shader[i];
		sl_shader_release_vertex_buffer(S.vertex_buf_id[i]);
		sl_buf_release(S.vertex_buf[i]);
		sl_shader_unload(shader);
	}
}

void 
sl_model3_bind() {
	if (S.curr_shader >= 0) {
		sl_shader_bind(S.shader[S.curr_shader]);
	}
}

void 
sl_model3_unbind() {
	sl_model3_commit();
}

void 
sl_model3_projection(float aspect) {
	sm_mat4_perspective(&S.projection_mat, -aspect, aspect, -1, 1, 1, 9999);
	for (int i = 0; i < MAX_SHADER_COUNT; ++i) {
		sl_shader_set_uniform(S.shader[i], S.projection_id[i], UNIFORM_FLOAT44, S.projection_mat.x);
	}
}

void 
sl_model3_modelview(const union sm_mat4* mat) {
	memcpy(&S.modelview_mat, mat, sizeof(union sm_mat4));
	for (int i = 0; i < MAX_SHADER_COUNT; ++i) {
		sl_shader_set_uniform(S.shader[i], S.modelview_id[i], UNIFORM_FLOAT44, S.modelview_mat.x);
	}

	union sm_mat3 mat3;
	sm_mat4_to_mat3(&mat3, &S.modelview_mat);
	sl_shader_set_uniform(S.shader[IDX_NORMAL], S.u_normal.normal_matrix, UNIFORM_FLOAT33, mat3.x);
 	sl_shader_set_uniform(S.shader[IDX_BOTH], S.u_both.normal_matrix, UNIFORM_FLOAT33, mat3.x);
}

void 
sl_model3_set_material(const struct sm_vec3* ambient, const struct sm_vec3* diffuse, 
					   const struct sm_vec3* specular, float shininess, int tex) {
	_uniform_light_set_material(S.shader[IDX_NORMAL], &S.u_normal, ambient, diffuse, specular, shininess);
	_uniform_light_set_material(S.shader[IDX_BOTH], &S.u_both, ambient, diffuse, specular, shininess);
	if (tex >= 0) {
		render_setdepth(sl_shader_get_render(), DEPTH_LESS_EQUAL);
		sl_shader_set_texture(tex, 0);
	}
}

void 
sl_model3_set_light_position(const struct sm_vec3* pos) {
	sl_shader_set_uniform(S.shader[IDX_NORMAL], S.u_normal.light_position, UNIFORM_FLOAT3, &pos->x);
	sl_shader_set_uniform(S.shader[IDX_BOTH], S.u_both.light_position, UNIFORM_FLOAT3, &pos->x);	
}

void 
sl_model3_draw(const struct ds_array* vertices, const struct ds_array* indices, 
			   bool has_normal, bool has_texcoord) {
	int shader = 0;
	if (!has_normal && !has_texcoord) shader = IDX_PLAIN;
	if ( has_normal && !has_texcoord) shader = IDX_NORMAL;
	if (!has_normal &&  has_texcoord) shader = IDX_TEXTURE;
	if ( has_normal &&  has_texcoord) shader = IDX_BOTH;
	if (shader != S.curr_shader) {
		sl_model3_commit();
		S.curr_shader = shader;
		sl_shader_bind(S.shader[S.curr_shader]);
	}

	int vn = ds_array_size(vertices),
		in = ds_array_size(indices);
	if (S.vertex_buf[shader]->n + vn > S.vertex_buf[shader]->cap ||
		S.index_buf->n + in > S.index_buf->cap) {
		sl_model3_commit();
	}
	if (vn > S.vertex_buf[shader]->cap || in > S.index_buf->cap) {
		return;
	}
	
	int ioffset = S.vertex_buf[shader]->n;
	int isz = ds_array_size(indices);
	uint16_t ibuf[isz];
	memcpy(ibuf, ds_array_data(indices), sizeof(ibuf));
	for (int i = 0; i < isz; ++i) {
		ibuf[i] += ioffset;
	}
	sl_buf_add(S.index_buf, ibuf, isz);

	sl_buf_add(S.vertex_buf[shader], ds_array_data(vertices), ds_array_size(vertices));
}

void 
sl_model3_commit() {
	if (S.index_buf->n == 0 || S.curr_shader < 0) {
		return;
	}
	sl_shader_apply_uniform(S.shader[S.curr_shader]);
	struct render* r = sl_shader_get_render();
	render_setdepth(r, DEPTH_LESS_EQUAL);
	sl_shader_flush();
	render_setdepth(r, DEPTH_DISABLE);
}