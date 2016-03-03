#include "sl_shader.h"
#include "sl_vertexbuffer.h"

#include <render/render.h>
#include <render/blendmode.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#ifdef _DEBUG
#include <stdio.h>
#endif // _DEBUG

#define MAX_SHADER 16

#define MAX_UNIFORM 16
#define MAX_TEXTURE_CHANNEL 8

//#define DC_LOG

struct uniform {
	int loc;
	int offset;
	enum UNIFORM_FORMAT type;
};

struct shader {
	RID prog;

	int texture_number;

	// uniform
	int uniform_number;
	struct uniform uniform[MAX_UNIFORM];
	bool reset_uniform;
	bool uniform_change[MAX_UNIFORM];
	float uniform_value[MAX_UNIFORM * 16];

	RID vertex_buffer;
	RID index_buffer;
	RID layout;

	enum DRAW_MODE draw_mode;
	struct sl_vertexbuffer* vb;

	bool element;
};

struct shader_mgr {
	struct render* R;

	int curr_shader;
	int shader_size;
	struct shader shader[MAX_SHADER];

 	int tex[MAX_TEXTURE_CHANNEL];
 	int blendchange;
	RID target;

// 	RID layout;
// 	struct render_buffer vb;
};

static struct shader_mgr* S = NULL;

void 
sl_shader_mgr_create() {
	if (S) return;

	struct shader_mgr* sm = (struct shader_mgr*)malloc(sizeof(*sm));
	memset(sm, 0 , sizeof(*sm));

	struct render_init_args RA;
	// todo: config these args
	RA.max_buffer = 128;
	RA.max_layout = 8;
	RA.max_target = 128;
	RA.max_texture = 256;
	RA.max_shader = MAX_SHADER;

	sm->target = render_query_target();

	int smz = render_size(&RA);
	sm->R = (struct render*)malloc(smz);
	sm->R = render_init(&RA, sm->R, smz);

	sm->curr_shader = -1;
	sm->shader_size = 0;
	sm->blendchange = 0;
	render_setblend(sm->R, BLEND_ONE, BLEND_ONE_MINUS_SRC_ALPHA);

	S = sm;
}

void 
sl_shader_mgr_release() {
	if (S == NULL) {
		return;
	}
	render_exit(S->R);
	free(S->R);
	free(S);
	S = NULL;
}

struct render* 
sl_shader_get_render() {
	return S->R;
}

int 
sl_shader_create() {
	if (S->shader_size >= MAX_SHADER) {
		return -1;
	} else {
		int id = S->shader_size++;
		struct shader* s = &S->shader[id];
		struct render* R = S->R;
		if (s->prog) {
			render_release(R, SHADER, s->prog);
			s->prog = 0;
		}
		memset(s, 0, sizeof(*s));
		return id;
	}
}

//void 
//sl_shader_reset() {
//	struct render* R = S->R; 
//	render_state_reset(R);
//	render_setblend(R, BLEND_ONE, BLEND_ONE_MINUS_SRC_ALPHA);
//	if (RS->current_program != -1) {
//		render_shader_bind(rs->R, RS->program[RS->current_program].prog);
//	}
//	render_set(R, VERTEXLAYOUT, rs->layout, 0);
//	render_set(R, TEXTURE, RS->tex[0], 0);
//	render_set(R, INDEXBUFFER, RS->index_buffer,0);
//	render_set(R, VERTEXBUFFER, RS->vertex_buffer,0);
//}

void 
sl_shader_load(int id, const char* vs, const char* fs) {
	struct shader* s = &S->shader[id];
	struct render* R = S->R;
	struct shader_init_args args;
	args.vs = vs;
	args.fs = fs;
// 	args.texture = texture;
// 	args.texture_uniform = texture_uniform_name;
	s->prog = render_shader_create(R, &args);
	render_shader_bind(R, s->prog);
	render_shader_bind(R, 0);
//	p->texture_number = texture;
	S->curr_shader = -1;
}

void 
sl_shader_unload(int id) {
	render_release(S->R, SHADER, id);
}

static inline void
_commit(struct shader* s) {
	struct sl_vertexbuffer* vb = s->vb;
	if (!vb || vb->n == 0) {
		return;
	}

#ifdef _DEBUG
	printf("_commit %d \n", vb->n);
#endif // _DEBUG

	render_buffer_update(S->R, s->vertex_buffer, vb->buf, vb->n);
	if (s->element) {
		render_draw_elements(S->R, s->draw_mode, 0, vb->element);
	} else {
		render_draw_arrays(S->R, s->draw_mode, 0, vb->n);
	}	
	vb->n = 0;
	vb->element = 0;
}

void 
sl_shader_blend(int m1, int m2) {
	if (m1 != BLEND_GL_ONE || m2 != BLEND_GL_ONE_MINUS_SRC_ALPHA) {
		_commit(&S->shader[S->curr_shader]);
#ifdef DC_LOG
		printf("shader_blend\n");
#endif // DC_LOG
		S->blendchange = 1;
		enum BLEND_FORMAT src = blend_mode(m1);
		enum BLEND_FORMAT dst = blend_mode(m2);
		render_setblend(S->R, src, dst);
	}
}

void 
sl_shader_defaultblend() {
	if (S->blendchange) {
		_commit(&S->shader[S->curr_shader]);
#ifdef DC_LOG
		printf("shader_defaultblend\n");
#endif // DC_LOG
		S->blendchange = 0;
		render_setblend(S->R, BLEND_ONE, BLEND_ONE_MINUS_SRC_ALPHA);
	}
}

void 
sl_shader_set_draw_mode(int id, enum DRAW_MODE dm) {
	assert(id >= 0 && id < MAX_SHADER);
	struct shader* s = &S->shader[id];
	if (s->draw_mode != dm) {
		_commit(s);
		s->draw_mode = dm;
	}
}

void 
sl_shader_set_texture(int id, int channel) {
	assert(channel < MAX_TEXTURE_CHANNEL);
	if (S->tex[channel] != id) {
		sl_shader_flush();
		S->tex[channel] = id;
		render_set(S->R, TEXTURE, id, channel);
	}
}

int 
sl_shader_get_texture() {
	return S->tex[0];
}

void 
sl_shader_set_target(int id) {
//	render_set(RS->R, TARGET, id, 0);
	S->target = id;	
}

int  
sl_shader_get_target() {
//	return render_get(RS->R, TARGET, 0);
	return S->target;
}

void 
sl_shader_create_vertex_buffer(int id, int n, int stride) {
	assert(id >= 0 && id < MAX_SHADER);
	struct shader* s = &S->shader[id];

	struct render* R = S->R;
	RID vertex_buffer = render_buffer_create(R, VERTEXBUFFER, NULL, n, stride);
	render_set(R, VERTEXBUFFER, vertex_buffer, 0);

	s->vertex_buffer = vertex_buffer;
	s->vb = sl_vb_create(stride, n);
}

void 
sl_shader_release_vertex_buffer(int id) {
	assert(id >= 0 && id < MAX_SHADER);
	struct shader* s = &S->shader[id];
	render_release(S->R, VERTEXBUFFER, s->vertex_buffer);
}

int 
sl_shader_create_index_buffer(int n, int stride, const void* data) {
	return render_buffer_create(S->R, INDEXBUFFER, data, n, stride);	
}

void 
sl_shader_set_index_buffer(int id, int buf_id) {
	assert(id >= 0 && id < MAX_SHADER);
	struct shader* s = &S->shader[id];
	s->index_buffer = buf_id;
	s->element = true;
	render_set(S->R, INDEXBUFFER, buf_id, 0);
}

void 
sl_shader_release_index_buffer(int id) {
	assert(id >= 0 && id < MAX_SHADER);
	struct shader* s = &S->shader[id];
	render_release(S->R, INDEXBUFFER, s->index_buffer);
}

void 
sl_shader_create_vertex_layout(int id, int n, struct vertex_attrib* va) {
	assert(id >= 0 && id < MAX_SHADER);

	struct render* R = S->R;
	RID layout = render_register_vertexlayout(R, n, va);
	render_set(R, VERTEXLAYOUT, layout, 0);

	S->shader[id].layout = layout;
}

static void
_apply_uniform(struct shader* s) {
	struct render* R = S->R;
	for (int i = 0; i < s->uniform_number; ++i) {
		if (!s->uniform_change[i]) {
			continue;
		}
		struct uniform* u = &s->uniform[i];
		if (u->loc >= 0) {
			render_shader_setuniform(R, u->loc, u->type, s->uniform_value + u->offset);
		}
	}
	s->reset_uniform = true;
}

void 
sl_shader_bind(int id) {
	struct shader* s = &S->shader[id];
	if (S->curr_shader != -1 && (S->curr_shader != id || s->reset_uniform)) {
		_commit(&S->shader[S->curr_shader]);
	}
	if (S->curr_shader != id) {
		S->curr_shader = id;
		render_shader_bind(S->R, s->prog);
		render_set(S->R, VERTEXBUFFER, s->vertex_buffer, 0);
		_apply_uniform(s);
	} else if (s->reset_uniform) {
		_apply_uniform(s);
	}
}

int 
sl_shader_add_uniform(int id, const char* name, enum UNIFORM_FORMAT t) {
	// reset current_program
	assert(id >=0 && id < MAX_SHADER);
	sl_shader_bind(id);
	struct shader* s = &S->shader[id];
	assert(s->uniform_number < MAX_UNIFORM);
	int loc = render_shader_locuniform(S->R, name);
	int index = s->uniform_number++;
	struct uniform* u = &s->uniform[index];
	u->loc = loc;
	u->type = t;
	if (index == 0) {
		u->offset = 0;
	} else {
		struct uniform* lu = &s->uniform[index-1];
		u->offset = lu->offset + sl_shader_uniform_size(lu->type);
	}
	if (loc < 0)
		return -1;
	return index;
}

void 
sl_shader_set_uniform(int id, int index, enum UNIFORM_FORMAT t, float* v) {
	assert(id >= 0 && id < MAX_SHADER);
	struct shader* s = &S->shader[id];
	_commit(s);
	assert(index >= 0 && index < s->uniform_number);
	struct uniform* u = &s->uniform[index];
	assert(t == u->type);
	int n = sl_shader_uniform_size(t);
	memcpy(s->uniform_value + u->offset, v, n * sizeof(float));
	s->reset_uniform = true;
	s->uniform_change[index] = true;
}

int 
sl_shader_uniform_size(enum UNIFORM_FORMAT t) {
	int n = 0;
	switch(t) {
	case UNIFORM_INVALID:
		n = 0;
		break;
	case UNIFORM_FLOAT1:
		n = 1;
		break;
	case UNIFORM_FLOAT2:
		n = 2;
		break;
	case UNIFORM_FLOAT3:
		n = 3;
		break;
	case UNIFORM_FLOAT4:
		n = 4;
		break;
	case UNIFORM_FLOAT33:
		n = 9;
		break;
	case UNIFORM_FLOAT44:
		n = 16;
		break;
	}
	return n;
}

void 
sl_shader_draw(int id, void* data, int n, int element) {
	assert(id >= 0 && id < MAX_SHADER);
	struct shader* s = &S->shader[id];
	if (sl_vb_add(s->vb, data, n, element)) {
		_commit(s);
	}
}

void 
sl_shader_clear(unsigned long argb) {
	render_clear(S->R, MASKC, argb);
}

void 
sl_shader_flush() {
	if (S && S->curr_shader >= 0 && S->curr_shader < MAX_SHADER) {
		struct shader* s = &S->shader[S->curr_shader];
		_commit(s);
	}
}

int 
sl_shader_version() {
	return render_version(S->R);
}

void 
sl_shader_scissortest(int enable) {
	render_enablescissor(S->R, enable);
}