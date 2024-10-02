#ifndef BOIDS_H
#define BOIDS_H

#include <stddef.h>//size_t

#include "hf_lib/hf_vec.h"
#include "hfe.h"

typedef struct boid_s {
    hf_vec2f position;
    hf_vec2f velocity;
    hf_vec2f acceleration;
    int id;
} boid;

void boids_set_bounds(float min_x, float min_y, float max_x, float max_y);
void boids_set_max_speed(float speed);

void boids_update(boid* boids, size_t size, float delta);
void boids_draw(boid* boids, size_t size, hfe_mesh mesh);

#endif//BOIDS_H
