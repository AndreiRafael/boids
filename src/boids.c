#include "boids.h"

#include <math.h>

#include "hf_lib/hf_transform.h"

#define BOIDS_MAX_NEIGHBORS 50

static struct {
    float min_x;
    float min_y;
    float max_x;
    float max_y;
} bounds;
static float max_speed = 5.f;

void boids_set_bounds(float min_x, float min_y, float max_x, float max_y) {
    bounds.min_x = min_x;
    bounds.min_y = min_y;
    bounds.max_x = max_x;
    bounds.max_y = max_y;
}

void boids_set_max_speed(float speed) {
    max_speed = speed;
}

static void boid_get_neighbors(boid* b, boid* boids, size_t boids_count, float radius, boid* out_neighbors, size_t* out_neighbors_count) {
    *out_neighbors_count = 0;
    for(size_t i = 0; i < boids_count; i++) {
        if(*out_neighbors_count >= BOIDS_MAX_NEIGHBORS) {
            break;
        }

        boid* other = &boids[i];
        if(b == other) {
            continue;
        }

        float dist_sqr = hf_vec2f_square_distance(b->position, other->position);
        if(dist_sqr < (radius * radius)) {
            out_neighbors[(*out_neighbors_count)++] = *other;
        }
    }
}

static void separation(boid* b, boid* boids, size_t boids_count, hf_vec2f out_vec) {
    boid neighbors[BOIDS_MAX_NEIGHBORS];
    size_t neighbors_count;
    boid_get_neighbors(b, boids, boids_count, 3.f, neighbors, &neighbors_count);

    for(size_t i = 0; i < neighbors_count; i++) {
        boid* other = &neighbors[i];
        hf_vec2f from_other;
        hf_vec2f_subtract(b->position, other->position, from_other);
        hf_vec2f_normalize(from_other, from_other);
        hf_vec2f_add(out_vec, from_other, out_vec);
    }

    if(neighbors_count) {
        hf_vec2f_divide(out_vec, (float)neighbors_count, out_vec);
    }
}

static void alignment(boid* b, boid* boids, size_t boids_count, hf_vec2f out_vec) {
    boid neighbors[BOIDS_MAX_NEIGHBORS];
    size_t neighbors_count;
    boid_get_neighbors(b, boids, boids_count, 7.f, neighbors, &neighbors_count);

    size_t c = 0;
    for(size_t i = 0; i < neighbors_count; i++) {
        boid* other = &neighbors[i];
        if(other->id == b->id) {
            hf_vec2f norm;
            hf_vec2f_normalize(other->velocity, norm);
            hf_vec2f_add(out_vec, norm, out_vec);

            c++;
        }
    }

    if(c) {
        hf_vec2f_divide(out_vec, (float)c, out_vec);
    }
    else {
        hf_vec2f_normalize(b->velocity, out_vec);
    }
}

static void cohesion(boid* b, boid* boids, size_t boids_count, hf_vec2f out_vec) {
    boid neighbors[BOIDS_MAX_NEIGHBORS];
    size_t neighbors_count;
    boid_get_neighbors(b, boids, boids_count, 7.f, neighbors, &neighbors_count);

    hf_vec2f mid;
    size_t c = 0;
    for(size_t i = 0; i < neighbors_count; i++) {
        boid* other = &neighbors[i];
        if(other->id == b->id) {
            hf_vec2f_add(mid, other->position, mid);
            c++;
        }
    }

    if(c) {
        hf_vec2f_divide(mid, (float)c, mid);
    }
    else {
        hf_vec2f_copy(b->position, mid);
    }

    hf_vec2f_subtract(mid, b->position, out_vec);
}

static void apply_func(boid* b, boid* boids, size_t boids_count, void(*func)(boid*, boid*, size_t, hf_vec2f), float intensity) {
    hf_vec2f res = { 0 };
    func(b, boids, boids_count, res);
    hf_vec2f_multiply(res, intensity, res);
    hf_vec2f_add(b->acceleration, res, b->acceleration);
}

void boids_update(boid* boids, size_t boids_count, float delta) {
    for(size_t i = 0; i < boids_count; i++) {
        boid* b = &boids[i];

        apply_func(b, boids, boids_count, separation, 4.f);
        apply_func(b, boids, boids_count, alignment, .8f);
        apply_func(b, boids, boids_count, cohesion, 0.5f);

        hf_vec2f delta_acc;
        hf_vec2f_multiply(b->acceleration, delta * 2.f, delta_acc);
        hf_vec2f_add(b->velocity, delta_acc, b->velocity);

        if(hf_vec2f_square_magnitude(b->velocity) > (max_speed * max_speed)) {
            hf_vec2f_normalize(b->velocity, b->velocity);
            hf_vec2f_multiply(b->velocity, max_speed, b->velocity);
        }

        hf_vec2f movement;
        hf_vec2f_multiply(b->velocity, delta, movement);
        hf_vec2f_add(b->position, movement, b->position);

        float bounds_width = bounds.max_x - bounds.min_x;
        float bounds_height = bounds.max_y - bounds.min_y;
        if(b->position[0] > bounds.max_x) {
            b->position[0] -= bounds_width;
        }
        else if(b->position[0] < bounds.min_x) {
            b->position[0] += bounds_width;
        }
        if(b->position[1] > bounds.max_y) {
            b->position[1] -= bounds_height;
        }
        else if(b->position[1] < bounds.min_y) {
            b->position[1] += bounds_height;
        }

        //reset acceleration
        hf_vec2f_copy((hf_vec2f) { 0 }, b->acceleration);
    }
}

static hf_vec3f colors[] = {
    { 1.f, 1.f, 1.f },
    { 0.f, 0.f, 0.f },
    { .7f, .2f, 0.f },
    { .5f, .5f, .5f },
};

void boids_draw(boid* boids, size_t size, hfe_mesh mesh) {
    hfe_mesh_use(mesh);
    for (size_t i = 0; i < size; i++) {
        boid b = boids[i];

        hf_mat4f mat_rot;
        hf_transform3f_rotation_z(atan2f(-b.velocity[1], b.velocity[0]) + 3.1415f / 2.f, mat_rot);

        hf_mat4f mat_tra;
        hf_transform3f_translation((hf_vec3f) { b.position[0], b.position[1], 0.f }, mat_tra);

        hf_mat4f mat_model;
        hf_mat4f_multiply_mat4f(mat_tra, mat_rot, mat_model);

        hfe_shader_property_set_mat4f(hfe_shader_property_get("u_Model"), mat_model[0]);
        hf_vec3f color;
        hf_vec3f_copy(colors[(unsigned int)b.id % (sizeof(colors) / sizeof(colors[0]))], color);
        hfe_shader_property_set_3f(hfe_shader_property_get("u_Color"), color[0], color[1], color[2]);
        hfe_mesh_draw();
    }
}
