#include "hfe.h"

#include <string.h>
#include <math.h>

#include "stb/stb_image.h"

//utility arraylist
typedef struct hfe_dynarray_s {
    void* data;
    size_t capacity;
    size_t count;
    size_t item_size;
    size_t growth_rate;
} hfe_dynarray;

static void hfe_dynarray_init(hfe_dynarray* arr, size_t item_size, size_t growth_rate) {
    arr->data = NULL;
    arr->capacity = 0;
    arr->count = 0;
    arr->item_size = item_size;
    arr->growth_rate = growth_rate;
}

static void hfe_dynarray_deinit(hfe_dynarray* arr) {
    free(arr->data);
}

static void* hfe_dynarray_get(hfe_dynarray* arr, size_t index) {
    char* data = (char*)arr->data;
    return &data[index * arr->item_size];
}

static bool hfe_dynarray_push(hfe_dynarray* arr, void* item) {
    if(arr->count >= arr->capacity) {
        void* new_memory = realloc(arr->data, (arr->capacity + arr->growth_rate) * arr->item_size);
        if(!new_memory) {
            return false;
        }

        arr->data = new_memory;
        arr->capacity += arr->growth_rate;
    }
    memcpy(hfe_dynarray_get(arr, arr->count++), item, arr->item_size);
    return true;
}

//utility text parsing functions from hf_lib
static bool hfe_string_starts_with(const char* string, const char* other) {
    while(*string && *other) {
        if(*string != *other) {
            return false;
        }
        string++;
        other++;
    }
    return !(*other);
}

static const char* hfe_string_parse_ull(const char* string, unsigned long long* value) {
    if(!string || string[0] < '0' || string[0] > '9') {
        return NULL;
    }

    const char* ptr = string;
    const char* start = ptr;//due to previous checks, start is guaranteed to be a number here
    do {
        ptr++;
    } while(ptr[0] >= '0' && ptr[0] <= '9');
    const char* end = ptr;

    if(value) {
        unsigned long long mul = 1;
        *value = 0;
        while(ptr != start) {
            ptr--;
            *value += mul * (unsigned long long)(*ptr - '0');
            mul *= 10;
        }
    }

    return end;
}

static const char* hfe_string_parse_ll(const char* string, long long* value) {
    if(!string) {
        return NULL;
    }

    bool neg = string[0] == '-';
    unsigned long long ull;

    const char* ptr = hfe_string_parse_ull(neg ? &string[1] : string, &ull);
    if(value && ptr) {
        *value = (long long)ull;
        if(neg) {
            *value = -*value;
        }
    }
    return ptr;
}

static const char* hfe_string_parse_double(const char* string, double* value) {
    if(!string) {
        return NULL;
    }

    bool neg = string[0] == '-';

    long long integer = 0;
    unsigned long long fraction = 0;
    unsigned long long fraction_digits = 0;
    long long exponent = 0;

    const char* ptr = hfe_string_parse_ll(string, &integer);
    if(!ptr) {
        return NULL;
    }

    if(ptr[0] == '.') {
        const char* tmp = hfe_string_parse_ull(&ptr[1], &fraction);
        if(tmp) {
            ptr++;
            while(ptr != tmp) {
                ptr++;
                fraction_digits++;
            }
        }
    }
    if(ptr[0] == 'e') {
        const char* tmp = hfe_string_parse_ll(&ptr[1], &exponent);
        if(tmp) {
            ptr = tmp;
        }
    }

    if(value) {
        double dinteger = (double)integer;

        double dfraction = neg ? -(double)fraction : (double)fraction;
        while(fraction_digits) {
            fraction_digits--;
            dfraction /= 10.0;
        }
        *value = dinteger + dfraction;

        while(exponent > 0) {
            *value *= 10.0;
            exponent--;
        }
        while(exponent < 0) {
            *value /= 10.0;
            exponent++;
        }
    }

    return ptr;
}

static const char* hfe_string_parse_float(const char* string, float* value) {
    double d;
    const char* ptr = hfe_string_parse_double(string, &d);
    if(ptr && value) {
        *value = (float)d;
    }
    return ptr;
}

static const char* hfe_string_find_ull(const char* string, unsigned long long* value) {
    const char* ptr = string;
    while(*ptr != '\0') {
        const char* ret = hfe_string_parse_ull(ptr, value);
        if(ret) {
            return ret;
        }
        ptr++;
    }
    return NULL;
}

static const char* hfe_string_find_float(const char* string, float* value) {
    const char* ptr = string;
    while(*ptr != '\0') {
        const char* ret = hfe_string_parse_float(ptr, value);
        if(ret) {
            return ret;
        }
        ptr++;
    }
    return NULL;
}

static bool hfe_file_read_line(FILE* file, char* buffer, size_t buffer_len) {
    size_t index = 0;
    size_t ret = 0;
    char c = '\0';
    while((ret = fread(&c, sizeof(char), 1, file)) && c != '\r' && c != '\n') {
        if(index < (buffer_len - 1)) {
            buffer[index++] = c;
        }
    }

    if(!ret && !index) {
        return false;
    }

    if(c == '\r' && fread(&c, sizeof(char), 1, file)) {
        if(c != '\n') {
            fseek(file, -1, SEEK_CUR);
        }
    }
    buffer[index] = '\0';
    return true;
}

#define HFE_ERROR_LOG_LEN 256
static char hfe_error_log[HFE_ERROR_LOG_LEN];

const char* hfe_error_get(void) {
    return hfe_error_log;
}

static void internal_hfe_error_set(const char* message) {
    hfe_error_log[0] = '\0';
    if(!message) {
        return;
    }

    const char* ptr = message;
    size_t i = 0;
    while(*ptr && i < (HFE_ERROR_LOG_LEN - 1)) {
        hfe_error_log[i] = *ptr;
        i++;
        ptr++;
    }
    hfe_error_log[i] = '\0';
}

typedef struct hfe_api_ptrs_s {
    hfe_shader(*shader_create_from_string)(hfe_shader_type, const char*);
    void(*shader_destroy)(hfe_shader);
    bool(*shader_valid)(hfe_shader);

    hfe_shader_program(*shader_program_create)(hfe_shader*, size_t);
    void(*shader_program_destroy)(hfe_shader_program);
    void(*shader_program_use)(hfe_shader_program);

    hfe_shader_property (*shader_property_get)(const char*);
    void (*shader_property_set_1i)(hfe_shader_property, int);
    void (*shader_property_set_2i)(hfe_shader_property, int, int);
    void (*shader_property_set_3i)(hfe_shader_property, int, int, int);
    void (*shader_property_set_4i)(hfe_shader_property, int, int, int, int);
    void (*shader_property_set_1f)(hfe_shader_property, float);
    void (*shader_property_set_2f)(hfe_shader_property, float, float);
    void (*shader_property_set_3f)(hfe_shader_property, float, float, float);
    void (*shader_property_set_4f)(hfe_shader_property, float, float, float, float);
    void (*shader_property_set_mat3f)(hfe_shader_property, float*);
    void (*shader_property_set_mat4f)(hfe_shader_property, float*);
    bool (*shader_property_valid)(hfe_shader_property);

    hfe_mesh(*mesh_create_indexed)(void*, size_t, unsigned short, unsigned short*, unsigned short);
    void(*mesh_destroy)(hfe_mesh);
    void(*mesh_use)(hfe_mesh);
    void(*mesh_reset)(void);
    void(*mesh_draw)(void);
    void(*mesh_vertex_spec_set)(hfe_mesh, size_t, hfe_vertex_spec, size_t, size_t);
    void(*mesh_vertex_specs_set)(hfe_mesh, hfe_vertex_spec*, size_t);

    hfe_texture(*hfe_texture_create)(int, int, hfe_texture_pixel_type, hfe_texture_pixel_format, hfe_texture_configuration, unsigned char*);
    void(*hfe_texture_destroy)(hfe_texture);
    void(*hfe_texture_use)(hfe_texture, size_t);
    bool(*hfe_texture_valid)(hfe_texture);

    hfe_depth_buffer(*hfe_depth_buffer_create)(int, int);
    void(*hfe_depth_buffer_destroy)(hfe_depth_buffer);
    bool(*hfe_depth_buffer_valid)(hfe_depth_buffer);

    hfe_render_target(*hfe_render_target_create_color)(int, int, hfe_texture_configuration);
    hfe_render_target(*hfe_render_target_create_depth)(int, int, hfe_texture_configuration);
    void(*hfe_render_target_destroy)(hfe_render_target);
    bool(*hfe_render_target_valid)(hfe_render_target);
    void(*hfe_render_target_use)(hfe_render_target);
    void(*hfe_render_target_reset)(void);
} hfe_api_ptrs;
hfe_api_ptrs internal_hfe_api_ptrs = { 0 };

hfe_shader hfe_shader_create_from_file(hfe_shader_type type, const char* path) {
    hfe_shader shader = { 0 };

    FILE* file = fopen(path, "rb");
    if(!file) {
        goto error_file;
    }

    long tell;
    if(
        fseek(file, 0, SEEK_END) != 0 ||
        (tell = ftell(file)) == -1 ||
        fseek(file, 0, SEEK_SET) != 0
    ) {
        goto error_tell;
    }
    size_t length = (size_t)tell;

    char* source = malloc(length + 1);
    if(!source) {
        goto error_alloc;
    }
    if(fread(source, 1, length, file) != length) {
        goto error_read;
    }
    source[length] = '\0';

    shader = internal_hfe_api_ptrs.shader_create_from_string(type, source);

    error_read:
        free(source);
    error_tell:
    error_alloc:
        fclose(file);
    error_file:
        return shader;
}

void hfe_shader_destroy(hfe_shader shader) {
    internal_hfe_api_ptrs.shader_destroy(shader);
}

bool hfe_shader_valid(hfe_shader shader) {
    return internal_hfe_api_ptrs.shader_valid(shader);
}


hfe_shader_program hfe_shader_program_create(hfe_shader* shaders, size_t shaders_count) {
    return internal_hfe_api_ptrs.shader_program_create(shaders, shaders_count);
}

void hfe_shader_program_destroy(hfe_shader_program program) {
    internal_hfe_api_ptrs.shader_program_destroy(program);
}

static hfe_shader_program hfe_active_program = { 0 };
void hfe_shader_program_use(hfe_shader_program program) {
    internal_hfe_api_ptrs.shader_program_use(program);
}


hfe_shader_property hfe_shader_property_get(const char* name) {
    return internal_hfe_api_ptrs.shader_property_get(name);
}

void hfe_shader_property_set_1i(hfe_shader_property property, int x) {
    internal_hfe_api_ptrs.shader_property_set_1i(property, x);
}

void hfe_shader_property_set_2i(hfe_shader_property property, int x, int y) {
    internal_hfe_api_ptrs.shader_property_set_2i(property, x, y);
}

void hfe_shader_property_set_3i(hfe_shader_property property, int x, int y, int z) {
    internal_hfe_api_ptrs.shader_property_set_3i(property, x, y, z);
}

void hfe_shader_property_set_4i(hfe_shader_property property, int x, int y, int z, int w) {
    internal_hfe_api_ptrs.shader_property_set_4i(property, x, y, z, w);
}

void hfe_shader_property_set_1f(hfe_shader_property property, float x) {
    internal_hfe_api_ptrs.shader_property_set_1f(property, x);
}

void hfe_shader_property_set_2f(hfe_shader_property property, float x, float y) {
    internal_hfe_api_ptrs.shader_property_set_2f(property, x, y);
}

void hfe_shader_property_set_3f(hfe_shader_property property, float x, float y, float z) {
    internal_hfe_api_ptrs.shader_property_set_3f(property, x, y, z);
}

void hfe_shader_property_set_4f(hfe_shader_property property, float x, float y, float z, float w) {
    internal_hfe_api_ptrs.shader_property_set_4f(property, x, y, z, w);
}

void hfe_shader_property_set_mat3f(hfe_shader_property property, float* value) {
    internal_hfe_api_ptrs.shader_property_set_mat3f(property, value);
}

void hfe_shader_property_set_mat4f(hfe_shader_property property, float* value) {
    internal_hfe_api_ptrs.shader_property_set_mat4f(property, value);
}

bool hfe_shader_property_valid(hfe_shader_property property) {
    return property.id >= 0;
}


static size_t internal_hfe_vertex_spec_type_sizeof(hfe_vertex_spec_type type) {
    switch(type) {
        case hfe_vertex_spec_type_byte:
            return sizeof(char);
        case hfe_vertex_spec_type_unsigned_byte:
            return sizeof(unsigned char);
        case hfe_vertex_spec_type_short:
            return sizeof(short);
        case hfe_vertex_spec_type_unsigned_short:
            return sizeof(unsigned short);
        case hfe_vertex_spec_type_int:
            return sizeof(int);
        case hfe_vertex_spec_type_unsigned_int:
            return sizeof(unsigned int);
        case hfe_vertex_spec_type_float:
            return sizeof(float);
        case hfe_vertex_spec_type_double:
            return sizeof(double);
    }
    return 0;
}

static size_t internal_hfe_vertex_spec_sizeof(hfe_vertex_spec spec) {
    return internal_hfe_vertex_spec_type_sizeof(spec.type) * (size_t)spec.width;
}

hfe_mesh hfe_mesh_create(void* data, size_t size, unsigned short vertex_count) {
    return hfe_mesh_create_indexed(data, size, vertex_count, NULL, 0);
}

hfe_mesh hfe_mesh_create_indexed(void* data, size_t size, unsigned short vertex_count, unsigned short* triangle_data, unsigned short triangle_count) {
    return internal_hfe_api_ptrs.mesh_create_indexed(data, size, vertex_count, triangle_data, triangle_count);
}

hfe_mesh hfe_mesh_create_f(void* data, unsigned short vertex_count, hfe_vertex_spec_width width0) {
    return hfe_mesh_create_indexed_f(data, vertex_count, NULL, 0, width0);
}

hfe_mesh hfe_mesh_create_ff(void* data, unsigned short vertex_count, hfe_vertex_spec_width width0, hfe_vertex_spec_width width1) {
        return hfe_mesh_create_indexed_ff(data, vertex_count, NULL, 0, width0, width1);
}

hfe_mesh hfe_mesh_create_fff(void* data, unsigned short vertex_count, hfe_vertex_spec_width width0, hfe_vertex_spec_width width1, hfe_vertex_spec_width width2) {
    return hfe_mesh_create_indexed_fff(data, vertex_count, NULL, 0, width0, width1, width2);
}

hfe_mesh hfe_mesh_create_indexed_f(void* data, unsigned short vertex_count, unsigned short* triangles, unsigned short triangles_count, hfe_vertex_spec_width width0) {
    size_t size = internal_hfe_vertex_spec_type_sizeof(hfe_vertex_spec_type_float) * (size_t)(width0) * vertex_count;
    hfe_mesh mesh = hfe_mesh_create_indexed(data, size, vertex_count, triangles, triangles_count);

    hfe_mesh_vertex_specs_set(
        mesh,
        (hfe_vertex_spec[]) {
            { .type = hfe_vertex_spec_type_float, .width = width0 },
        },
        1
    );

    return mesh;
}

hfe_mesh hfe_mesh_create_indexed_ff(void* data, unsigned short vertex_count, unsigned short* triangles, unsigned short triangles_count, hfe_vertex_spec_width width0, hfe_vertex_spec_width width1) {
    size_t size = internal_hfe_vertex_spec_type_sizeof(hfe_vertex_spec_type_float) * (size_t)(width0 + width1) * vertex_count;
    hfe_mesh mesh = hfe_mesh_create_indexed(data, size, vertex_count, triangles, triangles_count);

    hfe_mesh_vertex_specs_set(
        mesh,
        (hfe_vertex_spec[]) {
            { .type = hfe_vertex_spec_type_float, .width = width0 },
            { .type = hfe_vertex_spec_type_float, .width = width1 },
        },
        2
    );

    return mesh;
}

hfe_mesh hfe_mesh_create_indexed_fff(void* data, unsigned short vertex_count, unsigned short* triangles, unsigned short triangles_count, hfe_vertex_spec_width width0, hfe_vertex_spec_width width1, hfe_vertex_spec_width width2) {
    size_t size = internal_hfe_vertex_spec_type_sizeof(hfe_vertex_spec_type_float) * (size_t)(width0 + width1 + width2) * vertex_count;
    hfe_mesh mesh = hfe_mesh_create_indexed(data, size, vertex_count, triangles, triangles_count);

    hfe_mesh_vertex_specs_set(
        mesh,
        (hfe_vertex_spec[]) {
            { .type = hfe_vertex_spec_type_float, .width = width0 },
            { .type = hfe_vertex_spec_type_float, .width = width1 },
            { .type = hfe_vertex_spec_type_float, .width = width2 },
        },
        3
    );

    return mesh;
}


hfe_mesh hfe_mesh_create_primitive_cube(float size) {
    return hfe_mesh_create_primitive_rectangular_prism(size, size, size);
}

hfe_mesh hfe_mesh_create_primitive_cylinder(float radius, float height) {
    float vertex_data[(33 * 2 + 32 * 2) * 8];// 33 * 2 verts around(1 dup) + 32 * 2 cap verts
    unsigned short triangle_data[(32 * 2 + 30 * 2) * 3];//32 * 2 tris around + 30 * 2 cap tris

    //vert creation
    float rot_step = 6.283f / 32.f;
    for(unsigned short i = 0; i < 33; i++) {
        float rot = rot_step * (float)(i % 32);
        float vert_base[2] = { cosf(rot), -sinf(rot) };

        unsigned short top_index = 2 * i;
        unsigned short bot_index = 2 * i + 1;

        float* top_data = &vertex_data[top_index * 8];
        float* bot_data = &vertex_data[bot_index * 8];

        //vertex
        top_data[0] = bot_data[0] = vert_base[0] * radius;
        top_data[2] = bot_data[2] = vert_base[1] * radius;
        top_data[1] = height / 2.f;
        bot_data[1] = -height / 2.f;

        //uv
        top_data[3] = bot_data[3] = (1.f / 32.f) * (float)i;
        top_data[4] = 1.f;
        bot_data[4] = 0.f;

        //normal
        top_data[5] = bot_data[5] = vert_base[0];
        top_data[6] = bot_data[6] = 0.f;
        top_data[7] = bot_data[7] = vert_base[1];
    }

    //cap verts
    for(unsigned short i = 0; i < 32; i++) {
        float rot = rot_step * (float)i;
        float vert_base[2] = { cosf(rot), -sinf(rot) };

        unsigned short top_index = 33 * 2 + 2 * i;
        unsigned short bot_index = 33 * 2 + 2 * i + 1;

        float* top_data = &vertex_data[top_index * 8];
        float* bot_data = &vertex_data[bot_index * 8];

        //vertex
        top_data[0] = bot_data[0] = vert_base[0] * radius;
        top_data[2] = bot_data[2] = vert_base[1] * radius;
        top_data[1] = height / 2.f;
        bot_data[1] = -height / 2.f;

        //uv
        top_data[3] = bot_data[3] = (vert_base[0] + 1.f) / 2.f;
        top_data[4] = bot_data[4] = (vert_base[1] + 1.f) / 2.f;

        //normal
        top_data[5] = bot_data[5] = 0.f;
        top_data[6] =  1.f;
        bot_data[6] = -1.f;
        top_data[7] = bot_data[7] = 0.f;
    }

    //side triangles
    for(unsigned short i = 0; i < 32; i++) {
        unsigned short* tri = &triangle_data[2 * 3 * i];

        tri[0] = 2 * i + 0;
        tri[1] = 2 * i + 1;
        tri[2] = 2 * i + 2;

        tri[3] = 2 * i + 1;
        tri[4] = 2 * i + 3;
        tri[5] = 2 * i + 2;
    }

    //cap triangles
    for(unsigned short i = 0; i < 30; i++) {
        unsigned short* tri = &triangle_data[32 * 2 * 3 + 2 * 3 * i];

        tri[0] = 33 * 2;
        tri[1] = 33 * 2 + 2 * i + 2;
        tri[2] = 33 * 2 + 2 * i + 4;

        tri[3] = 33 * 2 + 1;
        tri[4] = 33 * 2 + 2 * i + 5;
        tri[5] = 33 * 2 + 2 * i + 3;
    }

    return hfe_mesh_create_indexed_fff(vertex_data, 33 * 2 + 32 * 2, triangle_data, 32 * 2 + 30 * 2, 3, 2, 3);
}

hfe_mesh hfe_mesh_create_primitive_plane(float size_x, float size_z) {
    float hx = size_x / 2.f;
    float hz = size_z / 2.f;
    float vertex_data[] = {
        //position    //uv       //normal
        -hx, 0.f,  hz,  0.f, 0.f,  0.f, 1.f, 0.f,
         hx, 0.f,  hz,  1.f, 0.f,  0.f, 1.f, 0.f,
         hx, 0.f, -hz,  1.f, 1.f,  0.f, 1.f, 0.f,
        -hx, 0.f, -hz,  0.f, 1.f,  0.f, 1.f, 0.f,
    };

    return hfe_mesh_create_fff(vertex_data, 4, 3, 2, 3);
}

hfe_mesh hfe_mesh_create_primitive_pyramid(float base_size, float height) {
    float h = base_size / 2.f;
    float vertex_data[] = {
         -h,    0.f,   h,  0.f, 0.f,  0.f, 0.f, 0.f,
          h,    0.f,   h,  1.f, 0.f,  0.f, 0.f, 0.f,
        0.f, height, 0.f,  .5f, .5f,  0.f, 0.f, 0.f,

          h,    0.f,   h,  1.f, 0.f,  0.f, 0.f, 0.f,
          h,    0.f,  -h,  1.f, 1.f,  0.f, 0.f, 0.f,
        0.f, height, 0.f,  .5f, .5f,  0.f, 0.f, 0.f,

          h,    0.f,  -h,  1.f, 1.f,  0.f, 0.f, 0.f,
         -h,    0.f,  -h,  0.f, 1.f,  0.f, 0.f, 0.f,
        0.f, height, 0.f,  .5f, .5f,  0.f, 0.f, 0.f,

         -h,    0.f,  -h,  0.f, 1.f,  0.f, 0.f, 0.f,
         -h,    0.f,   h,  0.f, 0.f,  0.f, 0.f, 0.f,
        0.f, height, 0.f,  .5f, .5f,  0.f, 0.f, 0.f,

         -h,    0.f,  -h,  0.f, 0.f,  0.f, 0.f, 0.f,
          h,    0.f,  -h,  1.f, 0.f,  0.f, 0.f, 0.f,
          h,    0.f,   h,  1.f, 1.f,  0.f, 0.f, 0.f,
         -h,    0.f,   h,  0.f, 1.f,  0.f, 0.f, 0.f,
    };
    unsigned short vertex_count = sizeof(vertex_data) / sizeof(vertex_data[0]) / 8;

    unsigned short triangle_data[] = {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        9, 10, 11,
        12, 13, 14,
        12, 14, 15,
    };
    unsigned short triangle_count = sizeof(triangle_data) / sizeof(triangle_data[0]) / 3;

    hfe_util_calculate_normals(vertex_data, 8, &vertex_data[5], 8, triangle_data, triangle_count);
    return hfe_mesh_create_indexed_fff(vertex_data, vertex_count, triangle_data, triangle_count, 3, 2, 3);
}

hfe_mesh hfe_mesh_create_primitive_quad(float size) {
    return hfe_mesh_create_primitive_plane(size, size);
}

hfe_mesh hfe_mesh_create_primitive_quad_ui(void) {
    float vertex_data[] = {
        //position //uv
        0.f, 0.f,  0.f, 0.f,
        1.f, 0.f,  1.f, 0.f,
        1.f, 1.f,  1.f, 1.f,
        0.f, 1.f,  0.f, 1.f,
    };

    return hfe_mesh_create_ff(vertex_data, 4, 2, 2);
}

hfe_mesh hfe_mesh_create_primitive_rectangular_prism(float size_x, float size_y, float size_z) {
    float hx = size_x / 2.f;
    float hy = size_y / 2.f;
    float hz = size_z / 2.f;
    float vertex_data[] = {
        //position      //uv       //normal
        -hx, -hy,  hz,  0.f, 0.f,  0.f, 0.f, 1.f,
         hx, -hy,  hz,  1.f, 0.f,  0.f, 0.f, 1.f,
         hx,  hy,  hz,  1.f, 1.f,  0.f, 0.f, 1.f,
        -hx,  hy,  hz,  0.f, 1.f,  0.f, 0.f, 1.f,

         hx, -hy,  hz,  0.f, 0.f,  1.f, 0.f, 0.f,
         hx, -hy, -hz,  1.f, 0.f,  1.f, 0.f, 0.f,
         hx,  hy, -hz,  1.f, 1.f,  1.f, 0.f, 0.f,
         hx,  hy,  hz,  0.f, 1.f,  1.f, 0.f, 0.f,

         hx, -hy, -hz,  0.f, 0.f,  0.f, 0.f, -1.f,
        -hx, -hy, -hz,  1.f, 0.f,  0.f, 0.f, -1.f,
        -hx,  hy, -hz,  1.f, 1.f,  0.f, 0.f, -1.f,
         hx,  hy, -hz,  0.f, 1.f,  0.f, 0.f, -1.f,

        -hx, -hy, -hz,  0.f, 0.f,  -1.f, 0.f, 0.f,
        -hx, -hy,  hz,  1.f, 0.f,  -1.f, 0.f, 0.f,
        -hx,  hy,  hz,  1.f, 1.f,  -1.f, 0.f, 0.f,
        -hx,  hy, -hz,  0.f, 1.f,  -1.f, 0.f, 0.f,

        -hx,  hy,  hz,  0.f, 0.f,  0.f, 1.f, 0.f,
         hx,  hy,  hz,  1.f, 0.f,  0.f, 1.f, 0.f,
         hx,  hy, -hz,  1.f, 1.f,  0.f, 1.f, 0.f,
        -hx,  hy, -hz,  0.f, 1.f,  0.f, 1.f, 0.f,

         hx, -hy,  hz,  0.f, 0.f,  0.f, -1.f, 0.f,
        -hx, -hy,  hz,  1.f, 0.f,  0.f, -1.f, 0.f,
        -hx, -hy, -hz,  1.f, 1.f,  0.f, -1.f, 0.f,
         hx, -hy, -hz,  0.f, 1.f,  0.f, -1.f, 0.f,
    };

    unsigned short triangle_data[] = {
        0, 1, 2,
        0, 2, 3,

        4, 5, 6,
        4, 6, 7,

        8, 9, 10,
        8, 10, 11,

        12, 13, 14,
        12, 14, 15,

        16, 17, 18,
        16, 18, 19,

        20, 21, 22,
        20, 22, 23,
    };

    return hfe_mesh_create_indexed_fff(vertex_data, 24, triangle_data, 12, 3, 2, 3);
}

hfe_mesh hfe_mesh_create_from_file_obj(const char* path) {
    hfe_mesh mesh = { 0 };

    FILE* file = fopen(path, "rb");
    if(!file) {
        goto error_file;
    }

    hfe_dynarray all_positions;
    hfe_dynarray_init(&all_positions, sizeof(float), 3 * 32);
    hfe_dynarray all_normals;
    hfe_dynarray_init(&all_normals, sizeof(float), 3 * 32);
    hfe_dynarray all_uvs;
    hfe_dynarray_init(&all_uvs, sizeof(float), 2 * 32);
    // TODO:

    char buffer[256];
    while(hfe_file_read_line(file, buffer, 256)) {
        if(hfe_string_starts_with(buffer, "v ")) {//found position
            const char* ptr = buffer;

            for(int i = 0; i < 3; i++) {
                float value;
                ptr = hfe_string_find_float(ptr, &value);
                if(!hfe_dynarray_push(&all_positions, &value)) {
                    goto error_push;
                }
            }
        }
        else if(hfe_string_starts_with(buffer, "vn ")) {//found normal
            const char* ptr = buffer;

            for(int i = 0; i < 3; i++) {
                float value;
                ptr = hfe_string_find_float(ptr, &value);
                if(!hfe_dynarray_push(&all_normals, &value)) {
                    goto error_push;
                }
            }
        }
        else if(hfe_string_starts_with(buffer, "vt ")) {//found uv
            const char* ptr = buffer;

            for(int i = 0; i < 2; i++) {
                float value;
                ptr = hfe_string_find_float(ptr, &value);
                if(!hfe_dynarray_push(&all_uvs, &value)) {
                    goto error_push;
                }
            }
        }
    }

    if(all_positions.count == 0) {//ensure at least one position
        for(int i = 0; i < 3; i++) {
            if(!hfe_dynarray_push(&all_positions, (float[]){ 0 })) {
                goto error_push;
            }
        }
    }
    if(all_normals.count == 0) {//ensure at least one normal
        for(int i = 0; i < 3; i++) {
            if(!hfe_dynarray_push(&all_normals, (float[]){ 0 })) {
                goto error_push;
            }
        }
    }
    if(all_uvs.count == 0) {//ensure at least one uv
        for(int i = 0; i < 2; i++) {
            if(!hfe_dynarray_push(&all_uvs, (float[]){ 0 })) {
                goto error_push;
            }
        }
    }

    fseek(file, 0, SEEK_SET);

    hfe_dynarray verts;
    hfe_dynarray_init(&verts, sizeof(float), 8 * 32);
    hfe_dynarray tris;
    hfe_dynarray_init(&tris, sizeof(unsigned short), 3 * 32);
    while(hfe_file_read_line(file, buffer, 256)) {
        if(hfe_string_starts_with(buffer, "f ")) {//found a face
            const char* ptr = buffer;
            size_t count = 0;
            size_t indices_position[3];
            size_t indices_uv[3];
            size_t indices_normal[3];
            while(true) {
                if(!ptr) {
                    break;
                }
                unsigned long long pos = 1;
                ptr = hfe_string_find_ull(ptr, &pos);//read position index
                pos--;
                if(!ptr) {
                    break;
                }

                unsigned long long uv = 1;
                if(*ptr=='/' && ptr[1] != '/') {
                    ptr = hfe_string_find_ull(ptr, &uv);
                }
                uv--;
                if(!ptr) {
                    break;
                }

                unsigned long long nor = 1;
                if(ptr[0] == '/') {
                    ptr = hfe_string_find_ull(ptr, &nor);
                }
                nor--;
                if(!ptr) {
                    break;
                }

                if(count <= 2) {
                    indices_position[count] = pos;
                    indices_uv[count] = uv;
                    indices_normal[count] = nor;
                }
                else {//additional item, must do fan
                    indices_position[1] = indices_position[2];
                    indices_uv[1] = indices_uv[2];
                    indices_normal[1] = indices_normal[2];

                    indices_position[2] = pos;
                    indices_uv[2] = uv;
                    indices_normal[2] = nor;
                }
                count++;

                for(size_t i = 0; i < 3; i++) {
                    if(!hfe_dynarray_push(&verts, hfe_dynarray_get(&all_positions, 3 * pos + i))) {
                        goto error_push2;
                    }
                }
                for(size_t i = 0; i < 2; i++) {
                    if(!hfe_dynarray_push(&verts, hfe_dynarray_get(&all_uvs, 2 * uv + i))) {
                        goto error_push2;
                    }
                }
                for(size_t i = 0; i < 3; i++) {
                    if(!hfe_dynarray_push(&verts, hfe_dynarray_get(&all_normals, 3 * nor + i))) {
                        goto error_push2;
                    }
                }

                if(count >= 3) {//add triangle data
                    unsigned short vert_count = (unsigned short)(verts.count / 8);
                    if(!hfe_dynarray_push(&tris, (unsigned short[]){ vert_count - (unsigned short)count })) {
                        goto error_push2;
                    }
                    if(!hfe_dynarray_push(&tris, (unsigned short[]){ vert_count - 2 })) {
                        goto error_push2;
                    }
                    if(!hfe_dynarray_push(&tris, (unsigned short[]){ vert_count - 1 })) {
                        goto error_push2;
                    }
                }
            }
        }
    }
    mesh = hfe_mesh_create_indexed_fff(verts.data, (unsigned short)(verts.count / 8), tris.data, (unsigned short)(tris.count / 3), 3, 2, 3);

    error_push2:
        hfe_dynarray_deinit(&verts);
        hfe_dynarray_deinit(&tris);
    error_push:
        hfe_dynarray_deinit(&all_positions);
        hfe_dynarray_deinit(&all_normals);
        hfe_dynarray_deinit(&all_uvs);
    error_file:
        return mesh;
}

void hfe_mesh_destroy(hfe_mesh mesh) {
    internal_hfe_api_ptrs.mesh_destroy(mesh);
}

static hfe_mesh internal_hfe_active_mesh = { 0 };
void hfe_mesh_use(hfe_mesh mesh) {
    internal_hfe_api_ptrs.mesh_use(mesh);
}

void hfe_mesh_reset(void) {
    internal_hfe_api_ptrs.mesh_reset();
}

void hfe_mesh_draw(void) {
    internal_hfe_api_ptrs.mesh_draw();
}

void hfe_mesh_vertex_spec_set(hfe_mesh mesh, size_t index, hfe_vertex_spec spec, size_t stride, size_t offset) {
    internal_hfe_api_ptrs.mesh_vertex_spec_set(mesh, index, spec, stride, offset);
}

void hfe_mesh_vertex_specs_set(hfe_mesh mesh, hfe_vertex_spec* specs, size_t count) {
    size_t stride = 0;
    for(size_t i = 0; i < count; i++) {
        stride += internal_hfe_vertex_spec_sizeof(specs[i]);
    }
    size_t offset = 0;
    for(size_t i = 0; i < count; i++) {
        hfe_mesh_vertex_spec_set(mesh, i, specs[i], stride, offset);
        offset += internal_hfe_vertex_spec_sizeof(specs[i]);
    }
}

//TEXTURE
hfe_texture hfe_texture_create(int w, int h, hfe_texture_pixel_type pixel_type, hfe_texture_pixel_format data_format, hfe_texture_configuration configuration) {
    hfe_texture texture = internal_hfe_api_ptrs.hfe_texture_create(w, h, pixel_type, data_format, configuration, NULL);
    if(!hfe_texture_valid(texture)) {
        return (hfe_texture) { 0 };
    }
    return texture;
}

hfe_texture hfe_texture_create_from_file(const char* path, hfe_texture_configuration configuration) {
    stbi_set_flip_vertically_on_load(true);
    int w, h, channels;
    unsigned char* image_data = stbi_load(path, &w, &h, &channels, STBI_rgb_alpha);
    if(!image_data) {
        return (hfe_texture) { 0 };
    }
    return hfe_texture_create_from_bytes(image_data, w, h, hfe_texture_pixel_type_unsigned_byte, hfe_texture_pixel_format_rgba, configuration);
}

hfe_texture hfe_texture_create_from_bytes(unsigned char* data, int w, int h, hfe_texture_pixel_type pixel_type, hfe_texture_pixel_format data_format, hfe_texture_configuration configuration) {
    hfe_texture texture = internal_hfe_api_ptrs.hfe_texture_create(w, h, pixel_type, data_format, configuration, data);
    if(!hfe_texture_valid(texture)) {
        return (hfe_texture) { 0 };
    }
    return texture;
}

void hfe_texture_destroy(hfe_texture texture) {
    internal_hfe_api_ptrs.hfe_texture_destroy(texture);
}

void hfe_texture_use(hfe_texture texture, size_t unit) {
    internal_hfe_api_ptrs.hfe_texture_use(texture, unit);
}

bool hfe_texture_valid(hfe_texture texture) {
    return internal_hfe_api_ptrs.hfe_texture_valid(texture);
}


//DEPTH BUFFER
hfe_depth_buffer hfe_depth_buffer_create(int w, int h) {
    return internal_hfe_api_ptrs.hfe_depth_buffer_create(w, h);
}

void hfe_depth_buffer_destroy(hfe_depth_buffer depth_buffer) {
    internal_hfe_api_ptrs.hfe_depth_buffer_destroy(depth_buffer);
}

bool hfe_depth_buffer_valid(hfe_depth_buffer depth_buffer) {
    return internal_hfe_api_ptrs.hfe_depth_buffer_valid(depth_buffer);
}

//RENDER TARGET
hfe_render_target hfe_render_target_create_color(int w, int h, hfe_texture_configuration texture_configuration) {
    return internal_hfe_api_ptrs.hfe_render_target_create_color(w, h, texture_configuration);
}

hfe_render_target hfe_render_target_create_depth(int w, int h, hfe_texture_configuration texture_configuration) {
    return internal_hfe_api_ptrs.hfe_render_target_create_depth(w, h, texture_configuration);
}

void hfe_render_target_destroy(hfe_render_target render_target) {
    internal_hfe_api_ptrs.hfe_render_target_destroy(render_target);
}

bool hfe_render_target_valid(hfe_render_target render_target) {
    return internal_hfe_api_ptrs.hfe_render_target_valid(render_target);
}

void hfe_render_target_use(hfe_render_target render_target) {
    internal_hfe_api_ptrs.hfe_render_target_use(render_target);
}

void hfe_render_target_reset(void) {
    internal_hfe_api_ptrs.hfe_render_target_reset();
}


//UTIL
void hfe_util_calculate_normals(float* vertices, size_t vertices_stride, float* normals, size_t normals_stride, unsigned short* triangles, unsigned short triangle_count) {
    for(unsigned short i = 0; i < triangle_count; i++) {
        float* vert0 = &vertices[triangles[i * 3 + 0] * vertices_stride];
        float* vert1 = &vertices[triangles[i * 3 + 1] * vertices_stride];
        float* vert2 = &vertices[triangles[i * 3 + 2] * vertices_stride];

        float edge0[3] = { vert1[0] - vert0[0], vert1[1] - vert0[1], vert1[2] - vert0[2] };
        float edge1[3] = { vert2[0] - vert0[0], vert2[1] - vert0[1], vert2[2] - vert0[2] };

        float cross[3] = {
            edge0[1] * edge1[2] - edge0[2] * edge1[1],
            edge0[2] * edge1[0] - edge0[0] * edge1[2],
            edge0[0] * edge1[1] - edge0[1] * edge1[0],
        };
        float len = sqrtf(cross[0] * cross[0] + cross[1] * cross[1] + cross[2] * cross[2]);

        float normal[3] = {
            normal[0] = cross[0] / len,
            normal[1] = cross[1] / len,
            normal[2] = cross[2] / len,
        };

        float* normal0 = &normals[triangles[i * 3 + 0] * normals_stride];
        float* normal1 = &normals[triangles[i * 3 + 1] * normals_stride];
        float* normal2 = &normals[triangles[i * 3 + 2] * normals_stride];
        memcpy(normal0, normal, sizeof(float) * 3);
        memcpy(normal1, normal, sizeof(float) * 3);
        memcpy(normal2, normal, sizeof(float) * 3);
    }
}

// OPENGL BACKEND
#if 1
static hfe_shader hfe_opengl_shader_create_from_string(hfe_shader_type type, const char* source) {
    GLenum gl_type;
    switch(type) {
        case hfe_shader_type_vertex:
            gl_type = GL_VERTEX_SHADER;
            break;
        case hfe_shader_type_fragment:
            gl_type = GL_FRAGMENT_SHADER;
            break;
    }
    GLuint gl_shader = glCreateShader(gl_type);
    if(gl_shader) {
        glShaderSource(gl_shader, 1, &source, NULL);
        glCompileShader(gl_shader);
        GLint success;
        glGetShaderiv(gl_shader, GL_COMPILE_STATUS, &success);
        if(success == GL_FALSE) {
            glGetShaderInfoLog(gl_shader, HFE_ERROR_LOG_LEN, NULL, hfe_error_log);
            glDeleteShader(gl_shader);
            return (hfe_shader) { 0 };
        }

        return (hfe_shader) { .id = gl_shader };
    }
    return (hfe_shader) { 0 };
}

static void hfe_opengl_shader_destroy(hfe_shader shader) {
    glDeleteShader(shader.id);
}

static bool hfe_opengl_shader_valid(hfe_shader shader) {
    return shader.id;
}


static hfe_shader_program hfe_opengl_shader_program_create(hfe_shader* shaders, size_t shaders_count) {
    for(size_t i = 0; i < shaders_count; i++) {
        if(!hfe_shader_valid(shaders[i])) {
            return (hfe_shader_program) { 0 };
        }
    }

    GLuint gl_program = glCreateProgram();
    if(gl_program) {
        for(size_t i = 0; i < shaders_count; i++) {
            glAttachShader(gl_program, shaders[i].id);
        }
        glLinkProgram(gl_program);
        GLint success;
        glGetProgramiv(gl_program, GL_LINK_STATUS, &success);
        if(success == GL_FALSE) {
            glGetProgramInfoLog(gl_program, HFE_ERROR_LOG_LEN, NULL, hfe_error_log);
            glDeleteProgram(gl_program);
            return (hfe_shader_program) { 0 };
        }
        return (hfe_shader_program) { .id = gl_program };
    }
    return (hfe_shader_program) { 0 };
}

static void hfe_opengl_shader_program_destroy(hfe_shader_program program) {
    glDeleteProgram(program.id);
}

static void hfe_opengl_shader_program_use(hfe_shader_program program) {
    hfe_active_program = program;
    glUseProgram(program.id);
}


static hfe_shader_property hfe_opengl_shader_property_get(const char* name) {
    return (hfe_shader_property) { .id = glGetUniformLocation(hfe_active_program.id, name) };
}

static void hfe_opengl_shader_property_set_1i(hfe_shader_property property, int x) {
    glUniform1i(property.id, x);
}

static void hfe_opengl_shader_property_set_2i(hfe_shader_property property, int x, int y) {
    glUniform2i(property.id, x, y);
}

static void hfe_opengl_shader_property_set_3i(hfe_shader_property property, int x, int y, int z) {
    glUniform3i(property.id, x, y, z);
}

static void hfe_opengl_shader_property_set_4i(hfe_shader_property property, int x, int y, int z, int w) {
    glUniform4i(property.id, x, y, z, w);
}

static void hfe_opengl_shader_property_set_1f(hfe_shader_property property, float x) {
    glUniform1f(property.id, x);
}

static void hfe_opengl_shader_property_set_2f(hfe_shader_property property, float x, float y) {
    glUniform2f(property.id, x, y);
}

static void hfe_opengl_shader_property_set_3f(hfe_shader_property property, float x, float y, float z) {
    glUniform3f(property.id, x, y, z);
}

static void hfe_opengl_shader_property_set_4f(hfe_shader_property property, float x, float y, float z, float w) {
    glUniform4f(property.id, x, y, z, w);
}

static void hfe_opengl_shader_property_set_mat3f(hfe_shader_property property, float* value) {
    glUniformMatrix3fv(property.id, 1, GL_FALSE, value);
}

static void hfe_opengl_shader_property_set_mat4f(hfe_shader_property property, float* value) {
    glUniformMatrix4fv(property.id, 1, GL_FALSE, value);
}

static bool hfe_opengl_shader_property_valid(hfe_shader_property property) {
    return property.id >= 0;
}


static hfe_mesh hfe_opengl_mesh_create_indexed(void* data, size_t size, unsigned short vertex_count, unsigned short* triangle_data, unsigned short triangle_count) {
    hfe_mesh mesh;
    glCreateVertexArrays(1, &mesh.id);
    if(!mesh.id) {
        internal_hfe_error_set("error creating mesh: could not create gl vertex array object");
        return (hfe_mesh){ 0 };
    }

    if(triangle_data && triangle_count) {
        glGenBuffers(2, mesh.buffers);
        if(!mesh.buffers[0] || !mesh.buffers[1]) {
            glDeleteBuffers(2, mesh.buffers);
            glDeleteVertexArrays(1, &mesh.id);
            internal_hfe_error_set("error creating mesh: could not create gl buffers");
            return (hfe_mesh){ 0 };
        }
    }
    else {
        mesh.buffers[1] = 0;
        glGenBuffers(1, mesh.buffers);
        if(!mesh.buffers[0]) {
            glDeleteBuffers(1, mesh.buffers);
            glDeleteVertexArrays(1, &mesh.id);
            internal_hfe_error_set("error creating mesh: could not create gl buffer");
            return (hfe_mesh){ 0 };
        }
    }

    GLint prev_vertex;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &prev_vertex);
    GLint prev_array;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &prev_array);
    GLint prev_element_array;
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &prev_element_array);

    glBindVertexArray(mesh.id);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)size, data, GL_STATIC_DRAW);
    if(mesh.buffers[1]) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.buffers[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(triangle_count * 3 * sizeof(unsigned short)), triangle_data, GL_STATIC_DRAW);
    }

    glBindVertexArray((GLuint)prev_vertex);
    glBindBuffer(GL_ARRAY_BUFFER, (GLuint)prev_array);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint)prev_element_array);

    mesh.count = mesh.buffers[1] ? triangle_count * 3 : vertex_count;
    return mesh;
}

static void hfe_opengl_mesh_destroy(hfe_mesh mesh) {
    glDeleteBuffers(mesh.buffers[1] ? 2 : 1, mesh.buffers);
    glDeleteVertexArrays(1, &mesh.id);
}

static void hfe_opengl_mesh_use(hfe_mesh mesh) {
    internal_hfe_active_mesh = mesh;
    glBindVertexArray(mesh.id);
}

static void hfe_opengl_mesh_reset(void) {
    internal_hfe_active_mesh = (hfe_mesh) { 0 };
    glBindVertexArray(0);
}

static void hfe_opengl_mesh_draw(void) {
    switch(internal_hfe_active_mesh.buffers[1]) {
        case 0:
            glDrawArrays(GL_TRIANGLE_FAN, 0, internal_hfe_active_mesh.count);
            break;
        default:
            glDrawElements(GL_TRIANGLES, internal_hfe_active_mesh.count, GL_UNSIGNED_SHORT, NULL);
            break;
    }
}

static void hfe_opengl_mesh_vertex_spec_set(hfe_mesh mesh, size_t index, hfe_vertex_spec spec, size_t stride, size_t offset) {
    GLint prev_vertex;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &prev_vertex);
    GLint prev_array;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &prev_array);

    glBindVertexArray(mesh.id);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[0]);

    glEnableVertexAttribArray((GLuint)index);
    glVertexAttribPointer((GLuint)index, (GLint)spec.width, (GLenum)spec.type, GL_FALSE, (GLsizei)stride, (void*)offset);

    glBindVertexArray((GLuint)prev_vertex);
    glBindBuffer(GL_ARRAY_BUFFER, (GLuint)prev_array);
}


static hfe_texture hfe_opengl_texture_create(int w, int h, hfe_texture_pixel_type pixel_type, hfe_texture_pixel_format data_format, hfe_texture_configuration configuration, unsigned char* data) {
    GLuint texture;
    glGenTextures(1, &texture);
    if(!texture) {
        return (hfe_texture) { 0 };
    }

    GLint prev;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &prev);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)configuration.wrap_x);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)configuration.wrap_y);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)configuration.filter_min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)configuration.filter_mag);

    glTexImage2D(GL_TEXTURE_2D, 0, (GLint)data_format, w, h, 0, (GLenum)data_format, (GLenum)pixel_type, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, (GLuint)prev);

    return (hfe_texture) { .id = texture };
}

static void hfe_opengl_texture_destroy(hfe_texture texture) {
    glDeleteTextures(1, &texture.id);
}

static void hfe_opengl_texture_use(hfe_texture texture, size_t unit) {
    glActiveTexture((GLenum)((size_t)GL_TEXTURE0 + unit));
    glBindTexture(GL_TEXTURE_2D, texture.id);
}

static bool hfe_opengl_texture_valid(hfe_texture texture) {
    return texture.id != 0;
}


static hfe_depth_buffer hfe_opengl_depth_buffer_create(int w, int h) {
    hfe_depth_buffer new_depth_buffer;
    glGenRenderbuffers(1, &new_depth_buffer.id);

    GLint prev;
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &prev);
    glBindRenderbuffer(GL_RENDERBUFFER, new_depth_buffer.id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, w, h);
    glBindRenderbuffer(GL_RENDERBUFFER, (GLuint)prev);

    return new_depth_buffer;
}

static void hfe_opengl_depth_buffer_destroy(hfe_depth_buffer depth_buffer) {
    glDeleteFramebuffers(1, &depth_buffer.id);
}

static bool hfe_opengl_depth_buffer_valid(hfe_depth_buffer depth_buffer) {
    return depth_buffer.id != 0;
}


static hfe_render_target hfe_opengl_render_target_create_color(int w, int h, hfe_texture_configuration texture_configuration) {
    hfe_render_target new_render_target;
    new_render_target.texture = hfe_texture_create(w, h, hfe_texture_pixel_type_unsigned_byte, hfe_texture_pixel_format_rgba, texture_configuration);
    if(!hfe_texture_valid(new_render_target.texture)) {
        goto error_texture;
    }
    new_render_target.depth_buffer = hfe_depth_buffer_create(w, h);
    if(!hfe_depth_buffer_valid(new_render_target.depth_buffer)) {
        goto error_depth;
    }

    glGenFramebuffers(1, &new_render_target.id);
    if(new_render_target.id == 0) {
        goto error_frame;
    }

    GLint prev;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prev);
    glBindFramebuffer(GL_FRAMEBUFFER, new_render_target.id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, new_render_target.texture.id, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, new_render_target.depth_buffer.id);
    glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)prev);
    return new_render_target;

    error_frame:
        hfe_depth_buffer_destroy(new_render_target.depth_buffer);
    error_depth:
        hfe_texture_destroy(new_render_target.texture);
    error_texture:
        return (hfe_render_target) { .id = 0 };
}

static hfe_render_target hfe_opengl_render_target_create_depth(int w, int h, hfe_texture_configuration texture_configuration) {
    hfe_render_target new_render_target;
    new_render_target.texture = hfe_texture_create(w, h, hfe_texture_pixel_type_float, hfe_texture_pixel_format_depth, texture_configuration);
    if(!hfe_texture_valid(new_render_target.texture)) {
        goto error_texture;
    }
    new_render_target.depth_buffer = (hfe_depth_buffer) { .id = 0 };

    glGenFramebuffers(1, &new_render_target.id);
    if(new_render_target.id == 0) {
        goto error_frame;
    }

    GLint prev;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prev);
    glBindFramebuffer(GL_FRAMEBUFFER, new_render_target.id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, new_render_target.texture.id, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)prev);
    return new_render_target;

    error_frame:
        hfe_texture_destroy(new_render_target.texture);
    error_texture:
        return (hfe_render_target) { .id = 0 };
}

static void hfe_opengl_render_target_destroy(hfe_render_target render_target) {
    hfe_texture_destroy(render_target.texture);
    hfe_depth_buffer_destroy(render_target.depth_buffer);
    glDeleteFramebuffers(1, &render_target.id);
}

static bool hfe_opengl_render_target_valid(hfe_render_target render_target) {
    return render_target.id != 0;
}

static void hfe_opengl_render_target_use(hfe_render_target render_target) {
    glBindFramebuffer(GL_FRAMEBUFFER, render_target.id);
}

static void hfe_opengl_render_target_reset(void) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


bool hfe_init_opengl(void) {
    gladLoadGL();
    internal_hfe_api_ptrs = (hfe_api_ptrs) {
        .shader_create_from_string = hfe_opengl_shader_create_from_string,
        .shader_destroy = hfe_opengl_shader_destroy,
        .shader_valid = hfe_opengl_shader_valid,

        .shader_program_create = hfe_opengl_shader_program_create,
        .shader_program_destroy = hfe_opengl_shader_program_destroy,
        .shader_program_use = hfe_opengl_shader_program_use,

        .shader_property_get = hfe_opengl_shader_property_get,
        .shader_property_set_1i = hfe_opengl_shader_property_set_1i,
        .shader_property_set_2i = hfe_opengl_shader_property_set_2i,
        .shader_property_set_3i = hfe_opengl_shader_property_set_3i,
        .shader_property_set_4i = hfe_opengl_shader_property_set_4i,
        .shader_property_set_1f = hfe_opengl_shader_property_set_1f,
        .shader_property_set_2f = hfe_opengl_shader_property_set_2f,
        .shader_property_set_3f = hfe_opengl_shader_property_set_3f,
        .shader_property_set_4f = hfe_opengl_shader_property_set_4f,
        .shader_property_set_mat3f = hfe_opengl_shader_property_set_mat3f,
        .shader_property_set_mat4f = hfe_opengl_shader_property_set_mat4f,
        .shader_property_valid = hfe_opengl_shader_property_valid,

        .mesh_create_indexed = hfe_opengl_mesh_create_indexed,
        .mesh_destroy = hfe_opengl_mesh_destroy,
        .mesh_use = hfe_opengl_mesh_use,
        .mesh_reset = hfe_opengl_mesh_reset,
        .mesh_draw = hfe_opengl_mesh_draw,
        .mesh_vertex_spec_set = hfe_opengl_mesh_vertex_spec_set,

        .hfe_texture_create = hfe_opengl_texture_create,
        .hfe_texture_destroy = hfe_opengl_texture_destroy,
        .hfe_texture_use = hfe_opengl_texture_use,
        .hfe_texture_valid = hfe_opengl_texture_valid,

        .hfe_depth_buffer_create = hfe_opengl_depth_buffer_create,
        .hfe_depth_buffer_destroy = hfe_opengl_depth_buffer_destroy,
        .hfe_depth_buffer_valid = hfe_opengl_depth_buffer_valid,

        .hfe_render_target_create_color = hfe_opengl_render_target_create_color,
        .hfe_render_target_create_depth = hfe_opengl_render_target_create_depth,
        .hfe_render_target_destroy = hfe_opengl_render_target_destroy,
        .hfe_render_target_valid = hfe_opengl_render_target_valid,
        .hfe_render_target_use = hfe_opengl_render_target_use,
        .hfe_render_target_reset = hfe_opengl_render_target_reset,
    };
    return true;
}
#endif
