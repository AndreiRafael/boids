#ifndef HF_TRANSFORM_H
#define HF_TRANSFORM_H

#include "hf_vec.h"
#include "hf_mat.h"

void hf_transform2f_translation(hf_vec2f vec, hf_mat3f out);//create a translation matrix
void hf_transform2f_rotation(float rad, hf_mat3f out);//create a rotation matrix
void hf_transform2f_rotation_cached(float sin_rad, float cos_rad, hf_mat3f out);//create a rotation matrix using pre-calculated sine and cosine
void hf_transform2f_scale(hf_vec2f vec, hf_mat3f out);//create a scale matrix
void hf_transform2f_apply(hf_vec2f vec, hf_mat3f mat, hf_vec2f out);//returns vec transformed by the matrix mat

void hf_transform3f_translation(hf_vec3f vec, hf_mat4f out);//create a translation matrix
void hf_transform3f_rotation(hf_vec3f vec, float rad, hf_mat4f out);//create a rotation matrix on an arbitrary axis
void hf_transform3f_rotation_cached(hf_vec3f vec, float sin_rad, float cos_rad, hf_mat4f out);//create a rotation matrix on an arbitrary axis using pre-calculated sine and cosine
void hf_transform3f_rotation_x(float rad, hf_mat4f out);//create a rotation matrix on the x axis
void hf_transform3f_rotation_x_cached(float sin_rad, float cos_rad, hf_mat4f out);//create a rotation matrix on the x axis using pre-calculated sine and cosine
void hf_transform3f_rotation_y(float rad, hf_mat4f out);//create a rotation matrix on the y axis
void hf_transform3f_rotation_y_cached(float sin_rad, float cos_rad, hf_mat4f out);//create a rotation matrix on the y axis using pre-calculated sine and cosine
void hf_transform3f_rotation_z(float rad, hf_mat4f out);//create a rotation matrix on the z axis
void hf_transform3f_rotation_z_cached(float sin_rad, float cos_rad, hf_mat4f out);//create a rotation matrix on the z axis using pre-calculated sine and cosine
void hf_transform3f_scale(hf_vec3f vec, hf_mat4f out);//create a scale matrix
void hf_transform3f_apply(hf_vec3f vec, hf_mat4f mat, hf_vec3f out);//returns vec transformed by the matrix mat

//TODO: view and perspective
void hf_transform3f_projection_orthographic_size(float w, float h, float near, float far, hf_mat4f out);
void hf_transform3f_projection_perspective_size(float w, float h, float near, float far, hf_mat4f out);
void hf_transform3f_view(hf_vec3f position, hf_vec3f forward, hf_vec3f up, hf_mat4f out);

#endif//HF_TRANSFORM_H
