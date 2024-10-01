#include "../include/hf_vec.h"

#include <string.h>
#include <math.h>

void hf_vec2i_copy(hf_vec2i vec, hf_vec2i out) {
	memcpy(out, vec, sizeof(out[0]) * 2);
}

void hf_vec2i_add(hf_vec2i a, hf_vec2i b, hf_vec2i out) {
	out[0] = a[0] + b[0];
	out[1] = a[1] + b[1];
}

void hf_vec2i_subtract(hf_vec2i a, hf_vec2i b, hf_vec2i out) {
	out[0] = a[0] - b[0];
	out[1] = a[1] - b[1];
}

void hf_vec2i_multiply(hf_vec2i vec, int scalar, hf_vec2i out) {
	out[0] = vec[0] * scalar;
	out[1] = vec[1] * scalar;
}

void hf_vec2i_divide(hf_vec2i vec, int scalar, hf_vec2i out) {
	out[0] = vec[0] / scalar;
	out[1] = vec[1] / scalar;
}

int hf_vec2i_square_magnitude(hf_vec2i vec) {
	return vec[0] * vec[0] + vec[1] * vec[1];
}

float hf_vec2i_magnitude(hf_vec2i vec) {
	return sqrtf((float)hf_vec2i_square_magnitude(vec));
}

int hf_vec2i_square_distance(hf_vec2i a, hf_vec2i b) {
	hf_vec2i aux;
	hf_vec2i_subtract(a, b, aux);
	return hf_vec2i_square_magnitude(aux);
}

float hf_vec2i_distance(hf_vec2i a, hf_vec2i b) {
	return sqrtf((float)hf_vec2i_square_distance(a, b));
}

int hf_vec2i_dot(hf_vec2i a, hf_vec2i b) {
	return a[0] * b[0] + a[1] * b[1];
}

void hf_vec2f_copy(hf_vec2f vec, hf_vec2f out) {
	memcpy(out, vec, sizeof(out[0]) * 2);
}

void hf_vec2f_add(hf_vec2f a, hf_vec2f b, hf_vec2f out) {
	out[0] = a[0] + b[0];
	out[1] = a[1] + b[1];
}

void hf_vec2f_subtract(hf_vec2f a, hf_vec2f b, hf_vec2f out) {
	out[0] = a[0] - b[0];
	out[1] = a[1] - b[1];
}

void hf_vec2f_multiply(hf_vec2f vec, float scalar, hf_vec2f out) {
	out[0] = vec[0] * scalar;
	out[1] = vec[1] * scalar;
}

void hf_vec2f_divide(hf_vec2f vec, float scalar, hf_vec2f out) {
	out[0] = vec[0] / scalar;
	out[1] = vec[1] / scalar;
}

void hf_vec2f_normalize(hf_vec2f vec, hf_vec2f out) {
	hf_vec2f_divide(vec, hf_vec2f_magnitude(vec), out);
}

void hf_vec2f_lerp(hf_vec2f a, hf_vec2f b, float t, hf_vec2f out) {
	out[0] = a[0] * (1.f - t) + b[0] * t;
	out[1] = a[1] * (1.f - t) + b[1] * t;
}

float hf_vec2f_square_magnitude(hf_vec2f vec) {
	return vec[0] * vec[0] + vec[1] * vec[1];
}

float hf_vec2f_magnitude(hf_vec2f vec) {
	return sqrtf(hf_vec2f_square_magnitude(vec));
}

float hf_vec2f_square_distance(hf_vec2f a, hf_vec2f b) {
	hf_vec2f aux;
	hf_vec2f_subtract(a, b, aux);
	return hf_vec2f_square_magnitude(aux);
}

float hf_vec2f_distance(hf_vec2f a, hf_vec2f b) {
	return sqrtf(hf_vec2f_square_distance(a, b));
}

float hf_vec2f_dot(hf_vec2f a, hf_vec2f b) {
	return a[0] * b[0] + a[1] * b[1];
}

void hf_vec2d_copy(hf_vec2d vec, hf_vec2d out) {
	memcpy(out, vec, sizeof(out[0]) * 2);
}

void hf_vec2d_add(hf_vec2d a, hf_vec2d b, hf_vec2d out) {
	out[0] = a[0] + b[0];
	out[1] = a[1] + b[1];
}

void hf_vec2d_subtract(hf_vec2d a, hf_vec2d b, hf_vec2d out) {
	out[0] = a[0] - b[0];
	out[1] = a[1] - b[1];
}

void hf_vec2d_multiply(hf_vec2d vec, double scalar, hf_vec2d out) {
	out[0] = vec[0] * scalar;
	out[1] = vec[1] * scalar;
}

void hf_vec2d_divide(hf_vec2d vec, double scalar, hf_vec2d out) {
	out[0] = vec[0] / scalar;
	out[1] = vec[1] / scalar;
}

void hf_vec2d_normalize(hf_vec2d vec, hf_vec2d out) {
	hf_vec2d_divide(vec, hf_vec2d_magnitude(vec), out);
}

double hf_vec2d_square_magnitude(hf_vec2d vec) {
	return vec[0] * vec[0] + vec[1] * vec[1];
}

double hf_vec2d_magnitude(hf_vec2d vec) {
	return sqrt(hf_vec2d_square_magnitude(vec));
}

double hf_vec2d_square_distance(hf_vec2d a, hf_vec2d b) {
	hf_vec2d aux;
	hf_vec2d_subtract(a, b, aux);
	return hf_vec2d_square_magnitude(aux);
}

double hf_vec2d_distance(hf_vec2d a, hf_vec2d b) {
	return sqrt(hf_vec2d_square_distance(a, b));
}

double hf_vec2d_dot(hf_vec2d a, hf_vec2d b) {
	return a[0] * b[0] + a[1] * b[1];
}

void hf_vec3i_copy(hf_vec3i vec, hf_vec3i out) {
	memcpy(out, vec, sizeof(out[0]) * 3);
}

void hf_vec3i_add(hf_vec3i a, hf_vec3i b, hf_vec3i out) {
	out[0] = a[0] + b[0];
	out[1] = a[1] + b[1];
	out[2] = a[2] + b[2];
}

void hf_vec3i_subtract(hf_vec3i a, hf_vec3i b, hf_vec3i out) {
	out[0] = a[0] - b[0];
	out[1] = a[1] - b[1];
	out[2] = a[2] - b[2];
}

void hf_vec3i_multiply(hf_vec3i vec, int scalar, hf_vec3i out) {
	out[0] = vec[0] * scalar;
	out[1] = vec[1] * scalar;
	out[2] = vec[2] * scalar;
}

void hf_vec3i_divide(hf_vec3i vec, int scalar, hf_vec3i out) {
	out[0] = vec[0] / scalar;
	out[1] = vec[1] / scalar;
	out[2] = vec[2] / scalar;
}

int hf_vec3i_square_magnitude(hf_vec3i vec) {
	return vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
}

float hf_vec3i_magnitude(hf_vec3i vec) {
	return sqrtf((float)hf_vec3i_square_magnitude(vec));
}

int hf_vec3i_square_distance(hf_vec3i a, hf_vec3i b) {
	hf_vec3i aux;
	hf_vec3i_subtract(a, b, aux);
	return hf_vec3i_square_magnitude(aux);
}

float hf_vec3i_distance(hf_vec3i a, hf_vec3i b) {
	return sqrtf((float)hf_vec3i_square_distance(a, b));
}

int hf_vec3i_dot(hf_vec3i a, hf_vec3i b) {
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void hf_vec3i_cross(hf_vec3i a, hf_vec3i b, hf_vec3i out) {
	hf_vec3i tmp;
	tmp[0] = a[1] * b[2] - a[2] * b[1];
	tmp[1] = a[2] * b[0] - a[0] * b[2];
	tmp[2] = a[0] * b[1] - a[1] * b[0];
	memcpy(out, tmp, sizeof(out[0]) * 3);
}

void hf_vec3f_copy(hf_vec3f vec, hf_vec3f out) {
	memcpy(out, vec, sizeof(out[0]) * 3);
}

void hf_vec3f_add(hf_vec3f a, hf_vec3f b, hf_vec3f out) {
	out[0] = a[0] + b[0];
	out[1] = a[1] + b[1];
	out[2] = a[2] + b[2];
}

void hf_vec3f_subtract(hf_vec3f a, hf_vec3f b, hf_vec3f out) {
	out[0] = a[0] - b[0];
	out[1] = a[1] - b[1];
	out[2] = a[2] - b[2];
}

void hf_vec3f_multiply(hf_vec3f vec, float scalar, hf_vec3f out) {
	out[0] = vec[0] * scalar;
	out[1] = vec[1] * scalar;
	out[2] = vec[2] * scalar;
}

void hf_vec3f_divide(hf_vec3f vec, float scalar, hf_vec3f out) {
	out[0] = vec[0] / scalar;
	out[1] = vec[1] / scalar;
	out[2] = vec[2] / scalar;
}

void hf_vec3f_normalize(hf_vec3f vec, hf_vec3f out) {
	hf_vec3f_divide(vec, hf_vec3f_magnitude(vec), out);
}

void hf_vec3f_lerp(hf_vec3f a, hf_vec3f b, float t, hf_vec3f out) {
	out[0] = a[0] * (1.f - t) + b[0] * t;
	out[1] = a[1] * (1.f - t) + b[1] * t;
	out[2] = a[2] * (1.f - t) + b[2] * t;
}

float hf_vec3f_square_magnitude(hf_vec3f vec) {
	return vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
}

float hf_vec3f_magnitude(hf_vec3f vec) {
	return sqrtf(hf_vec3f_square_magnitude(vec));
}

float hf_vec3f_square_distance(hf_vec3f a, hf_vec3f b) {
	hf_vec3f aux;
	hf_vec3f_subtract(a, b, aux);
	return hf_vec3f_square_magnitude(aux);
}

float hf_vec3f_distance(hf_vec3f a, hf_vec3f b) {
	return sqrtf(hf_vec3f_square_distance(a, b));
}

float hf_vec3f_dot(hf_vec3f a, hf_vec3f b) {
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void hf_vec3f_cross(hf_vec3f a, hf_vec3f b, hf_vec3f out) {
	hf_vec3f tmp;
	tmp[0] = a[1] * b[2] - a[2] * b[1];
	tmp[1] = a[2] * b[0] - a[0] * b[2];
	tmp[2] = a[0] * b[1] - a[1] * b[0];
	memcpy(out, tmp, sizeof(out[0]) * 3);
}

void hf_vec3d_copy(hf_vec3d vec, hf_vec3d out) {
	memcpy(out, vec, sizeof(out[0]) * 3);
}

void hf_vec3d_add(hf_vec3d a, hf_vec3d b, hf_vec3d out) {
	out[0] = a[0] + b[0];
	out[1] = a[1] + b[1];
	out[2] = a[2] + b[2];
}

void hf_vec3d_subtract(hf_vec3d a, hf_vec3d b, hf_vec3d out) {
	out[0] = a[0] - b[0];
	out[1] = a[1] - b[1];
	out[2] = a[2] - b[2];
}

void hf_vec3d_multiply(hf_vec3d vec, double scalar, hf_vec3d out) {
	out[0] = vec[0] * scalar;
	out[1] = vec[1] * scalar;
	out[2] = vec[2] * scalar;
}

void hf_vec3d_divide(hf_vec3d vec, double scalar, hf_vec3d out) {
	out[0] = vec[0] / scalar;
	out[1] = vec[1] / scalar;
	out[2] = vec[2] / scalar;
}

void hf_vec3d_normalize(hf_vec3d vec, hf_vec3d out) {
	hf_vec3d_divide(vec, hf_vec3d_magnitude(vec), out);
}

double hf_vec3d_square_magnitude(hf_vec3d vec) {
	return vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
}

double hf_vec3d_magnitude(hf_vec3d vec) {
	return sqrt(hf_vec3d_square_magnitude(vec));
}

double hf_vec3d_square_distance(hf_vec3d a, hf_vec3d b) {
	hf_vec3d aux;
	hf_vec3d_subtract(a, b, aux);
	return hf_vec3d_square_magnitude(aux);
}

double hf_vec3d_distance(hf_vec3d a, hf_vec3d b) {
	return sqrt(hf_vec3d_square_distance(a, b));
}

double hf_vec3d_dot(hf_vec3d a, hf_vec3d b) {
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void hf_vec3d_cross(hf_vec3d a, hf_vec3d b, hf_vec3d out) {
	hf_vec3d tmp;
	tmp[0] = a[1] * b[2] - a[2] * b[1];
	tmp[1] = a[2] * b[0] - a[0] * b[2];
	tmp[2] = a[0] * b[1] - a[1] * b[0];
	memcpy(out, tmp, sizeof(out[0]) * 3);
}

void hf_vec4i_copy(hf_vec4i vec, hf_vec4i out) {
	memcpy(out, vec, sizeof(out[0]) * 4);
}

void hf_vec4i_add(hf_vec4i a, hf_vec4i b, hf_vec4i out) {
	out[0] = a[0] + b[0];
	out[1] = a[1] + b[1];
	out[2] = a[2] + b[2];
	out[3] = a[3] + b[3];
}

void hf_vec4i_subtract(hf_vec4i a, hf_vec4i b, hf_vec4i out) {
	out[0] = a[0] - b[0];
	out[1] = a[1] - b[1];
	out[2] = a[2] - b[2];
	out[3] = a[3] - b[3];
}

void hf_vec4i_multiply(hf_vec4i vec, int scalar, hf_vec4i out) {
	out[0] = vec[0] * scalar;
	out[1] = vec[1] * scalar;
	out[2] = vec[2] * scalar;
	out[3] = vec[3] * scalar;
}

void hf_vec4i_divide(hf_vec4i vec, int scalar, hf_vec4i out) {
	out[0] = vec[0] / scalar;
	out[1] = vec[1] / scalar;
	out[2] = vec[2] / scalar;
	out[3] = vec[3] / scalar;
}

int hf_vec4i_square_magnitude(hf_vec4i vec) {
	return vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2] + vec[3] * vec[3];
}

float hf_vec4i_magnitude(hf_vec4i vec) {
	return sqrtf((float)hf_vec4i_square_magnitude(vec));
}

int hf_vec4i_square_distance(hf_vec4i a, hf_vec4i b) {
	hf_vec4i aux;
	hf_vec4i_subtract(a, b, aux);
	return hf_vec4i_square_magnitude(aux);
}

float hf_vec4i_distance(hf_vec4i a, hf_vec4i b) {
	return sqrtf((float)hf_vec4i_square_distance(a, b));
}

int hf_vec4i_dot(hf_vec4i a, hf_vec4i b) {
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

void hf_vec4f_copy(hf_vec4f vec, hf_vec4f out) {
	memcpy(out, vec, sizeof(out[0]) * 4);
}

void hf_vec4f_add(hf_vec4f a, hf_vec4f b, hf_vec4f out) {
	out[0] = a[0] + b[0];
	out[1] = a[1] + b[1];
	out[2] = a[2] + b[2];
	out[3] = a[3] + b[3];
}

void hf_vec4f_subtract(hf_vec4f a, hf_vec4f b, hf_vec4f out) {
	out[0] = a[0] - b[0];
	out[1] = a[1] - b[1];
	out[2] = a[2] - b[2];
	out[3] = a[3] - b[3];
}

void hf_vec4f_multiply(hf_vec4f vec, float scalar, hf_vec4f out) {
	out[0] = vec[0] * scalar;
	out[1] = vec[1] * scalar;
	out[2] = vec[2] * scalar;
	out[3] = vec[3] * scalar;
}

void hf_vec4f_divide(hf_vec4f vec, float scalar, hf_vec4f out) {
	out[0] = vec[0] / scalar;
	out[1] = vec[1] / scalar;
	out[2] = vec[2] / scalar;
	out[3] = vec[3] / scalar;
}

void hf_vec4f_normalize(hf_vec4f vec, hf_vec4f out) {
	hf_vec4f_divide(vec, hf_vec4f_magnitude(vec), out);
}

void hf_vec4f_lerp(hf_vec4f a, hf_vec4f b, float t, hf_vec4f out) {
	out[0] = a[0] * (1.f - t) + b[0] * t;
	out[1] = a[1] * (1.f - t) + b[1] * t;
	out[2] = a[2] * (1.f - t) + b[2] * t;
	out[3] = a[3] * (1.f - t) + b[3] * t;
}

float hf_vec4f_square_magnitude(hf_vec4f vec) {
	return vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2] + vec[3] * vec[3];
}

float hf_vec4f_magnitude(hf_vec4f vec) {
	return sqrtf(hf_vec4f_square_magnitude(vec));
}

float hf_vec4f_square_distance(hf_vec4f a, hf_vec4f b) {
	hf_vec4f aux;
	hf_vec4f_subtract(a, b, aux);
	return hf_vec4f_square_magnitude(aux);
}

float hf_vec4f_distance(hf_vec4f a, hf_vec4f b) {
	return sqrtf(hf_vec4f_square_distance(a, b));
}

float hf_vec4f_dot(hf_vec4f a, hf_vec4f b) {
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

void hf_vec4d_copy(hf_vec4d vec, hf_vec4d out) {
	memcpy(out, vec, sizeof(out[0]) * 4);
}

void hf_vec4d_add(hf_vec4d a, hf_vec4d b, hf_vec4d out) {
	out[0] = a[0] + b[0];
	out[1] = a[1] + b[1];
	out[2] = a[2] + b[2];
	out[3] = a[3] + b[3];
}

void hf_vec4d_subtract(hf_vec4d a, hf_vec4d b, hf_vec4d out) {
	out[0] = a[0] - b[0];
	out[1] = a[1] - b[1];
	out[2] = a[2] - b[2];
	out[3] = a[3] - b[3];
}

void hf_vec4d_multiply(hf_vec4d vec, double scalar, hf_vec4d out) {
	out[0] = vec[0] * scalar;
	out[1] = vec[1] * scalar;
	out[2] = vec[2] * scalar;
	out[3] = vec[3] * scalar;
}

void hf_vec4d_divide(hf_vec4d vec, double scalar, hf_vec4d out) {
	out[0] = vec[0] / scalar;
	out[1] = vec[1] / scalar;
	out[2] = vec[2] / scalar;
	out[3] = vec[3] / scalar;
}

void hf_vec4d_normalize(hf_vec4d vec, hf_vec4d out) {
	hf_vec4d_divide(vec, hf_vec4d_magnitude(vec), out);
}

double hf_vec4d_square_magnitude(hf_vec4d vec) {
	return vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2] + vec[3] * vec[3];
}

double hf_vec4d_magnitude(hf_vec4d vec) {
	return sqrt(hf_vec4d_square_magnitude(vec));
}

double hf_vec4d_square_distance(hf_vec4d a, hf_vec4d b) {
	hf_vec4d aux;
	hf_vec4d_subtract(a, b, aux);
	return hf_vec4d_square_magnitude(aux);
}

double hf_vec4d_distance(hf_vec4d a, hf_vec4d b) {
	return sqrt(hf_vec4d_square_distance(a, b));
}

double hf_vec4d_dot(hf_vec4d a, hf_vec4d b) {
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}
