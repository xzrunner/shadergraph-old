#include "sl_math.h"

#include <string.h>

void 
sl_mat4_identity(union sl_mat4* mat) {
	memset(mat->x, 0, sizeof(float)*16);
	mat->x[0] = mat->x[5] = mat->x[10] = mat->x[15] = 1;
}

void 
sl_mat4_ortho(union sl_mat4* mat, float left, float right, float bottom, float top, float near, float far) {
	float a = 2 / (right - left);
	float b = 2 / (top - bottom);
	float c = - (right + left) / (right - left);
	float d = - (top + bottom) / (top - bottom);
	float e = - (far + near) / (far - near);
	float f = -2 / (far - near);
	mat->x[0] = a; mat->x[1] = 0; mat->x[2] = 0; mat->x[3] = 0;
	mat->x[4] = 0; mat->x[5] = b; mat->x[6] = 0; mat->x[7] = 0;
	mat->x[8] = 0; mat->x[9] = 0; mat->x[10]= f; mat->x[11]= 0;
	mat->x[12]= c; mat->x[13]= d; mat->x[14]= e; mat->x[15]= 1;
}

void 
sl_mat4_perspective(union sl_mat4* mat, float left, float right, float bottom, float top, float near, float far) {
	float a = 2 * near / (right - left);
	float b = 2 * near / (top - bottom);
	float c = (right + left) / (right - left);
	float d = (top + bottom) / (top - bottom);
	float e = -2 * near;
	mat->x[0] = a; mat->x[1] = 0; mat->x[2] = 0; mat->x[3] = 0;
	mat->x[4] = 0; mat->x[5] = b; mat->x[6] = 0; mat->x[7] = 0;
	mat->x[8] = c; mat->x[9] = d; mat->x[10]=-1; mat->x[11]=-1;
	mat->x[12]= 0; mat->x[13]= 0; mat->x[14]= e; mat->x[15]= 0;
}

void 
sl_mat4_set_scale(union sl_mat4* mat, float sx, float sy) {
	mat->x[0] = sx;
	mat->x[5] = sy;
}

void 
sl_mat4_set_translate(union sl_mat4* mat, float x, float y) {
	mat->x[12] = x;
	mat->x[13] = y;
}

void 
sl_mat4_to_mat3(const union sl_mat4* mat, union sl_mat3* mat3) {
	mat3->x[0] = mat->c[0][0]; mat3->x[1] = mat->c[0][1]; mat3->x[2] = mat->c[0][2];
	mat3->x[3] = mat->c[1][0]; mat3->x[4] = mat->c[1][1]; mat3->x[5] = mat->c[1][2];
	mat3->x[6] = mat->c[2][0]; mat3->x[7] = mat->c[2][1]; mat3->x[8] = mat->c[2][2];
}