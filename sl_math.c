#include "sl_math.h"

#include <string.h>
#include <math.h>

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
sl_mat4_set_translate3(union sl_mat4* mat, float x, float y, float z) {
	mat->x[12] = x;
	mat->x[13] = y;
	mat->x[14] = z;
}

void 
sl_mat4_rotate_x(union sl_mat4* mat, float degrees) {
	float radians = degrees * 3.14159f / 180.0f;
	float s = sin(radians);
	float c = cos(radians);
	mat->c[0][0] = 1; mat->c[0][1] = 0; mat->c[0][2] = 0; mat->c[0][3] = 0;
	mat->c[1][0] = 0; mat->c[1][1] = c; mat->c[1][2] = s; mat->c[1][3] = 0;
	mat->c[2][0] = 0; mat->c[2][1] =-s; mat->c[2][2] = c; mat->c[2][3] = 0;
	mat->c[3][0] = 0; mat->c[3][1] = 0; mat->c[3][2] = 0; mat->c[3][3] = 1;
}

union sl_mat4* 
sl_mat4_mul(union sl_mat4* m, union sl_mat4* m1, union sl_mat4* m2) {
	union sl_mat4 mf;
	const float *m1x = m1->x;
	const float *m2x = m2->x;

	mf.x[0] = m1x[0] * m2x[0] + m1x[4] * m2x[1] + m1x[8] * m2x[2] + m1x[12] * m2x[3];
	mf.x[1] = m1x[1] * m2x[0] + m1x[5] * m2x[1] + m1x[9] * m2x[2] + m1x[13] * m2x[3];
	mf.x[2] = m1x[2] * m2x[0] + m1x[6] * m2x[1] + m1x[10] * m2x[2] + m1x[14] * m2x[3];
	mf.x[3] = m1x[3] * m2x[0] + m1x[7] * m2x[1] + m1x[11] * m2x[2] + m1x[15] * m2x[3];

	mf.x[4] = m1x[0] * m2x[4] + m1x[4] * m2x[5] + m1x[8] * m2x[6] + m1x[12] * m2x[7];
	mf.x[5] = m1x[1] * m2x[4] + m1x[5] * m2x[5] + m1x[9] * m2x[6] + m1x[13] * m2x[7];
	mf.x[6] = m1x[2] * m2x[4] + m1x[6] * m2x[5] + m1x[10] * m2x[6] + m1x[14] * m2x[7];
	mf.x[7] = m1x[3] * m2x[4] + m1x[7] * m2x[5] + m1x[11] * m2x[6] + m1x[15] * m2x[7];

	mf.x[8] = m1x[0] * m2x[8] + m1x[4] * m2x[9] + m1x[8] * m2x[10] + m1x[12] * m2x[11];
	mf.x[9] = m1x[1] * m2x[8] + m1x[5] * m2x[9] + m1x[9] * m2x[10] + m1x[13] * m2x[11];
	mf.x[10] = m1x[2] * m2x[8] + m1x[6] * m2x[9] + m1x[10] * m2x[10] + m1x[14] * m2x[11];
	mf.x[11] = m1x[3] * m2x[8] + m1x[7] * m2x[9] + m1x[11] * m2x[10] + m1x[15] * m2x[11];

	mf.x[12] = m1x[0] * m2x[12] + m1x[4] * m2x[13] + m1x[8] * m2x[14] + m1x[12] * m2x[15];
	mf.x[13] = m1x[1] * m2x[12] + m1x[5] * m2x[13] + m1x[9] * m2x[14] + m1x[13] * m2x[15];
	mf.x[14] = m1x[2] * m2x[12] + m1x[6] * m2x[13] + m1x[10] * m2x[14] + m1x[14] * m2x[15];
	mf.x[15] = m1x[3] * m2x[12] + m1x[7] * m2x[13] + m1x[11] * m2x[14] + m1x[15] * m2x[15];

	*m = mf;

	return m;
}

void 
sl_mat4_to_mat3(const union sl_mat4* mat, union sl_mat3* mat3) {
	mat3->x[0] = mat->c[0][0]; mat3->x[1] = mat->c[0][1]; mat3->x[2] = mat->c[0][2];
	mat3->x[3] = mat->c[1][0]; mat3->x[4] = mat->c[1][1]; mat3->x[5] = mat->c[1][2];
	mat3->x[6] = mat->c[2][0]; mat3->x[7] = mat->c[2][1]; mat3->x[8] = mat->c[2][2];
}