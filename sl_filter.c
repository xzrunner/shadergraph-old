#include "sl_filter.h"
#include "sl_matrix.h"
#include "sl_utility.h"
#include "sl_shader.h"

#include <stdint.h>

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
	int projection[SLFM_MAX_COUNT];
	int modelview[SLFM_MAX_COUNT];

	struct sl_matrix modelview_mat, projection_mat;

	struct vertex* buf;
	int quad_sz;

	int tex;
	enum SL_FILTER_MODE mode;
};

static struct shader_state S;

void 
sl_filter_load() {
	uint16_t idxs[6 * MAX_COMMBINE];
	sl_init_quad_index_buffer(idxs, MAX_COMMBINE);
	int idx_buf = sl_shader_create_index_buffer(6 * MAX_COMMBINE, sizeof(uint16_t), idxs);

	int s = sl_shader_create();
	if (s < 0) {
		return;
	}
	sl_shader_set_index_buffer();

	S.shader[SLFM_EDGE_DETECTION] = s;
	
}

void 
sl_filter_unload() {

}

void 
sl_filter_bind() {

}

void 
sl_filter_unbind() {

}

void 
sl_filter_projection(int width, int height) {

}

void 
sl_filter_modelview(float x, float y, float sx, float sy) {

}

void 
sl_filter_set_mode(enum SL_BLEND_MODE mode) {

}

void 
sl_filter_draw(const float* positions, const float* texcoords, int texid) {

}

void 
sl_filter_commit() {

}