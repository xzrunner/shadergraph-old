#ifdef __cplusplus
extern "C"
{
#endif

#ifndef shader_lab_math_h
#define shader_lab_math_h

struct sl_vec3 {
	float x, y, z;
};

union sl_mat3 {
	float c[3][3];
	float x[9];
};

union sl_mat4 {
	/**
	* | e0 e4 e8  e12 |
	* | e1 e5 e9  e13 |
	* | e2 e6 e10 e14 |
	* | e3 e7 e11 e15 |
	**/
	float c[4][4];
	float x[16];
};

void sl_mat4_identity(union sl_mat4* mat);

void sl_mat4_set_scale(union sl_mat4* mat, float sx, float sy);
void sl_mat4_set_translate(union sl_mat4* mat, float x, float y);
void sl_mat4_set_translate3(union sl_mat4* mat, float x, float y, float z);
void sl_mat4_rotate_x(union sl_mat4* mat, float degrees);

union sl_mat4* sl_mat4_mul(union sl_mat4* m, union sl_mat4* m1, union sl_mat4* m2);

void sl_mat4_ortho(union sl_mat4* mat, float left, float right, float bottom, float top, float near, float far);
void sl_mat4_perspective(union sl_mat4* mat, float left, float right, float bottom, float top, float near, float far);

void sl_mat4_to_mat3(const union sl_mat4* mat4, union sl_mat3* mat3);

#endif // shader_lab_math_h

#ifdef __cplusplus
}
#endif