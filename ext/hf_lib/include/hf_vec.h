#ifndef HF_VEC_H
#define HF_VEC_H

typedef int hf_vec2i[2];
typedef float hf_vec2f[2];
typedef double hf_vec2d[2];
typedef int hf_vec3i[3];
typedef float hf_vec3f[3];
typedef double hf_vec3d[3];
typedef int hf_vec4i[4];
typedef float hf_vec4f[4];
typedef double hf_vec4d[4];

void hf_vec2i_copy(hf_vec2i vec, hf_vec2i out);
void hf_vec2i_add(hf_vec2i a, hf_vec2i b, hf_vec2i out);
void hf_vec2i_subtract(hf_vec2i a, hf_vec2i b, hf_vec2i out);
void hf_vec2i_multiply(hf_vec2i vec, int scalar, hf_vec2i out);
void hf_vec2i_divide(hf_vec2i vec, int scalar, hf_vec2i out);
int hf_vec2i_square_magnitude(hf_vec2i vec);
float hf_vec2i_magnitude(hf_vec2i vec);
int hf_vec2i_square_distance(hf_vec2i a, hf_vec2i b);
float hf_vec2i_distance(hf_vec2i a, hf_vec2i b);
int hf_vec2i_dot(hf_vec2i a, hf_vec2i b);

void hf_vec2f_copy(hf_vec2f vec, hf_vec2f out);
void hf_vec2f_add(hf_vec2f a, hf_vec2f b, hf_vec2f out);
void hf_vec2f_subtract(hf_vec2f a, hf_vec2f b, hf_vec2f out);
void hf_vec2f_multiply(hf_vec2f vec, float scalar, hf_vec2f out);
void hf_vec2f_divide(hf_vec2f vec, float scalar, hf_vec2f out);
void hf_vec2f_normalize(hf_vec2f vec, hf_vec2f out);
void hf_vec2f_lerp(hf_vec2f a, hf_vec2f b, float t, hf_vec2f out);
float hf_vec2f_square_magnitude(hf_vec2f vec);
float hf_vec2f_magnitude(hf_vec2f vec);
float hf_vec2f_square_distance(hf_vec2f a, hf_vec2f b);
float hf_vec2f_distance(hf_vec2f a, hf_vec2f b);
float hf_vec2f_dot(hf_vec2f a, hf_vec2f b);

void hf_vec2d_copy(hf_vec2d vec, hf_vec2d out);
void hf_vec2d_add(hf_vec2d a, hf_vec2d b, hf_vec2d out);
void hf_vec2d_subtract(hf_vec2d a, hf_vec2d b, hf_vec2d out);
void hf_vec2d_multiply(hf_vec2d vec, double scalar, hf_vec2d out);
void hf_vec2d_divide(hf_vec2d vec, double scalar, hf_vec2d out);
void hf_vec2d_normalize(hf_vec2d vec, hf_vec2d out);
double hf_vec2d_square_magnitude(hf_vec2d vec);
double hf_vec2d_magnitude(hf_vec2d vec);
double hf_vec2d_square_distance(hf_vec2d a, hf_vec2d b);
double hf_vec2d_distance(hf_vec2d a, hf_vec2d b);
double hf_vec2d_dot(hf_vec2d a, hf_vec2d b);

void hf_vec3i_copy(hf_vec3i vec, hf_vec3i out);
void hf_vec3i_add(hf_vec3i a, hf_vec3i b, hf_vec3i out);
void hf_vec3i_subtract(hf_vec3i a, hf_vec3i b, hf_vec3i out);
void hf_vec3i_multiply(hf_vec3i vec, int scalar, hf_vec3i out);
void hf_vec3i_divide(hf_vec3i vec, int scalar, hf_vec3i out);
int hf_vec3i_square_magnitude(hf_vec3i vec);
float hf_vec3i_magnitude(hf_vec3i vec);
int hf_vec3i_square_distance(hf_vec3i a, hf_vec3i b);
float hf_vec3i_distance(hf_vec3i a, hf_vec3i b);
int hf_vec3i_dot(hf_vec3i a, hf_vec3i b);
void hf_vec3i_cross(hf_vec3i a, hf_vec3i b, hf_vec3i out);

void hf_vec3f_copy(hf_vec3f vec, hf_vec3f out);
void hf_vec3f_add(hf_vec3f a, hf_vec3f b, hf_vec3f out);
void hf_vec3f_subtract(hf_vec3f a, hf_vec3f b, hf_vec3f out);
void hf_vec3f_multiply(hf_vec3f vec, float scalar, hf_vec3f out);
void hf_vec3f_divide(hf_vec3f vec, float scalar, hf_vec3f out);
void hf_vec3f_normalize(hf_vec3f vec, hf_vec3f out);
void hf_vec3f_lerp(hf_vec3f a, hf_vec3f b, float t, hf_vec3f out);
float hf_vec3f_square_magnitude(hf_vec3f vec);
float hf_vec3f_magnitude(hf_vec3f vec);
float hf_vec3f_square_distance(hf_vec3f a, hf_vec3f b);
float hf_vec3f_distance(hf_vec3f a, hf_vec3f b);
float hf_vec3f_dot(hf_vec3f a, hf_vec3f b);
void hf_vec3f_cross(hf_vec3f a, hf_vec3f b, hf_vec3f out);

void hf_vec3d_copy(hf_vec3d vec, hf_vec3d out);
void hf_vec3d_add(hf_vec3d a, hf_vec3d b, hf_vec3d out);
void hf_vec3d_subtract(hf_vec3d a, hf_vec3d b, hf_vec3d out);
void hf_vec3d_multiply(hf_vec3d vec, double scalar, hf_vec3d out);
void hf_vec3d_divide(hf_vec3d vec, double scalar, hf_vec3d out);
void hf_vec3d_normalize(hf_vec3d vec, hf_vec3d out);
double hf_vec3d_square_magnitude(hf_vec3d vec);
double hf_vec3d_magnitude(hf_vec3d vec);
double hf_vec3d_square_distance(hf_vec3d a, hf_vec3d b);
double hf_vec3d_distance(hf_vec3d a, hf_vec3d b);
double hf_vec3d_dot(hf_vec3d a, hf_vec3d b);
void hf_vec3d_cross(hf_vec3d a, hf_vec3d b, hf_vec3d out);

void hf_vec4i_copy(hf_vec4i vec, hf_vec4i out);
void hf_vec4i_add(hf_vec4i a, hf_vec4i b, hf_vec4i out);
void hf_vec4i_subtract(hf_vec4i a, hf_vec4i b, hf_vec4i out);
void hf_vec4i_multiply(hf_vec4i vec, int scalar, hf_vec4i out);
void hf_vec4i_divide(hf_vec4i vec, int scalar, hf_vec4i out);
int hf_vec4i_square_magnitude(hf_vec4i vec);
float hf_vec4i_magnitude(hf_vec4i vec);
int hf_vec4i_square_distance(hf_vec4i a, hf_vec4i b);
float hf_vec4i_distance(hf_vec4i a, hf_vec4i b);
int hf_vec4i_dot(hf_vec4i a, hf_vec4i b);

void hf_vec4f_copy(hf_vec4f vec, hf_vec4f out);
void hf_vec4f_add(hf_vec4f a, hf_vec4f b, hf_vec4f out);
void hf_vec4f_subtract(hf_vec4f a, hf_vec4f b, hf_vec4f out);
void hf_vec4f_multiply(hf_vec4f vec, float scalar, hf_vec4f out);
void hf_vec4f_divide(hf_vec4f vec, float scalar, hf_vec4f out);
void hf_vec4f_normalize(hf_vec4f vec, hf_vec4f out);
void hf_vec4f_lerp(hf_vec4f a, hf_vec4f b, float t, hf_vec4f out);
float hf_vec4f_square_magnitude(hf_vec4f vec);
float hf_vec4f_magnitude(hf_vec4f vec);
float hf_vec4f_square_distance(hf_vec4f a, hf_vec4f b);
float hf_vec4f_distance(hf_vec4f a, hf_vec4f b);
float hf_vec4f_dot(hf_vec4f a, hf_vec4f b);

void hf_vec4d_copy(hf_vec4d vec, hf_vec4d out);
void hf_vec4d_add(hf_vec4d a, hf_vec4d b, hf_vec4d out);
void hf_vec4d_subtract(hf_vec4d a, hf_vec4d b, hf_vec4d out);
void hf_vec4d_multiply(hf_vec4d vec, double scalar, hf_vec4d out);
void hf_vec4d_divide(hf_vec4d vec, double scalar, hf_vec4d out);
void hf_vec4d_normalize(hf_vec4d vec, hf_vec4d out);
double hf_vec4d_square_magnitude(hf_vec4d vec);
double hf_vec4d_magnitude(hf_vec4d vec);
double hf_vec4d_square_distance(hf_vec4d a, hf_vec4d b);
double hf_vec4d_distance(hf_vec4d a, hf_vec4d b);
double hf_vec4d_dot(hf_vec4d a, hf_vec4d b);

#endif//HF_VEC_H
