#include "sl_blend.h"
#include "sl_matrix.h"
#include "sl_shader.h"
#include "sl_typedef.h"
#include "sl_utility.h"

#include <render/render.h>

#include <stdlib.h>

#define STRINGIFY(A)  #A
#include "blend.vert"
#include "blend.frag"

#define MAX_COMMBINE 256

struct position {
	float vx, vy;
	float tx_blend, ty_blend;
	float tx_base, ty_base;
};

struct color {
	uint32_t color, additive;
};

struct vertex {
	struct position pos;
	struct color col;
};

struct shader_state {
	int shader;

	int projection, modelview;
	struct sl_matrix modelview_mat, projection_mat;

	uint32_t color, additive;

	struct vertex* buf;
	int quad_sz;

	int tex_blend, tex_base;

	int mode_id;
	enum SL_BLEND_MODE mode;
};

static struct shader_state S;

void 
sl_blend_load() {
	int s = sl_shader_create();
	if (s < 0) {
		return;
	}

	uint16_t idxs[6 * MAX_COMMBINE];
	sl_init_quad_index_buffer(idxs, MAX_COMMBINE);
	int idx_buf = sl_shader_create_index_buffer(6 * MAX_COMMBINE, sizeof(uint16_t), idxs);
	sl_shader_set_index_buffer(s, idx_buf);

	sl_shader_create_vertex_buffer(s, 4 * MAX_COMMBINE, sizeof(struct vertex));

	struct vertex_attrib va[5] = {
		{ "position", 0, 2, sizeof(float), BUFFER_OFFSET(vertex, pos.vx) },
		{ "texcoord", 0, 2, sizeof(float), BUFFER_OFFSET(vertex, pos.tx_blend) },
		{ "texcoord_base", 0, 2, sizeof(float), BUFFER_OFFSET(vertex, pos.tx_base) },
		{ "color", 0, 4, sizeof(uint8_t), BUFFER_OFFSET(vertex, col.color) },
		{ "additive", 0, 4, sizeof(uint8_t), BUFFER_OFFSET(vertex, col.additive) },
	};
	sl_shader_create_vertex_layout(s, sizeof(va)/sizeof(va[0]), va);

	sl_shader_load(s, blend_vert, blend_frag);

	sl_shader_set_draw_mode(s, DRAW_TRIANGLES);

	S.shader = s;

	S.projection = sl_shader_add_uniform(s, "u_projection", UNIFORM_FLOAT44);
	S.modelview = sl_shader_add_uniform(s, "u_modelview", UNIFORM_FLOAT44);
	sl_matrix_identity(&S.projection_mat);
	sl_matrix_identity(&S.modelview_mat);

	S.mode_id = sl_shader_add_uniform(s, "u_mode", UNIFORM_INT1);
	S.mode = SLBM_NORMAL;

	S.color = 0xffffffff;
	S.additive = 0x00000000;

	S.buf = (struct vertex*)malloc(sizeof(struct vertex) * MAX_COMMBINE * 4);
	S.quad_sz = 0;
	S.tex_blend = S.tex_base = 0;
}

void 
sl_blend_unload() {
	sl_shader_release_index_buffer(S.shader);
	sl_shader_release_vertex_buffer(S.shader);
	sl_shader_unload(S.shader);
	free(S.buf); S.buf = NULL;
}

void 
sl_blend_bind() {
}

void 
sl_blend_unbind() {
	sl_blend_commit();
}

void 
sl_blend_projection(int width, int height) {
	float hw = width * 0.5f;
	float hh = height * 0.5f;
	sl_matrix_ortho(&S.projection_mat, -hw, hw, -hh, hh, 1, -1);
	sl_shader_set_uniform(S.shader, S.projection, UNIFORM_FLOAT44, S.projection_mat.e);
}

void 
sl_blend_modelview(float x, float y, float sx, float sy) {
	sl_matrix_set_scale(&S.modelview_mat, sx, sy);
	sl_matrix_set_translate(&S.modelview_mat, x * sx, y * sy);
	sl_shader_set_uniform(S.shader, S.modelview, UNIFORM_FLOAT44, S.modelview_mat.e);
}

void 
sl_blend_set_color(uint32_t color, uint32_t additive) {
	S.color = color;
	S.additive = additive;
}

void 
sl_blend_set_mode(enum SL_BLEND_MODE mode) {
	if (mode != S.mode) {
		sl_blend_commit();
	}
	S.mode = mode;
	sl_shader_set_uniform(S.shader, S.mode, UNIFORM_FLOAT44, S.modelview_mat.e);	
}

void 
sl_blend_set_base(int texid) {
	sl_shader_set_texture(texid, 1);
}

void 
sl_blend_draw(const float* positions, const float* texcoords_blend, 
			  const float* texcoords_base, int tex_blend, int tex_base) {
	if (S.quad_sz >= MAX_COMMBINE) {
		sl_blend_commit();
		return;
	}
	if ((tex_blend != S.tex_blend && S.tex_blend != 0) ||
		(tex_base != S.tex_base && S.tex_base != 0)) {
		sl_blend_commit();
	}
	S.tex_blend = tex_blend;
	S.tex_base = tex_base;

	for (int i = 0; i < 4; ++i) {
		struct vertex* v = &S.buf[S.quad_sz * 4 + i];
		v->pos.vx = positions[i * 2];
		v->pos.vy = positions[i * 2 + 1];
		v->pos.tx_blend = texcoords_blend[i * 2];
		v->pos.ty_blend = texcoords_blend[i * 2 + 1];
		v->pos.tx_base = texcoords_base[i * 2];
		v->pos.ty_base = texcoords_base[i * 2 + 1];
		v->col.color = S.color;
		v->col.additive = S.additive;
	}
}

void 
sl_blend_commit() {
	if (S.quad_sz == 0) {
		return;
	}
	
	sl_shader_set_texture(S.tex_blend, 0);
	sl_shader_set_texture(S.tex_base, 1);

	sl_shader_bind(S.shader);
	sl_shader_draw(S.shader, S.buf, S.quad_sz * 4, S.quad_sz * 6);

	S.quad_sz = 0;
	S.tex_blend = S.tex_base = 0;
}