#ifndef HF_MAT_H
#define HF_MAT_H

typedef float hf_mat1x2f[1][2];
typedef float hf_mat1x3f[1][3];
typedef float hf_mat1x4f[1][4];
typedef float hf_mat2x1f[2][1];
typedef float hf_mat2f[2][2];
typedef float hf_mat2x3f[2][3];
typedef float hf_mat2x4f[2][4];
typedef float hf_mat3x1f[3][1];
typedef float hf_mat3x2f[3][2];
typedef float hf_mat3f[3][3];
typedef float hf_mat3x4f[3][4];
typedef float hf_mat4x1f[4][1];
typedef float hf_mat4x2f[4][2];
typedef float hf_mat4x3f[4][3];
typedef float hf_mat4f[4][4];

void hf_mat1x2f_copy(hf_mat1x2f mat, hf_mat1x2f out);
void hf_mat1x2f_transpose(hf_mat1x2f mat, hf_mat2x1f out);
void hf_mat1x2f_add(hf_mat1x2f a, hf_mat1x2f b, hf_mat1x2f out);
void hf_mat1x2f_multiply(hf_mat1x2f mat, float scalar, hf_mat1x2f out);
void hf_mat1x2f_multiply_mat2f(hf_mat1x2f a, hf_mat2f b, hf_mat1x2f out);
void hf_mat1x2f_multiply_mat2x3f(hf_mat1x2f a, hf_mat2x3f b, hf_mat1x3f out);
void hf_mat1x2f_multiply_mat2x4f(hf_mat1x2f a, hf_mat2x4f b, hf_mat1x4f out);

void hf_mat1x3f_copy(hf_mat1x3f mat, hf_mat1x3f out);
void hf_mat1x3f_transpose(hf_mat1x3f mat, hf_mat3x1f out);
void hf_mat1x3f_add(hf_mat1x3f a, hf_mat1x3f b, hf_mat1x3f out);
void hf_mat1x3f_multiply(hf_mat1x3f mat, float scalar, hf_mat1x3f out);
void hf_mat1x3f_multiply_mat3x2f(hf_mat1x3f a, hf_mat3x2f b, hf_mat1x2f out);
void hf_mat1x3f_multiply_mat3f(hf_mat1x3f a, hf_mat3f b, hf_mat1x3f out);
void hf_mat1x3f_multiply_mat3x4f(hf_mat1x3f a, hf_mat3x4f b, hf_mat1x4f out);

void hf_mat1x4f_copy(hf_mat1x4f mat, hf_mat1x4f out);
void hf_mat1x4f_transpose(hf_mat1x4f mat, hf_mat4x1f out);
void hf_mat1x4f_add(hf_mat1x4f a, hf_mat1x4f b, hf_mat1x4f out);
void hf_mat1x4f_multiply(hf_mat1x4f mat, float scalar, hf_mat1x4f out);
void hf_mat1x4f_multiply_mat4x2f(hf_mat1x4f a, hf_mat4x2f b, hf_mat1x2f out);
void hf_mat1x4f_multiply_mat4x3f(hf_mat1x4f a, hf_mat4x3f b, hf_mat1x3f out);
void hf_mat1x4f_multiply_mat4f(hf_mat1x4f a, hf_mat4f b, hf_mat1x4f out);

void hf_mat2x1f_copy(hf_mat2x1f mat, hf_mat2x1f out);
void hf_mat2x1f_transpose(hf_mat2x1f mat, hf_mat1x2f out);
void hf_mat2x1f_add(hf_mat2x1f a, hf_mat2x1f b, hf_mat2x1f out);
void hf_mat2x1f_multiply(hf_mat2x1f mat, float scalar, hf_mat2x1f out);
void hf_mat2x1f_multiply_mat1x2f(hf_mat2x1f a, hf_mat1x2f b, hf_mat2f out);
void hf_mat2x1f_multiply_mat1x3f(hf_mat2x1f a, hf_mat1x3f b, hf_mat2x3f out);
void hf_mat2x1f_multiply_mat1x4f(hf_mat2x1f a, hf_mat1x4f b, hf_mat2x4f out);

void hf_mat2f_copy(hf_mat2f mat, hf_mat2f out);
void hf_mat2f_identity(hf_mat2f out);
void hf_mat2f_transpose(hf_mat2f mat, hf_mat2f out);
float hf_mat2f_determinant(hf_mat2f mat);
float hf_mat2f_minor(hf_mat2f mat, int i, int j);
void hf_mat2f_inverse(hf_mat2f mat, hf_mat2f out);
void hf_mat2f_add(hf_mat2f a, hf_mat2f b, hf_mat2f out);
void hf_mat2f_multiply(hf_mat2f mat, float scalar, hf_mat2f out);
void hf_mat2f_multiply_mat2x1f(hf_mat2f a, hf_mat2x1f b, hf_mat2x1f out);
void hf_mat2f_multiply_mat2f(hf_mat2f a, hf_mat2f b, hf_mat2f out);
void hf_mat2f_multiply_mat2x3f(hf_mat2f a, hf_mat2x3f b, hf_mat2x3f out);
void hf_mat2f_multiply_mat2x4f(hf_mat2f a, hf_mat2x4f b, hf_mat2x4f out);

void hf_mat2x3f_copy(hf_mat2x3f mat, hf_mat2x3f out);
void hf_mat2x3f_transpose(hf_mat2x3f mat, hf_mat3x2f out);
void hf_mat2x3f_add(hf_mat2x3f a, hf_mat2x3f b, hf_mat2x3f out);
void hf_mat2x3f_multiply(hf_mat2x3f mat, float scalar, hf_mat2x3f out);
void hf_mat2x3f_multiply_mat3x1f(hf_mat2x3f a, hf_mat3x1f b, hf_mat2x1f out);
void hf_mat2x3f_multiply_mat3x2f(hf_mat2x3f a, hf_mat3x2f b, hf_mat2f out);
void hf_mat2x3f_multiply_mat3f(hf_mat2x3f a, hf_mat3f b, hf_mat2x3f out);
void hf_mat2x3f_multiply_mat3x4f(hf_mat2x3f a, hf_mat3x4f b, hf_mat2x4f out);

void hf_mat2x4f_copy(hf_mat2x4f mat, hf_mat2x4f out);
void hf_mat2x4f_transpose(hf_mat2x4f mat, hf_mat4x2f out);
void hf_mat2x4f_add(hf_mat2x4f a, hf_mat2x4f b, hf_mat2x4f out);
void hf_mat2x4f_multiply(hf_mat2x4f mat, float scalar, hf_mat2x4f out);
void hf_mat2x4f_multiply_mat4x1f(hf_mat2x4f a, hf_mat4x1f b, hf_mat2x1f out);
void hf_mat2x4f_multiply_mat4x2f(hf_mat2x4f a, hf_mat4x2f b, hf_mat2f out);
void hf_mat2x4f_multiply_mat4x3f(hf_mat2x4f a, hf_mat4x3f b, hf_mat2x3f out);
void hf_mat2x4f_multiply_mat4f(hf_mat2x4f a, hf_mat4f b, hf_mat2x4f out);

void hf_mat3x1f_copy(hf_mat3x1f mat, hf_mat3x1f out);
void hf_mat3x1f_transpose(hf_mat3x1f mat, hf_mat1x3f out);
void hf_mat3x1f_add(hf_mat3x1f a, hf_mat3x1f b, hf_mat3x1f out);
void hf_mat3x1f_multiply(hf_mat3x1f mat, float scalar, hf_mat3x1f out);
void hf_mat3x1f_multiply_mat1x2f(hf_mat3x1f a, hf_mat1x2f b, hf_mat3x2f out);
void hf_mat3x1f_multiply_mat1x3f(hf_mat3x1f a, hf_mat1x3f b, hf_mat3f out);
void hf_mat3x1f_multiply_mat1x4f(hf_mat3x1f a, hf_mat1x4f b, hf_mat3x4f out);

void hf_mat3x2f_copy(hf_mat3x2f mat, hf_mat3x2f out);
void hf_mat3x2f_transpose(hf_mat3x2f mat, hf_mat2x3f out);
void hf_mat3x2f_add(hf_mat3x2f a, hf_mat3x2f b, hf_mat3x2f out);
void hf_mat3x2f_multiply(hf_mat3x2f mat, float scalar, hf_mat3x2f out);
void hf_mat3x2f_multiply_mat2x1f(hf_mat3x2f a, hf_mat2x1f b, hf_mat3x1f out);
void hf_mat3x2f_multiply_mat2f(hf_mat3x2f a, hf_mat2f b, hf_mat3x2f out);
void hf_mat3x2f_multiply_mat2x3f(hf_mat3x2f a, hf_mat2x3f b, hf_mat3f out);
void hf_mat3x2f_multiply_mat2x4f(hf_mat3x2f a, hf_mat2x4f b, hf_mat3x4f out);

void hf_mat3f_copy(hf_mat3f mat, hf_mat3f out);
void hf_mat3f_identity(hf_mat3f out);
void hf_mat3f_transpose(hf_mat3f mat, hf_mat3f out);
float hf_mat3f_determinant(hf_mat3f mat);
float hf_mat3f_minor(hf_mat3f mat, int i, int j);
void hf_mat3f_inverse(hf_mat3f mat, hf_mat3f out);
void hf_mat3f_add(hf_mat3f a, hf_mat3f b, hf_mat3f out);
void hf_mat3f_multiply(hf_mat3f mat, float scalar, hf_mat3f out);
void hf_mat3f_multiply_mat3x1f(hf_mat3f a, hf_mat3x1f b, hf_mat3x1f out);
void hf_mat3f_multiply_mat3x2f(hf_mat3f a, hf_mat3x2f b, hf_mat3x2f out);
void hf_mat3f_multiply_mat3f(hf_mat3f a, hf_mat3f b, hf_mat3f out);
void hf_mat3f_multiply_mat3x4f(hf_mat3f a, hf_mat3x4f b, hf_mat3x4f out);

void hf_mat3x4f_copy(hf_mat3x4f mat, hf_mat3x4f out);
void hf_mat3x4f_transpose(hf_mat3x4f mat, hf_mat4x3f out);
void hf_mat3x4f_add(hf_mat3x4f a, hf_mat3x4f b, hf_mat3x4f out);
void hf_mat3x4f_multiply(hf_mat3x4f mat, float scalar, hf_mat3x4f out);
void hf_mat3x4f_multiply_mat4x1f(hf_mat3x4f a, hf_mat4x1f b, hf_mat3x1f out);
void hf_mat3x4f_multiply_mat4x2f(hf_mat3x4f a, hf_mat4x2f b, hf_mat3x2f out);
void hf_mat3x4f_multiply_mat4x3f(hf_mat3x4f a, hf_mat4x3f b, hf_mat3f out);
void hf_mat3x4f_multiply_mat4f(hf_mat3x4f a, hf_mat4f b, hf_mat3x4f out);

void hf_mat4x1f_copy(hf_mat4x1f mat, hf_mat4x1f out);
void hf_mat4x1f_transpose(hf_mat4x1f mat, hf_mat1x4f out);
void hf_mat4x1f_add(hf_mat4x1f a, hf_mat4x1f b, hf_mat4x1f out);
void hf_mat4x1f_multiply(hf_mat4x1f mat, float scalar, hf_mat4x1f out);
void hf_mat4x1f_multiply_mat1x2f(hf_mat4x1f a, hf_mat1x2f b, hf_mat4x2f out);
void hf_mat4x1f_multiply_mat1x3f(hf_mat4x1f a, hf_mat1x3f b, hf_mat4x3f out);
void hf_mat4x1f_multiply_mat1x4f(hf_mat4x1f a, hf_mat1x4f b, hf_mat4f out);

void hf_mat4x2f_copy(hf_mat4x2f mat, hf_mat4x2f out);
void hf_mat4x2f_transpose(hf_mat4x2f mat, hf_mat2x4f out);
void hf_mat4x2f_add(hf_mat4x2f a, hf_mat4x2f b, hf_mat4x2f out);
void hf_mat4x2f_multiply(hf_mat4x2f mat, float scalar, hf_mat4x2f out);
void hf_mat4x2f_multiply_mat2x1f(hf_mat4x2f a, hf_mat2x1f b, hf_mat4x1f out);
void hf_mat4x2f_multiply_mat2f(hf_mat4x2f a, hf_mat2f b, hf_mat4x2f out);
void hf_mat4x2f_multiply_mat2x3f(hf_mat4x2f a, hf_mat2x3f b, hf_mat4x3f out);
void hf_mat4x2f_multiply_mat2x4f(hf_mat4x2f a, hf_mat2x4f b, hf_mat4f out);

void hf_mat4x3f_copy(hf_mat4x3f mat, hf_mat4x3f out);
void hf_mat4x3f_transpose(hf_mat4x3f mat, hf_mat3x4f out);
void hf_mat4x3f_add(hf_mat4x3f a, hf_mat4x3f b, hf_mat4x3f out);
void hf_mat4x3f_multiply(hf_mat4x3f mat, float scalar, hf_mat4x3f out);
void hf_mat4x3f_multiply_mat3x1f(hf_mat4x3f a, hf_mat3x1f b, hf_mat4x1f out);
void hf_mat4x3f_multiply_mat3x2f(hf_mat4x3f a, hf_mat3x2f b, hf_mat4x2f out);
void hf_mat4x3f_multiply_mat3f(hf_mat4x3f a, hf_mat3f b, hf_mat4x3f out);
void hf_mat4x3f_multiply_mat3x4f(hf_mat4x3f a, hf_mat3x4f b, hf_mat4f out);

void hf_mat4f_copy(hf_mat4f mat, hf_mat4f out);
void hf_mat4f_identity(hf_mat4f out);
void hf_mat4f_transpose(hf_mat4f mat, hf_mat4f out);
float hf_mat4f_determinant(hf_mat4f mat);
float hf_mat4f_minor(hf_mat4f mat, int i, int j);
void hf_mat4f_inverse(hf_mat4f mat, hf_mat4f out);
void hf_mat4f_add(hf_mat4f a, hf_mat4f b, hf_mat4f out);
void hf_mat4f_multiply(hf_mat4f mat, float scalar, hf_mat4f out);
void hf_mat4f_multiply_mat4x1f(hf_mat4f a, hf_mat4x1f b, hf_mat4x1f out);
void hf_mat4f_multiply_mat4x2f(hf_mat4f a, hf_mat4x2f b, hf_mat4x2f out);
void hf_mat4f_multiply_mat4x3f(hf_mat4f a, hf_mat4x3f b, hf_mat4x3f out);
void hf_mat4f_multiply_mat4f(hf_mat4f a, hf_mat4f b, hf_mat4f out);

#endif//HF_MAT_H
