#ifndef HFE_H
#define HFE_H

#include <stdbool.h>

#include "glad/glad.h"

const char* hfe_error_get(void);

//SHADER
typedef enum hfe_shader_type_e {
    hfe_shader_type_vertex = 0,
    hfe_shader_type_fragment,
} hfe_shader_type;

typedef struct hfe_shader_s {
    GLuint id;
} hfe_shader;
hfe_shader hfe_shader_create_from_string(hfe_shader_type type, const char* source);
hfe_shader hfe_shader_create_from_file(hfe_shader_type type, const char* path);
void hfe_shader_destroy(hfe_shader shader);
bool hfe_shader_valid(hfe_shader shader);

//SHADER PROGRAM
typedef struct hfe_shader_program_s {
    GLuint id;
} hfe_shader_program;
hfe_shader_program hfe_shader_program_create(hfe_shader* shaders, size_t shaders_count);
void hfe_shader_program_destroy(hfe_shader_program program);
void hfe_shader_program_use(hfe_shader_program program);

typedef struct hfe_shader_property_s {
    GLint id;
} hfe_shader_property;
hfe_shader_property hfe_shader_property_get(const char* name);
void hfe_shader_property_set_1i(hfe_shader_property property, int x);
void hfe_shader_property_set_2i(hfe_shader_property property, int x, int y);
void hfe_shader_property_set_3i(hfe_shader_property property, int x, int y, int z);
void hfe_shader_property_set_4i(hfe_shader_property property, int x, int y, int z, int w);
void hfe_shader_property_set_1f(hfe_shader_property property, float x);
void hfe_shader_property_set_2f(hfe_shader_property property, float x, float y);
void hfe_shader_property_set_3f(hfe_shader_property property, float x, float y, float z);
void hfe_shader_property_set_4f(hfe_shader_property property, float x, float y, float z, float w);
void hfe_shader_property_set_mat3f(hfe_shader_property property, float* value);
void hfe_shader_property_set_mat4f(hfe_shader_property property, float* value);
bool hfe_shader_property_valid(hfe_shader_property property);

//MESH
typedef enum hfe_vertex_spec_type_e {
    hfe_vertex_spec_type_byte = GL_BYTE,
    hfe_vertex_spec_type_unsigned_byte = GL_UNSIGNED_BYTE,
    hfe_vertex_spec_type_short = GL_SHORT,
    hfe_vertex_spec_type_unsigned_short = GL_UNSIGNED_SHORT,
    hfe_vertex_spec_type_int = GL_INT,
    hfe_vertex_spec_type_unsigned_int = GL_UNSIGNED_INT,
    hfe_vertex_spec_type_float = GL_FLOAT,
    hfe_vertex_spec_type_double = GL_DOUBLE,
} hfe_vertex_spec_type;

typedef enum hfe_vertex_spec_width_e {
    hfe_vertex_spec_width_one = 1,
    hfe_vertex_spec_width_two = 2,
    hfe_vertex_spec_width_three = 3,
    hfe_vertex_spec_width_four = 4,
} hfe_vertex_spec_width;

typedef struct hfe_vertex_spec_s {
    hfe_vertex_spec_type type;
    hfe_vertex_spec_width width;
} hfe_vertex_spec;

typedef struct hfe_mesh_s {
    GLuint id;
    GLuint buffers[2];
    unsigned short count;
} hfe_mesh;

hfe_mesh hfe_mesh_create(void* data, size_t size, unsigned short vertex_count);
hfe_mesh hfe_mesh_create_indexed(void* data, size_t size, unsigned short vertex_count, unsigned short* triangle_data, unsigned short triangle_count);

hfe_mesh hfe_mesh_create_f(void* data, unsigned short vertex_count, hfe_vertex_spec_width width0);
hfe_mesh hfe_mesh_create_ff(void* data, unsigned short vertex_count, hfe_vertex_spec_width width0, hfe_vertex_spec_width width1);
hfe_mesh hfe_mesh_create_fff(void* data, unsigned short vertex_count, hfe_vertex_spec_width width0, hfe_vertex_spec_width width1, hfe_vertex_spec_width width2);

hfe_mesh hfe_mesh_create_indexed_f(void* data, unsigned short vertex_count, unsigned short* triangle_data, unsigned short triangle_count, hfe_vertex_spec_width width0);
hfe_mesh hfe_mesh_create_indexed_ff(void* data, unsigned short vertex_count, unsigned short* triangle_data, unsigned short triangle_count, hfe_vertex_spec_width width0, hfe_vertex_spec_width width1);
hfe_mesh hfe_mesh_create_indexed_fff(void* data, unsigned short vertex_count, unsigned short* triangle_data, unsigned short triangle_count, hfe_vertex_spec_width width0, hfe_vertex_spec_width width1, hfe_vertex_spec_width width2);

hfe_mesh hfe_mesh_create_primitive_cube(float size);
hfe_mesh hfe_mesh_create_primitive_cylinder(float radius, float height);//creates a cylinder mesh with specified radius and height.
hfe_mesh hfe_mesh_create_primitive_plane(float size_x, float size_y);
hfe_mesh hfe_mesh_create_primitive_pyramid(float base_size, float height);
hfe_mesh hfe_mesh_create_primitive_quad(float size);
hfe_mesh hfe_mesh_create_primitive_quad_ui(void);
hfe_mesh hfe_mesh_create_primitive_rectangular_prism(float size_x, float size_y, float size_z);

hfe_mesh hfe_mesh_create_from_file_obj(const char* path);

void hfe_mesh_destroy(hfe_mesh mesh);
void hfe_mesh_use(hfe_mesh mesh);
void hfe_mesh_reset(void);
void hfe_mesh_draw(void);
void hfe_mesh_vertex_spec_set(hfe_mesh mesh, size_t index, hfe_vertex_spec spec, size_t stride, size_t offset);
void hfe_mesh_vertex_specs_set(hfe_mesh mesh, hfe_vertex_spec* specs, size_t count);

typedef enum hfe_texture_wrap_mode_e {
    hfe_texture_wrap_mode_repeat = GL_REPEAT,
    hfe_texture_wrap_mode_clamp = GL_CLAMP_TO_EDGE,
    hfe_texture_wrap_mode_mirror = GL_MIRRORED_REPEAT,
} hfe_texture_wrap_mode;

typedef enum hfe_texture_filter_e {
    hfe_texture_filter_nearest = GL_NEAREST,
    hfe_texture_filter_nearest_mipmap = GL_NEAREST_MIPMAP_NEAREST,
    hfe_texture_filter_nearest_mipmaps = GL_NEAREST_MIPMAP_LINEAR,
    hfe_texture_filter_linear = GL_LINEAR,
    hfe_texture_filter_linear_mipmap = GL_LINEAR_MIPMAP_NEAREST,
    hfe_texture_filter_linear_mipmaps = GL_LINEAR_MIPMAP_LINEAR,
} hfe_texture_filter;

typedef enum hfe_texture_pixel_type_e {
    hfe_texture_pixel_type_unsigned_byte = GL_UNSIGNED_BYTE,
    hfe_texture_pixel_type_float = GL_FLOAT,
} hfe_texture_pixel_type;

typedef enum hfe_texture_pixel_format_e {
    hfe_texture_pixel_format_rgba = GL_RGBA,
    hfe_texture_pixel_format_depth = GL_DEPTH_COMPONENT,
} hfe_texture_pixel_format;

typedef struct hfe_texture_configuration_s {
    hfe_texture_wrap_mode wrap_x;
    hfe_texture_wrap_mode wrap_y;
    hfe_texture_filter filter_min;
    hfe_texture_filter filter_mag;
} hfe_texture_configuration;

typedef struct hfe_texture_s {
    GLuint id;
} hfe_texture;

hfe_texture hfe_texture_create(int w, int h, hfe_texture_pixel_type pixel_type, hfe_texture_pixel_format data_format, hfe_texture_configuration configuration);
hfe_texture hfe_texture_create_from_file(const char* path, hfe_texture_configuration configuration);
hfe_texture hfe_texture_create_from_bytes(unsigned char* data, int w, int h, hfe_texture_pixel_type pixel_type, hfe_texture_pixel_format data_format, hfe_texture_configuration configuration);
void hfe_texture_destroy(hfe_texture texture);
void hfe_texture_use(hfe_texture texture, size_t unit);
bool hfe_texture_valid(hfe_texture texture);

typedef struct hfe_depth_buffer_s {
    GLuint id;
} hfe_depth_buffer;

hfe_depth_buffer hfe_depth_buffer_create(int w, int h);
void hfe_depth_buffer_destroy(hfe_depth_buffer depth_buffer);
bool hfe_depth_buffer_valid(hfe_depth_buffer depth_buffer);

typedef struct hfe_render_target_s {
    GLuint id;
    hfe_texture texture;
    hfe_depth_buffer depth_buffer;
} hfe_render_target;

hfe_render_target hfe_render_target_create_color(int w, int h, hfe_texture_configuration texture_configuration);
hfe_render_target hfe_render_target_create_depth(int w, int h, hfe_texture_configuration texture_configuration);
void hfe_render_target_destroy(hfe_render_target render_target);
bool hfe_render_target_valid(hfe_render_target render_target);
void hfe_render_target_use(hfe_render_target render_target);
void hfe_render_target_reset(void);

//UTIL
void hfe_util_calculate_normals(float* vertices, size_t vertices_stride, float* normals, size_t normals_stride, unsigned short* triangles, unsigned short triangle_count);

bool hfe_init_opengl(void);

#endif//HFE_H
