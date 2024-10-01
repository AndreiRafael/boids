#ifndef BOIDS_H
#define BOIDS_H

#include <stddef.h>//size_t

#include "hf_lib/hf_vec.h"

typedef struct boid_s {
    hf_vec2f position;
    hf_vec2f velocity;
} boid;

void boids_update(boid* boids, size_t size, float delta);
void boids_draw(boid* boids, size_t size);

#endif//BOIDS_H
