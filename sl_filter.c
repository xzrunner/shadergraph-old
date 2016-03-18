#include "sl_filter.h"
#include "sl_matrix.h"
#include "sl_utility.h"
#include "sl_shader.h"
#include "sl_vertexbuffer.h"
#include "sl_typedef.h"

#include <stdint.h>
#include <stdlib.h>

#define STRINGIFY(A)  #A
#include "filter.vert"
#include "edge_detect.frag"
#include "relief.frag"
#include "outline.frag"
#include "blur.frag"

#define MAX_COMMBINE 256

struct vertex {
	float vx, vy;
	float tx, ty;
};

struct shader_state {
	int shader[SLFM_MAX_COUNT];

	int index_buf_id, vertex_buf_id;
	struct sl_vertexbuffer* vertex_buf;

	int projection[SLFM_MAX_COUNT];
	int modelview[SLFM_MAX_COUNT];

	struct sl_matrix modelview_mat, projection_mat;

	struct vertex* buf;
	int quad_sz;

	int tex;
	enum SL_FILTER_MODE mode;

	int edge_detect_val;
	int blur_val;
};

static struct shader_state S;

static void
_create_shader(int idx, const char* vs, const char* fs, int index_buf_id, 
			   int vertex_buf_id, struct sl_vertexbuffer* vertex_buf, int layout_id) {
	int s = sl_shader_create();
	if (s < 0) {
		return;
	}

	sl_shader_set_index_buffer(s, index_buf_id);
	sl_shader_set_vertex_buffer(s, vertex_buf_id, vertex_buf);
	sl_shader_set_vertex_layout(s, layout_id);

	sl_shader_load(s, vs, fs);

	sl_shader_set_draw_mode(s, DRAW_TRIANGLES);

	S.shader[idx] = s;
	S.projection[idx] = sl_shader_add_uniform(s, "u_projection", UNIFORM_FLOAT44);
	S.modelview[idx] = sl_shader_add_uniform(s, "u_modelview", UNIFORM_FLOAT44);
}

void 
sl_filter_load() {
	uint16_t idxs[6 * MAX_COMMBINE];
	sl_init_quad_index_buffer(idxs, MAX_COMMBINE);
	int index_buf_id = sl_shader_create_index_buffer(6 * MAX_COMMBINE, sizeof(uint16_t), idxs);

	int vertex_buf_id = sl_shader_create_vertex_buffer(4 * MAX_COMMBINE, sizeof(struct vertex));
	struct sl_vertexbuffer* vertex_buf = sl_vb_create(sizeof(struct vertex), 4 * MAX_COMMBINE);

	struct vertex_attrib va[2] = {
		{ "position", 0, 2, sizeof(float), BUFFER_OFFSET(vertex, vx) },
		{ "texcoord", 0, 2, sizeof(float), BUFFER_OFFSET(vertex, tx) },
	};
	int layout_id = sl_shader_create_vertex_layout(sizeof(va)/sizeof(va[0]), va);

	_create_shader(SLFM_EDGE_DETECTION, filter_vert, edge_detect_frag, index_buf_id, vertex_buf_id, vertex_buf, layout_id);
	_create_shader(SLFM_RELIEF, filter_vert, relief_frag, index_buf_id, vertex_buf_id, vertex_buf, layout_id);
	_create_shader(SLFM_OUTLINE, filter_vert, outline_frag, index_buf_id, vertex_buf_id, vertex_buf, layout_id);
	_create_shader(SLFM_BLUR, filter_vert, blur_frag, index_buf_id, vertex_buf_id, vertex_buf, layout_id);

	sl_matrix_identity(&S.projection_mat);
	sl_matrix_identity(&S.modelview_mat);

	S.index_buf_id = index_buf_id;
	S.vertex_buf_id = vertex_buf_id;
	S.vertex_buf = vertex_buf;

	S.buf = (struct vertex*)malloc(sizeof(struct vertex) * MAX_COMMBINE * 4);
	S.quad_sz = 0;
	S.tex = 0;
	S.mode = SLFM_MAX_COUNT;

	S.edge_detect_val = sl_shader_add_uniform(S.shader[SLFM_EDGE_DETECTION], "u_blend", UNIFORM_FLOAT1);
	S.blur_val = sl_shader_add_uniform(S.shader[SLFM_BLUR], "u_radius", UNIFORM_FLOAT1);
	sl_filter_set_edge_detect_val(0.5f);
	sl_filter_set_blur_val(1);
}

void 
sl_filter_unload() {
	sl_shader_release_index_buffer(S.index_buf_id);
	sl_shader_release_vertex_buffer(S.vertex_buf_id);
	sl_vb_release(S.vertex_buf);
	for (int i = 0; i < SLFM_MAX_COUNT; ++i) {
		sl_shader_unload(S.shader[i]);
	}
	free(S.buf); S.buf = NULL;
}

void 
sl_filter_bind() {
	if (S.mode != SLFM_MAX_COUNT) {
		sl_shader_bind(S.shader[S.mode]);
	}
}

void 
sl_filter_unbind() {
	sl_filter_commit();
}

void 
sl_filter_projection(int width, int height) {
	float hw = width * 0.5f;
	float hh = height * 0.5f;
	sl_matrix_ortho(&S.projection_mat, -hw, hw, -hh, hh, 1, -1);
	for (int i = 0; i < SLFM_MAX_COUNT; ++i) {
		sl_shader_set_uniform(S.shader[i], S.projection[i], UNIFORM_FLOAT44, S.projection_mat.e);
	}
}

void 
sl_filter_modelview(float x, float y, float sx, float sy) {
	sl_matrix_set_scale(&S.modelview_mat, sx, sy);
	sl_matrix_set_translate(&S.modelview_mat, x * sx, y * sy);
	for (int i = 0; i < SLFM_MAX_COUNT; ++i) {
		sl_shader_set_uniform(S.shader[i], S.modelview[i], UNIFORM_FLOAT44, S.modelview_mat.e);
	}
}

void 
sl_filter_set_mode(enum SL_FILTER_MODE mode) {
	if (mode != S.mode) {
		sl_filter_commit();
		S.mode = mode;
		sl_shader_bind(S.shader[S.mode]);
	}
}

void 
sl_filter_set_edge_detect_val(float val) {
	sl_shader_set_uniform(S.shader[SLFM_EDGE_DETECTION], S.edge_detect_val, UNIFORM_FLOAT1, &val);
}

void 
sl_filter_set_blur_val(float val) {
	sl_shader_set_uniform(S.shader[SLFM_BLUR], S.blur_val, UNIFORM_FLOAT1, &val);
}

void 
sl_filter_draw(const float* positions, const float* texcoords, int texid) {
	if (S.quad_sz >= MAX_COMMBINE || (texid != S.tex && S.tex != 0)) {
		sl_filter_commit();
	}
	S.tex = texid;

	for (int i = 0; i < 4; ++i) {
		struct vertex* v = &S.buf[S.quad_sz * 4 + i];
		v->vx = positions[i * 2];
		v->vy = positions[i * 2 + 1];
		v->tx = texcoords[i * 2];
		v->ty = texcoords[i * 2 + 1];
	}
	++S.quad_sz;
}

void 
sl_filter_commit() {
	if (S.quad_sz == 0) {
		return;
	}

	sl_shader_set_texture(S.tex, 0);

	sl_shader_draw(S.shader[S.mode], S.buf, S.quad_sz * 4, S.quad_sz * 6);

	S.quad_sz = 0;
	S.tex = 0;

	sl_shader_flush();
}