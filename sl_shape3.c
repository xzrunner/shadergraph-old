#include "sl_shape.h"
#include "sl_shader.h"
#include "sl_buffer.h"

#include <sm.h>
#include <ds_array.h>

#include <stdint.h>
#include <string.h>

#define STRINGIFY(A)  #A
#include "shape.vert"
#include "shape.frag"

#define MAX_VERTICES		4096

#define BUFFER_OFFSET(f) ((intptr_t)&(((struct vertex *)NULL)->f))

struct vertex {
	float vx, vy, vz;
	uint32_t color;
};

struct shader_state {
	int shader;

	int vertex_buf_id;
	struct sl_buffer* vertex_buf;

	int projection_id, modelview_id;
	union sm_mat4 modelview_mat, projection_mat;

	int color;
};

static struct shader_state S;

void 
sl_shape3_load() {
	int s = sl_shader_create();
	if (s < 0) {
		return;
	}

	int vertex_buf_id = sl_shader_create_vertex_buffer(MAX_VERTICES, sizeof(struct vertex));
	struct sl_buffer* vertex_buf = sl_buf_create(sizeof(struct vertex), MAX_VERTICES);
	sl_shader_set_vertex_buffer(s, vertex_buf_id, vertex_buf);

	struct vertex_attrib va[2] = {
		{ "position", 0, 3, sizeof(float), BUFFER_OFFSET(vx) },
		{ "color", 0, 4, sizeof(uint8_t), BUFFER_OFFSET(color) },
	};
	int layout_id = sl_shader_create_vertex_layout(sizeof(va)/sizeof(va[0]), va);
	sl_shader_set_vertex_layout(s, layout_id);

	sl_shader_load(s, shape_vert, shape_frag);

	S.shader = s;

	S.vertex_buf_id = vertex_buf_id;
	S.vertex_buf = vertex_buf;

	S.projection_id = sl_shader_add_uniform(s, "u_projection", UNIFORM_FLOAT44);
	S.modelview_id = sl_shader_add_uniform(s, "u_modelview", UNIFORM_FLOAT44);
	sm_mat4_identity(&S.projection_mat);
	sm_mat4_identity(&S.modelview_mat);

	S.color = 0xffffffff;

	sl_shader_set_clear_flag(MASKC);
}

void 
sl_shape3_unload() {
	sl_shader_release_vertex_buffer(S.vertex_buf_id);
	sl_buf_release(S.vertex_buf);
	sl_shader_unload(S.shader);

	memset(&S, 0, sizeof(struct shader_state));
}

void 
sl_shape3_bind() {
	sl_shader_bind(S.shader);
}

void 
sl_shape3_unbind() {
}

void 
sl_shape3_projection(const union sm_mat4* mat) {
	memcpy(&S.projection_mat, mat, sizeof(*mat));
	sl_shader_set_uniform(S.shader, S.projection_id, UNIFORM_FLOAT44, S.projection_mat.x);
}

void 
sl_shape3_modelview(const union sm_mat4* mat) {
	memcpy(&S.modelview_mat, mat, sizeof(*mat));	
	sl_shader_set_uniform(S.shader, S.modelview_id, UNIFORM_FLOAT44, S.modelview_mat.x);
}

void 
sl_shape3_color(int color) {
	S.color = color;
}

void 
sl_shape3_type(int type) {
	sl_shader_set_draw_mode(S.shader, type);
}

void 
sl_shape3_draw(const float* positions, int count) {
	struct vertex vb[count];
	for (int i = 0; i < count; ++i) {
		struct vertex* v = &vb[i];
		v->vx = positions[i * 3];
		v->vy = positions[i * 3 + 1];
		v->vz = positions[i * 3 + 2];
		v->color = S.color;
	}
	sl_shader_draw(S.shader, vb, count, 0);
}

void 
sl_shape3_draw_node(float x, float y, float z, bool dummy) {
	struct vertex v;
	v.vx = x;
	v.vy = y;
	v.vz = z;
	v.color = dummy ? 0 : S.color;
	sl_shader_draw(S.shader, &v, 1, 0);
}

void 
sl_shape3_commit() {
	sl_shader_flush();
}