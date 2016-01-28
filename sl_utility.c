#include "sl_utility.h"

void 
sl_init_quad_index_buffer(uint16_t* buf, int quad_count) {
	for (int i = 0; i < quad_count; ++i) {
		buf[i*6] = i*4;
		buf[i*6+1] = i*4+1;
		buf[i*6+2] = i*4+2;
		buf[i*6+3] = i*4;
		buf[i*6+4] = i*4+2;
		buf[i*6+5] = i*4+3;
	}
}