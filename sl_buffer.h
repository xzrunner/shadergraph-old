#ifdef __cplusplus
extern "C"
{
#endif

#ifndef shader_lab_buffer_h
#define shader_lab_buffer_h

#include <stdbool.h>

struct sl_buffer {
	char* buf;

	int size;
	int n;
	int cap;

	bool dirty;
};

struct sl_buffer* sl_buf_create(int size, int cap);
void sl_buf_release(struct sl_buffer*);

extern inline bool 
sl_buf_add(struct sl_buffer*, void* data, int n);

#endif // shader_lab_buffer_h

#ifdef __cplusplus
}
#endif