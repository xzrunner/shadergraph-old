#include "sl_model.h"
#include "sl_shader.h"
#include "sl_matrix.h"
#include "sl_vertexbuffer.h"

#include <string.h>
#include <stdlib.h>

#define STRINGIFY(A)  #A
#include "model_simple.vert"
#include "model_simple.frag"

#define MAX_VERTICES		4096

#define BUFFER_OFFSET(f) ((intptr_t)&(((struct vertex *)NULL)->f))

struct vertex {
	float vx, vy, vz;
	float tx, ty;
};

struct shader_state {
	int shader;

	int vertex_buf_id;
	struct sl_vertexbuffer* vertex_buf;

	int projection_idx, modeview_idx;
	struct sl_matrix modelview_mat, projection_mat;

	struct vertex* buf;
	int vertices_sz;

	int color;
	int tex;
};

static struct shader_state S;

void 
sl_model_load() {
	int s = sl_shader_create();
	if (s < 0) {
		return;
	}

	int vertex_buf_id = sl_shader_create_vertex_buffer(MAX_VERTICES, sizeof(struct vertex));
	struct sl_vertexbuffer* vertex_buf = sl_vb_create(sizeof(struct vertex), MAX_VERTICES);
	sl_shader_set_vertex_buffer(s, vertex_buf_id, vertex_buf);

	struct vertex_attrib va[2] = {
		{ "position", 0, 3, sizeof(float), BUFFER_OFFSET(vx) },
		{ "texcoord", 0, 2, sizeof(float), BUFFER_OFFSET(tx) },
	};
	int layout_id = sl_shader_create_vertex_layout(sizeof(va)/sizeof(va[0]), va);
	sl_shader_set_vertex_layout(s, layout_id);

	sl_shader_load(s, model_simple_vert, model_simple_frag);

	sl_shader_set_draw_mode(s, DRAW_TRIANGLES);

	S.shader = s;

	S.vertex_buf_id = vertex_buf_id;
	S.vertex_buf = vertex_buf;

	S.projection_idx = sl_shader_add_uniform(s, "u_projection", UNIFORM_FLOAT44);
	S.modeview_idx = sl_shader_add_uniform(s, "u_modelview", UNIFORM_FLOAT44);
	sl_matrix_identity(&S.projection_mat);
	sl_matrix_identity(&S.modelview_mat);

	S.buf = (struct vertex*)malloc(sizeof(struct vertex) * MAX_VERTICES);
	S.vertices_sz = 0;

	S.color = 0xffffffff;
	S.tex = 0;
}

void 
sl_model_unload() {
	sl_shader_release_vertex_buffer(S.vertex_buf_id);
	sl_vb_release(S.vertex_buf);
	sl_shader_unload(S.shader);

	free(S.buf); S.buf = NULL;

	memset(&S, 0, sizeof(struct shader_state));
}

void 
sl_model_bind() {
	sl_shader_bind(S.shader);
}

void 
sl_model_unbind() {
	sl_model_commit();
}

void 
sl_model_projection(int width, int height, float near, float far) {
	float hw = width * 0.5f;
	float hh = height * 0.5f;
 	sl_matrix_perspective(&S.projection_mat, -hw, hw, -hh, hh, near, far);
	sl_shader_set_uniform(S.shader, S.projection_idx, UNIFORM_FLOAT44, S.projection_mat.e);
}

void 
sl_model_modelview(float x, float y, float sx, float sy) {
	sl_matrix_set_scale(&S.modelview_mat, sx, sy);
	sl_matrix_set_translate(&S.modelview_mat, x * sx, y * sy);
	sl_shader_set_uniform(S.shader, S.modeview_idx, UNIFORM_FLOAT44, S.modelview_mat.e);
}

void 
sl_model_draw(const float* positions, const float* texcoords, int texid, int vertices_count) {
	if (S.vertices_sz + vertices_count > MAX_VERTICES ||
		(texid != S.tex && S.tex != 0)) {
		sl_model_commit();
	}
	S.tex = texid;

	for (int i = 0; i < vertices_count; ++i) {
		struct vertex* v = &S.buf[S.vertices_sz++];
		v->vx = positions[i * 3];
		v->vy = positions[i * 3 + 1];
		v->vz = positions[i * 3 + 2];
		v->tx = texcoords[i * 2];
		v->ty = texcoords[i * 2 + 1];
	}
}

void 
sl_model_commit() {
	if (S.vertices_sz == 0) {
		return;
	}

	sl_shader_set_texture(S.tex, 0);

	sl_shader_draw(S.shader, S.buf, S.vertices_sz, 0);
	S.vertices_sz = 0;

	sl_shader_flush();
}