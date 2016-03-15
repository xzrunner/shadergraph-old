#include "sl_model.h"
#include "sl_shader.h"
#include "sl_matrix.h"

#include <gl/glew.h>

#include <string.h>

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

	int projection_idx, modeview_idx;
	struct sl_matrix modelview_mat, projection_mat;

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

	sl_shader_create_vertex_buffer(s, MAX_VERTICES, sizeof(struct vertex));

	struct vertex_attrib va[2] = {
		{ "position", 0, 3, sizeof(float), BUFFER_OFFSET(vx) },
		{ "texcoord", 0, 2, sizeof(float), BUFFER_OFFSET(tx) },
	};
	sl_shader_create_vertex_layout(s, sizeof(va)/sizeof(va[0]), va);

	sl_shader_load(s, model_simple_vert, model_simple_frag);

	sl_shader_set_draw_mode(s, DRAW_TRIANGLES);

	S.shader = s;

	S.projection_idx = sl_shader_add_uniform(s, "u_projection", UNIFORM_FLOAT44);
	S.modeview_idx = sl_shader_add_uniform(s, "u_modelview", UNIFORM_FLOAT44);
	sl_matrix_identity(&S.projection_mat);
	sl_matrix_identity(&S.modelview_mat);

	S.color = 0xffffffff;
	S.tex = 0;
}

void 
sl_model_unload() {
	sl_shader_release_vertex_buffer(S.shader);
	sl_shader_unload(S.shader);

	memset(&S, 0, sizeof(struct shader_state));
}

void 
sl_model_bind() {
	sl_shader_bind(S.shader);
	glEnable(GL_DEPTH_TEST);

 	 	glClearColor(0.5f, 0.5f, 0.5f, 1);
 	 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void 
sl_model_unbind() {
	sl_model_commit();
	glDisable(GL_DEPTH_TEST);
}

void 
sl_model_projection(int width, int height) {
// 	float hh = 1.0f * height / width;
// 	sl_matrix_perspective(&S.projection_mat, -1, 1, -hh, hh, 1, 3);

	float hw = width * 0.5f;
	float hh = height * 0.5f;
 	sl_matrix_perspective(&S.projection_mat, -hw, hw, -hh, hh, 2, -2);

//	sl_matrix_ortho(&S.projection_mat, -hw, hw, -hh, hh, 1, -1);

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
	if (texid != S.tex && S.tex != 0) {
		sl_model_commit();
	}
	S.tex = texid;

	struct vertex vb[vertices_count];
	for (int i = 0; i < vertices_count; ++i) {
		struct vertex* v = &vb[i];
		v->vx = positions[i * 3];
		v->vy = positions[i * 3 + 1];
		v->vz = positions[i * 3 + 2];
		v->tx = texcoords[i * 2];
		v->ty = texcoords[i * 2 + 1];
	}
	sl_shader_draw(S.shader, vb, vertices_count, 0);
}

void 
sl_model_commit() {
	sl_shader_set_texture(S.tex, 0);
	sl_shader_flush();
}