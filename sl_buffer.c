#include "sl_buffer.h"

#include <stdlib.h>
#include <string.h>

struct sl_buffer* 
sl_buf_create(int size, int cap) {
	int sz = sizeof(struct sl_buffer) + size * cap;
	struct sl_buffer* vb = (struct sl_buffer*)malloc(sz);
	memset(vb, 0, sz);
	vb->size = size;
	vb->n = 0;
	vb->cap = cap;
	vb->buf = (char*)(vb + 1);
	vb->dirty = false;
	return vb;
}

void 
sl_buf_release(struct sl_buffer* vb) {
	free(vb);
}

inline bool 
sl_buf_add(struct sl_buffer* vb, const void* data, int n) {
	if (vb->n + n > vb->cap) {
		return true;
	} else {
		if (data) {
			memcpy(vb->buf + vb->size * vb->n, data, vb->size * n);
		}
		vb->dirty = true;
		vb->n += n;
		return false;
	}
}