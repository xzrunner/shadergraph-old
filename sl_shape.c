#include "sl_shape.h"
#include "sl_shader.h"
#include "sl_matrix.h"
#include "sl_vertexbuffer.h"

#include <stdint.h>
#include <string.h>

#define STRINGIFY(A)  #A
#include "shape.vert"
#include "shape.frag"

#define MAX_VERTICES		4096

#define BUFFER_OFFSET(f) ((intptr_t)&(((struct vertex *)NULL)->f))

struct vertex {
	float vx, vy;
	uint32_t color;
};

struct shader_state {
	int shader;

	int vertex_buf_id;
	struct sl_vertexbuffer* vertex_buf;

	int projection_idx, modeview_idx;
	struct sl_matrix modelview_mat, projection_mat;

	int color;
};

static struct shader_state S;

void 
sl_shape_load() {
	int s = sl_shader_create();
	if (s < 0) {
		return;
	}

	int vertex_buf_id = sl_shader_create_vertex_buffer(MAX_VERTICES, sizeof(struct vertex));
	struct sl_vertexbuffer* vertex_buf = sl_vb_create(sizeof(struct vertex), MAX_VERTICES);
	sl_shader_set_vertex_buffer(s, vertex_buf_id, vertex_buf);

	struct vertex_attrib va[2] = {
		{ "position", 0, 2, sizeof(float), BUFFER_OFFSET(vx) },
		{ "color", 0, 4, sizeof(uint8_t), BUFFER_OFFSET(color) },
	};
	int layout_id = sl_shader_create_vertex_layout(sizeof(va)/sizeof(va[0]), va);
	sl_shader_set_vertex_layout(s, layout_id);

	sl_shader_load(s, shape_vert, shape_frag);

	S.shader = s;

	S.vertex_buf_id = vertex_buf_id;
	S.vertex_buf = vertex_buf;

	S.projection_idx = sl_shader_add_uniform(s, "u_projection", UNIFORM_FLOAT44);
	S.modeview_idx = sl_shader_add_uniform(s, "u_modelview", UNIFORM_FLOAT44);
	sl_matrix_identity(&S.projection_mat);
	sl_matrix_identity(&S.modelview_mat);

	S.color = 0xffffffff;
}

void 
sl_shape_unload() {
	sl_shader_release_vertex_buffer(S.vertex_buf_id);
	sl_vb_release(S.vertex_buf);
	sl_shader_unload(S.shader);

	memset(&S, 0, sizeof(struct shader_state));
}

void 
sl_shape_bind() {
	sl_shader_bind(S.shader);
}

void 
sl_shape_unbind() {
}

void 
sl_shape_projection(int width, int height) {
	float hw = width * 0.5f;
	float hh = height * 0.5f;
	sl_matrix_ortho(&S.projection_mat, -hw, hw, -hh, hh, 1, -1);
	sl_shader_set_uniform(S.shader, S.projection_idx, UNIFORM_FLOAT44, S.projection_mat.e);
}

void 
sl_shape_modelview(float x, float y, float sx, float sy) {
	sl_matrix_set_scale(&S.modelview_mat, sx, sy);
	sl_matrix_set_translate(&S.modelview_mat, x * sx, y * sy);
	sl_shader_set_uniform(S.shader, S.modeview_idx, UNIFORM_FLOAT44, S.modelview_mat.e);
}

void 
sl_shape_color(int color) {
	S.color = color;
}

void 
sl_shape_type(int type) {
	sl_shader_set_draw_mode(S.shader, type);
}

void 
sl_shape_draw(const float* positions, int count) {
	struct vertex vb[count];
	for (int i = 0; i < count; ++i) {
		struct vertex* v = &vb[i];
		v->vx = positions[i * 2];
		v->vy = positions[i * 2 + 1];
		v->color = S.color;
	}
	sl_shader_draw(S.shader, vb, count, 0);
}

void 
sl_shape_draw_node(float x, float y, bool dummy) {
	struct vertex v;
	v.vx = x;
	v.vy = y;
	v.color = dummy ? 0 : S.color;
	sl_shader_draw(S.shader, &v, 1, 0);
}

void 
sl_shape_commit() {
	sl_shader_flush();
}