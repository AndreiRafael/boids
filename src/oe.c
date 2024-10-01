#include "oe.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "hf_lib/hf_line.h"
#include "hf_lib/hf_string.h"
#include "hf_lib/hf_shape.h"
#include "hf_lib/hf_mat.h"
#include "hf_lib/hf_transform.h"

#define OE_WORLD_MAX_PLATFORMS 4000
#define OE_WORLD_MAX_WALLS 8000
#define OE_WORLD_MAX_PROPS 2000
#define OE_WORLD_MAX_INTERACTABLES 100
#define OE_WORLD_MAX_ACTORS 400
#define OE_WORLD_MAX_ACTIVATORS 20

void oe_platform_init(oe_platform* platform) {
    hf_vec3f_copy((hf_vec3f) { 0.f, 1.f, 0.f }, platform->normal);
    platform->vertices_count = 0;
}

void oe_platform_add_vertex(oe_platform* platform, hf_vec3f vertex) {
    if(platform->vertices_count >= OE_PLATFORM_MAX_VERTICES) {
        return;
    }

    hf_vec2f new_point = { vertex[0], vertex[2] };
    hf_vec2f_copy(new_point, platform->points[platform->vertices_count]);
    hf_vec3f_copy(vertex, platform->vertices[platform->vertices_count]);
    platform->vertices_count++;

    if(platform->vertices_count == 3) {// cálculo de normal
        hf_vec3f edge_a;
        hf_vec3f_subtract(platform->vertices[1], platform->vertices[0], edge_a);
        hf_vec3f_normalize(edge_a, edge_a);
        hf_vec3f edge_b;
        hf_vec3f_subtract(platform->vertices[2], platform->vertices[0], edge_b);
        hf_vec3f_normalize(edge_b, edge_b);

        hf_vec3f_cross(edge_b, edge_a, platform->normal);
        hf_vec3f_normalize(platform->normal, platform->normal);
    }
}

bool oe_platform_is_inside(oe_platform* platform, hf_vec3f position) {
    hf_vec2f position2d = { position[0], position[2] };
    return hf_shape_point_inside_polygon_convex(platform->points, platform->vertices_count, position2d);
}

float oe_platform_height_at(oe_platform* platform, hf_vec3f position) {
    hf_vec3f tan;
    hf_vec3f_cross((hf_vec3f){ 1.f, 0.f, 0.f }, platform->normal, tan);
    hf_vec3f tan2;
    hf_vec3f_cross((hf_vec3f){ 0.f, 0.f, 1.f }, platform->normal, tan2);

    hf_vec3f vec;
    hf_vec3f_subtract(position, platform->vertices[0], vec);

    return (tan2[1] / tan2[0]) * vec[0] + (tan[1] / tan[2]) * vec[2] + platform->vertices[0][1];
}

static void internal_oe_wall_info_at(oe_wall* wall, hf_vec3f point, hf_vec3f out_base, float* out_height) {
    hf_vec2f line_start = { wall->start.position[0], wall->start.position[2] };
    hf_vec2f line_end = { wall->end.position[0], wall->end.position[2] };
    hf_vec2f point_2d = { point[0], point[2] };
    float lerp = hf_segment2f_projection(line_start, line_end, point_2d);
    if(out_base) {
        hf_vec3f_lerp(wall->start.position, wall->end.position, lerp, out_base);
    }
    if(out_height) {
        *out_height = (1.f - lerp) * wall->start.height + lerp * wall->end.height;
    }
}

void oe_wall_resolve_collision(oe_wall* wall, hf_vec3f actor_position, float actor_radius, float actor_height, float actor_step) {
    hf_vec2f line_start = { wall->start.position[0], wall->start.position[2] };
    hf_vec2f line_end = { wall->end.position[0], wall->end.position[2] };
    hf_vec2f line_vec;
    hf_vec2f_subtract(line_end, line_start, line_vec);
    hf_vec2f line_normal = { -line_vec[1], line_vec[0] };

    hf_vec2f pos = { actor_position[0], actor_position[2] };
    hf_vec2f point;
    hf_segment2f_closest_point(line_start, line_end, pos, point);
    hf_vec2f diff;
    hf_vec2f_subtract(pos, point, diff);

    if(hf_vec2f_dot(line_normal, diff) < 0.f) {//ignora colisão reversa
        return;
    }

    if(hf_vec2f_square_magnitude(diff) < actor_radius * actor_radius) {
        float lerp = hf_segment2f_projection(line_start, line_end, point);
        hf_vec3f point_3d;
        hf_vec3f_lerp(wall->start.position, wall->end.position, lerp, point_3d);
        float h = (1.f - lerp) * wall->start.height + lerp * wall->end.height;
        if((actor_position[1] + actor_step) < point_3d[1] + h && (actor_position[1] + actor_height) > point_3d[1]) {
            hf_vec2f push;
            hf_vec2f_normalize(diff, push);
            hf_vec2f_multiply(push, actor_radius, push);
            hf_vec2f new_pos;
            hf_vec2f_copy(point, new_pos);
            hf_vec2f_add(new_pos, push, new_pos);
            actor_position[0] = new_pos[0];
            actor_position[2] = new_pos[1];
        }
    }
}

static oe_prop_type internal_oe_oses1_to_type(size_t group_id, size_t item_id) {
     switch(group_id) {
        case 0:
            switch(item_id) {
                case 0:
                    return oe_prop_type_player;
                case 7:
                    return oe_prop_type_dynamic_bridge;
                case 8:
                    return oe_prop_type_dynamic_gate;
                case 9:
                    return oe_prop_type_activator_hold;
                case 10:
                    return oe_prop_type_activator_permanent;
                case 11:
                    return oe_prop_type_dynamic_elevator;
            }
            break;
     }
     return oe_prop_type_static;
}

typedef enum internal_oe_prop_shape_e {
    internal_oe_prop_shape_invalid    = -1,
    internal_oe_prop_shape_block      =  0,
    internal_oe_prop_shape_half_block,
    internal_oe_prop_shape_ramp,
    internal_oe_prop_shape_bridge,
    internal_oe_prop_shape_roof4,
    internal_oe_prop_shape_roof2,
    internal_oe_prop_shape_ground_small,
    internal_oe_prop_shape_ground_big,
    internal_oe_prop_shape_fence,
    internal_oe_prop_shape_elevator,

    internal_oe_prop_shape_count,
} internal_oe_prop_shape;

static internal_oe_prop_shape internal_oe_oses1_to_shape(size_t group_id, size_t item_id) {
    switch(group_id) {
        case 0:
            switch(item_id) {
                case 7:
                case 11:
                    return internal_oe_prop_shape_elevator;//plano que nem uma ponte, mas o collider fica no chão
                case 8://portão
                    return internal_oe_prop_shape_fence;
            }
            break;
        case 1:
        case 2:
            switch(item_id) {
                case 0:
                case 7:
                    return internal_oe_prop_shape_block;
                case 1:
                    return internal_oe_prop_shape_half_block;
                case 2:
                    return internal_oe_prop_shape_ramp;
                case 3:
                case 4:
                case 5:
                case 6:
                    return internal_oe_prop_shape_bridge;
            }
            break;
        case 3:
            switch(item_id) {
                case 0:
                    return internal_oe_prop_shape_roof4;
                case 1:
                case 3:
                    return internal_oe_prop_shape_roof2;
                case 2:
                case 4:
                    return internal_oe_prop_shape_ramp;
            }
            break;
        case 4:
            switch(item_id) {
                case 1:
                case 5:
                    return internal_oe_prop_shape_fence;
            }
            break;
        case 5:
            switch(item_id) {
                case 0:
                case 2:
                case 4:
                    return internal_oe_prop_shape_ground_small;
                case 1:
                case 3:
                case 5:
                    return internal_oe_prop_shape_ground_big;
            }
            break;
    }
    return internal_oe_prop_shape_invalid;
}


void oe_interactable_init(oe_interactable* interactable, oe_prop_type type) {
    interactable->type = type;
    interactable->slide = 0.f;
    interactable->action = oe_interactable_action_none;

    interactable->prop = NULL;
    interactable->platforms_count = 0;
    interactable->walls_count = 0;
}

void oe_interactable_set_prop(oe_interactable* interactable, oe_prop* prop) {
    interactable->prop = prop;
    hf_vec3f_copy(prop->position, interactable->position_base);
}

void oe_interactable_add_wall(oe_interactable*  interactable, oe_wall* wall) {
    if(interactable->walls_count >= OE_INTERACTABLE_MAX_WALLS) {
        return;
    }

    interactable->walls[interactable->walls_count] = wall;
    interactable->walls_base[interactable->walls_count] = *wall;
    interactable->walls_count++;
}

void oe_interactable_add_platform(oe_interactable* interactable, oe_platform* platform) {
    if(interactable->platforms_count >= OE_INTERACTABLE_MAX_PLATFORMS) {
        return;
    }

    interactable->platforms[interactable->platforms_count] = platform;
    interactable->platforms_base[interactable->platforms_count] = *platform;
    interactable->platforms_count++;
}

void oe_interactable_set(oe_world* world, oe_interactable* interactable, float value) {
    hf_vec3f slide_vec = { 0.f, 0.f, 0.f };//vetor de movimento dependendo do tipo do interagível
    switch(interactable->type) {
        case oe_prop_type_dynamic_bridge:
            slide_vec[2] = -2.25f;
            break;
        case oe_prop_type_dynamic_elevator:
            slide_vec[1] = 2.25f;
            break;
        case oe_prop_type_dynamic_gate:
            slide_vec[0] = 2.25f;
            break;
        default:
            break;
    }

    hf_mat4f mat_rx;
    hf_transform3f_rotation_x(interactable->prop->rotation[0], mat_rx);
    hf_mat4f mat_ry;
    hf_transform3f_rotation_y(interactable->prop->rotation[1], mat_ry);
    hf_mat4f mat_rz;
    hf_transform3f_rotation_z(interactable->prop->rotation[2], mat_rz);
    hf_mat4f mat;
    hf_mat4f_identity(mat);
    hf_mat4f_multiply_mat4f(mat, mat_rx, mat);
    hf_mat4f_multiply_mat4f(mat, mat_ry, mat);
    hf_mat4f_multiply_mat4f(mat, mat_rz, mat);
    hf_transform3f_apply(slide_vec, mat, slide_vec);

    hf_vec3f slide_vec_prev;
    hf_vec3f_multiply(slide_vec, interactable->slide, slide_vec_prev);
    interactable->slide = value > 1.f ? 1.f : (value < 0.f ? 0.f : value);
    hf_vec3f_multiply(slide_vec, interactable->slide, slide_vec);
    hf_vec3f push_vec;
    hf_vec3f_subtract(slide_vec, slide_vec_prev, push_vec);

    if(interactable->prop) {
        hf_vec3f_add(interactable->position_base, slide_vec, interactable->prop->position);
    }
    for(size_t i = 0; i < interactable->walls_count; i++) {
        oe_wall* base = &interactable->walls_base[i];
        oe_wall* wall = interactable->walls[i];

        hf_vec3f_add(base->start.position, slide_vec, wall->start.position);
        hf_vec3f_add(base->end.position, slide_vec, wall->end.position);
    }
    for(size_t i = 0; i < interactable->platforms_count; i++) {
        oe_platform* base = &interactable->platforms_base[i];
        oe_platform* plat = interactable->platforms[i];
        if(interactable->type == oe_prop_type_dynamic_bridge) {
            for(size_t j = 0; j < world->actors_count; j++) {
                oe_actor* actor = &world->actors[j];
                if(
                    oe_platform_is_inside(interactable->platforms[i], actor->position) &&
                    fabsf(actor->position[1] - oe_platform_height_at(interactable->platforms[i], actor->position)) < 0.1f
                ) {
                    hf_vec3f new_pos;
                    hf_vec3f_add(actor->position, push_vec, new_pos);
                    oe_world_move_actor(world, actor, new_pos);
                }
            }
        }

        for(size_t j = 0; j < base->vertices_count; j++) {
            hf_vec3f* base_vert_ptr = &base->vertices[j];
            hf_vec2f* base_point_ptr = &base->points[j];
            hf_vec3f* plat_vert_ptr = &plat->vertices[j];
            hf_vec2f* plat_point_ptr = &plat->points[j];
            hf_vec2f slide_vec2 = { slide_vec[0], slide_vec[2] };

            hf_vec3f_add(*base_vert_ptr, slide_vec, *plat_vert_ptr);
            hf_vec2f_add(*base_point_ptr, slide_vec2, *plat_point_ptr);
        }
    }
}


bool oe_world_init(oe_world* world) {
    world->platforms_count = 0;
    world->platforms = malloc(sizeof(world->platforms[0]) * OE_WORLD_MAX_PLATFORMS);
    if(!world->platforms) {
        goto error_platform;
    }
    world->props_count = 0;
    world->props = malloc(sizeof(world->props[0]) * OE_WORLD_MAX_PROPS);
    if(!world->props) {
        goto error_prop;
    }
    world->walls_count = 0;
    world->walls = malloc(sizeof(world->walls[0]) * OE_WORLD_MAX_WALLS);
    if(!world->walls) {
        goto error_wall;
    }
    world->interactables_count = 0;
    world->interactables = malloc(sizeof(world->interactables[0]) * OE_WORLD_MAX_INTERACTABLES);
    if(!world->interactables) {
        goto error_interactable;
    }
    world->actors_count = 0;
    world->actors = malloc(sizeof(world->actors[0]) * OE_WORLD_MAX_ACTORS);
    if(!world->actors) {
        goto error_actor;
    }
    world->activators_count = 0;
    world->activators = malloc(sizeof(world->activators[0]) * OE_WORLD_MAX_ACTIVATORS);
    if(!world->activators) {
        goto error_activator;
    }

    world->database_count = internal_oe_prop_shape_count;
    world->database = malloc(sizeof(world->database[0]) * internal_oe_prop_shape_count);
    if(!world->database) {
        goto error_database;
    }

    //checagem de erro
    goto success;
    error_database:
        free(world->activators);
    error_activator:
        free(world->actors);
    error_actor:
        free(world->interactables);
    error_interactable:
        free(world->walls);
    error_wall:
        free(world->props);
    error_prop:
        free(world->platforms);
    error_platform:
        return false;
    success:

    //mesh dos atores
    world->actor_mesh = (oe_mesh_data) {
        .mesh = hfe_mesh_create_primitive_cylinder(0.3f, 1.5f),
        .offset = { 0.f, 1.5f / 2.f, 0.f }
    };

    //mesh dos ativadores
    world->activator_mesh = (oe_mesh_data) {
        .mesh = hfe_mesh_create_primitive_rectangular_prism(0.3f, 1.f, 0.3f),
        .offset = { 0.f, 1.f / 2.f, 0.f }
    };

    {//bloco
        oe_database_entry* db_entry = &world->database[internal_oe_prop_shape_block];
        db_entry->mesh = (oe_mesh_data) {
            .mesh = hfe_mesh_create_primitive_cube(2.25f),
            //.mesh = hfe_mesh_create_from_file_obj("./res/models/opa.obj"),
            .offset = { 0.f, 2.25f / 2.f, 0.f }
        };

        db_entry->platform.platforms_count = 1;
        oe_platform* platform = &db_entry->platform.platforms[0];
        oe_platform_init(platform);
        oe_platform_add_vertex(platform, (hf_vec3f){ -2.25f * .5f, 2.25f,  2.25f * .5f });
        oe_platform_add_vertex(platform, (hf_vec3f){  2.25f * .5f, 2.25f,  2.25f * .5f });
        oe_platform_add_vertex(platform, (hf_vec3f){  2.25f * .5f, 2.25f, -2.25f * .5f });
        oe_platform_add_vertex(platform, (hf_vec3f){ -2.25f * .5f, 2.25f, -2.25f * .5f });

        db_entry->wall.walls_count = 4;
        oe_wall* walls = db_entry->wall.walls;
        walls[0] = (oe_wall){
            .start = { { -2.25f * .5f, 0.f,  2.25f * .5f }, 2.25f },
            .end =   { {  2.25f * .5f, 0.f,  2.25f * .5f }, 2.25f },
        };
        walls[1] = (oe_wall){
            .start = { {  2.25f * .5f, 0.f,  2.25f * .5f }, 2.25f },
            .end =   { {  2.25f * .5f, 0.f, -2.25f * .5f }, 2.25f },
        };
        walls[2] = (oe_wall){
            .start = { {  2.25f * .5f, 0.f, -2.25f * .5f }, 2.25f },
            .end =   { { -2.25f * .5f, 0.f, -2.25f * .5f }, 2.25f },
        };
        walls[3] = (oe_wall){
            .start = { { -2.25f * .5f, 0.f, -2.25f * .5f }, 2.25f },
            .end =   { { -2.25f * .5f, 0.f,  2.25f * .5f }, 2.25f },
        };
    }

    {//meio bloco
        oe_database_entry* db_entry = &world->database[internal_oe_prop_shape_half_block];
        db_entry->mesh = (oe_mesh_data) {
            .mesh = hfe_mesh_create_primitive_rectangular_prism(2.25f, 2.25f / 2.f, 2.25f),
            .offset = { 0.f, 2.25f / 4.f, 0.f }
        };

        db_entry->platform.platforms_count = 1;
        oe_platform* platform = &db_entry->platform.platforms[0];
        oe_platform_init(platform);
        oe_platform_add_vertex(platform, (hf_vec3f){ -2.25f * .5f, 2.25f * 0.5f,  2.25f * .5f });
        oe_platform_add_vertex(platform, (hf_vec3f){  2.25f * .5f, 2.25f * 0.5f,  2.25f * .5f });
        oe_platform_add_vertex(platform, (hf_vec3f){  2.25f * .5f, 2.25f * 0.5f, -2.25f * .5f });
        oe_platform_add_vertex(platform, (hf_vec3f){ -2.25f * .5f, 2.25f * 0.5f, -2.25f * .5f });

        db_entry->wall.walls_count = 4;
        oe_wall* walls = db_entry->wall.walls;
        walls[0] = (oe_wall){
            .start = { { -2.25f * .5f, 0.f,  2.25f * .5f }, 2.25f * .5f },
            .end =   { {  2.25f * .5f, 0.f,  2.25f * .5f }, 2.25f * .5f },
        };
        walls[1] = (oe_wall){
            .start = { {  2.25f * .5f, 0.f,  2.25f * .5f }, 2.25f * .5f },
            .end =   { {  2.25f * .5f, 0.f, -2.25f * .5f }, 2.25f * .5f },
        };
        walls[2] = (oe_wall){
            .start = { {  2.25f * .5f, 0.f, -2.25f * .5f }, 2.25f * .5f },
            .end =   { { -2.25f * .5f, 0.f, -2.25f * .5f }, 2.25f * .5f },
        };
        walls[3] = (oe_wall){
            .start = { { -2.25f * .5f, 0.f, -2.25f * .5f }, 2.25f * .5f },
            .end =   { { -2.25f * .5f, 0.f,  2.25f * .5f }, 2.25f * .5f },
        };
    }

    {//rampa
        float base = 2.25f / 2.f;
        float height = 2.25f / 2.f;
        float v = 1.f / 4.f;
        float vertex_data[] = {
            -base,    0.f,  base,  0.f, 0.f,  0.f, 0.f,  1.f,//0
            base,    0.f,  base,  1.f, 0.f,   0.f, 0.f,  1.f,
            base, height,  base,  1.f,   v,   0.f, 0.f,  1.f,

            base,    0.f,  base,  0.f, 0.f,  1.f, 0.f, 0.f,
            base,    0.f, -base,  1.f, 0.f,  1.f, 0.f, 0.f,
            base, height, -base,  1.f,   v,  1.f, 0.f, 0.f,
            base, height,  base,  0.f,   v,  1.f, 0.f, 0.f,

            base,    0.f, -base,   0.f, 0.f,  0.f, 0.f, -1.f,//7
            -base,    0.f, -base,  1.f, 0.f,  0.f, 0.f, -1.f,
            base, height, -base,   0.f,   v,  0.f, 0.f, -1.f,

            -base,    0.f, -base,  0.f, 0.f,  -.44f, .89f, 0.f,
            -base,    0.f,  base,  1.f, 0.f,  -.44f, .89f, 0.f,
            base, height,  base,  1.f, 1.f,  -.44f, .89f, 0.f,
            base, height, -base,  0.f, 1.f,  -.44f, .89f, 0.f,
        };

        unsigned short triangle_data[] = {
            0, 1, 2,
            3, 4, 5,
            3, 5, 6,
            7, 8, 9,
            10, 11, 12,
            10, 12, 13,
        };

        oe_database_entry* db_entry = &world->database[internal_oe_prop_shape_ramp];
        db_entry->mesh = (oe_mesh_data) {
            .mesh = hfe_mesh_create_indexed_fff(vertex_data, sizeof(vertex_data) / sizeof(float) / 8, triangle_data, sizeof(triangle_data) / sizeof(triangle_data[0]) / 3, 3, 2, 3),
            .offset = { 0.f, 0.f, 0.f }
        };

        db_entry->platform.platforms_count = 1;

        oe_platform* platforms = &db_entry->platform.platforms[0];
        oe_platform_init(&platforms[0]);
        oe_platform_add_vertex(&platforms[0], (hf_vec3f){ -2.25f * 0.5,         0.f,  2.25f * 0.5f });
        oe_platform_add_vertex(&platforms[0], (hf_vec3f){  2.25f * 0.5, 2.25f * .5f,  2.25f * 0.5f });
        oe_platform_add_vertex(&platforms[0], (hf_vec3f){  2.25f * 0.5, 2.25f * .5f, -2.25f * 0.5f });
        oe_platform_add_vertex(&platforms[0], (hf_vec3f){ -2.25f * 0.5,         0.f, -2.25f * 0.5f });

        db_entry->wall.walls_count = 3;
        oe_wall* walls = db_entry->wall.walls;
        walls[0] = (oe_wall){
            .start = { { -base, 0.f,  base }, 0.f },
            .end =   { {  base, 0.f,  base }, height },
        };
        walls[1] = (oe_wall){
            .start = { {  base, 0.f,  base }, height },
            .end =   { {  base, 0.f, -base }, height },
        };
        walls[2] = (oe_wall){
            .start = { {  base, 0.f, -base }, height },
            .end =   { { -base, 0.f, -base }, 0.f },
        };
    }

    {//ponte
        oe_database_entry* db_entry = &world->database[internal_oe_prop_shape_bridge];
        db_entry->mesh = (oe_mesh_data) {
            .mesh = hfe_mesh_create_primitive_quad(2.25f),
            .offset = { 0.f, 2.25f, 0.f }
        };

        db_entry->platform.platforms_count = 1;

        oe_platform* platforms = &db_entry->platform.platforms[0];
        oe_platform_init(&platforms[0]);
        oe_platform_add_vertex(&platforms[0], (hf_vec3f){ -2.25f * 0.5, 2.25f,  2.25f * 0.5f });
        oe_platform_add_vertex(&platforms[0], (hf_vec3f){  2.25f * 0.5, 2.25f,  2.25f * 0.5f });
        oe_platform_add_vertex(&platforms[0], (hf_vec3f){  2.25f * 0.5, 2.25f, -2.25f * 0.5f });
        oe_platform_add_vertex(&platforms[0], (hf_vec3f){ -2.25f * 0.5, 2.25f, -2.25f * 0.5f });

        db_entry->wall.walls_count = 0;
    }

    //quatro águas
    {
        float base = 2.25f / 2.f;
        float height = 2.25f / 4.f;
        float vertex_data[] = {// TODO: normals!!
            -base,    0.f,  base,  0.f, 0.f,  0.f, .89f, .44f,//0
            base,    0.f,  base,  1.f, 0.f,  0.f, .89f, .44f,
            0.f, height,   0.f,  .5f, 1.f,  0.f, .89f, .44f,

            base,    0.f,  base,  0.f, 0.f,  .44f, .89f, 0.f,//0
            base,    0.f, -base,  1.f, 0.f,  .44f, .89f, 0.f,
            0.f, height,   0.f,  .5f, 1.f,  .44f, .89f, 0.f,

            base,    0.f, -base,  0.f, 0.f,  0.f, .89f, -.44f,//0
            -base,    0.f, -base,  1.f, 0.f,  0.f, .89f, -.44f,
            0.f, height,   0.f,  .5f, 1.f,  0.f, .89f, -.44f,

            -base,    0.f, -base,  0.f, 0.f,  -.44f, .89f, 0.f,//0
            -base,    0.f,  base,  1.f, 0.f,  -.44f, .89f, 0.f,
            0.f, height,   0.f,  .5f, 1.f,  -.44f, .89f, 0.f,
        };

        unsigned short triangle_data[] = {
            0, 1, 2,
            3, 4, 5,
            6, 7, 8,
            9, 10, 11
        };

        oe_database_entry* db_entry = &world->database[internal_oe_prop_shape_roof4];
        db_entry->mesh = (oe_mesh_data) {
            .mesh = hfe_mesh_create_indexed_fff(vertex_data, sizeof(vertex_data) / sizeof(float) / 8, triangle_data, sizeof(triangle_data) / sizeof(triangle_data[0]) / 3, 3, 2, 3),
            .offset = { 0.f, 0.f, 0.f }
        };

        db_entry->platform.platforms_count = 4;

        oe_platform* platforms = &db_entry->platform.platforms[0];
        oe_platform_init(&platforms[0]);
        oe_platform_add_vertex(&platforms[0], (hf_vec3f){ -base,    0.f,  base });
        oe_platform_add_vertex(&platforms[0], (hf_vec3f){  base,    0.f,  base });
        oe_platform_add_vertex(&platforms[0], (hf_vec3f){   0.f, height,   0.f });

        oe_platform_init(&platforms[1]);
        oe_platform_add_vertex(&platforms[1], (hf_vec3f){  base,    0.f,  base });
        oe_platform_add_vertex(&platforms[1], (hf_vec3f){  base,    0.f, -base });
        oe_platform_add_vertex(&platforms[1], (hf_vec3f){   0.f, height,   0.f });

        oe_platform_init(&platforms[2]);
        oe_platform_add_vertex(&platforms[2], (hf_vec3f){  base,    0.f, -base });
        oe_platform_add_vertex(&platforms[2], (hf_vec3f){ -base,    0.f, -base });
        oe_platform_add_vertex(&platforms[2], (hf_vec3f){   0.f, height,   0.f });

        oe_platform_init(&platforms[3]);
        oe_platform_add_vertex(&platforms[3], (hf_vec3f){ -base,    0.f, -base });
        oe_platform_add_vertex(&platforms[3], (hf_vec3f){ -base,    0.f,  base });
        oe_platform_add_vertex(&platforms[3], (hf_vec3f){   0.f, height,   0.f });

        db_entry->wall.walls_count = 0;
    }

    //duas águas
    {
        float base = 2.25f / 2.f;
        float height = 2.25f / 4.f;
        float vertex_data[] = {
            -base,    0.f,  base,  0.f, 0.f,  0.f, 0.f, 1.f,//0
            base,    0.f,  base,  1.f, 0.f,  0.f, 0.f, 1.f,
            0.f, height,  base,  .5f, 1.f,  0.f, 0.f, 1.f,

            base,    0.f, -base,  0.f, 0.f,  0.f, 0.f, -1.f,//0
            -base,    0.f, -base,  1.f, 0.f,  0.f, 0.f, -1.f,
            0.f, height, -base,  .5f, 1.f,  0.f, 0.f, -1.f,

            base,    0.f,  base,  0.f, 0.f,  .44f, .89f, 0.f,//0
            base,    0.f, -base,  1.f, 0.f,  .44f, .89f, 0.f,
            0.f, height, -base,  1.f, .5f,  .44f, .89f, 0.f,
            0.f, height,  base,  0.f, .5f,  .44f, .89f, 0.f,

            -base,    0.f, -base,  1.f, 1.f,  -.44f, .89f, 0.f,//0
            -base,    0.f,  base,  0.f, 1.f,  -.44f, .89f, 0.f,
            0.f, height,  base,  0.f, .5f,  -.44f, .89f, 0.f,
            0.f, height, -base,  1.f, .5f,  -.44f, .89f, 0.f,
        };

        unsigned short triangle_data[] = {
            0, 1, 2,
            3, 4, 5,

            6, 7, 8,
            6, 8, 9,

            10, 11, 12,
            10, 12, 13,
        };

        oe_database_entry* db_entry = &world->database[internal_oe_prop_shape_roof2];
        db_entry->mesh = (oe_mesh_data) {
            .mesh = hfe_mesh_create_indexed_fff(vertex_data, sizeof(vertex_data) / sizeof(float) / 8, triangle_data, sizeof(triangle_data) / sizeof(triangle_data[0]) / 3, 3, 2, 3),
            .offset = { 0.f, 0.f, 0.f }
        };

        db_entry->platform.platforms_count = 2;

        oe_platform* platforms = &db_entry->platform.platforms[0];
        oe_platform_init(&platforms[0]);
        oe_platform_add_vertex(&platforms[0], (hf_vec3f){ -2.25f * 0.5,          0.f,  2.25f * 0.5f });
        oe_platform_add_vertex(&platforms[0], (hf_vec3f){    0.f * 0.5, 2.25f * .25f,  2.25f * 0.5f });
        oe_platform_add_vertex(&platforms[0], (hf_vec3f){    0.f * 0.5, 2.25f * .25f, -2.25f * 0.5f });
        oe_platform_add_vertex(&platforms[0], (hf_vec3f){ -2.25f * 0.5,          0.f, -2.25f * 0.5f });

        oe_platform_init(&platforms[1]);
        oe_platform_add_vertex(&platforms[1], (hf_vec3f){          0.f, 2.25f * .25f,  2.25f * 0.5f });
        oe_platform_add_vertex(&platforms[1], (hf_vec3f){  2.25f * 0.5,          0.f,  2.25f * 0.5f });
        oe_platform_add_vertex(&platforms[1], (hf_vec3f){  2.25f * 0.5,          0.f, -2.25f * 0.5f });
        oe_platform_add_vertex(&platforms[1], (hf_vec3f){          0.f, 2.25f * .25f, -2.25f * 0.5f });

        db_entry->wall.walls_count = 4;
        oe_wall* walls = db_entry->wall.walls;
        walls[0] = (oe_wall){
            .start = { { -base, 0.f,  base }, 0.f },
            .end =   { {   0.f, 0.f,  base }, height },
        };
        walls[1] = (oe_wall){
            .start = { {   0.f, 0.f,  base }, height },
            .end =   { {  base, 0.f,  base }, 0.f },
        };
        walls[2] = (oe_wall){
            .start = { {  base, 0.f, -base }, 0.f },
            .end =   { {   0.f, 0.f, -base }, height },
        };
        walls[3] = (oe_wall){
            .start = { {   0.f, 0.f, -base }, height },
            .end =   { { -base, 0.f, -base }, 0.f },
        };
    }

    {//terreno P
        oe_database_entry* db_entry = &world->database[internal_oe_prop_shape_ground_small];
        db_entry->mesh = (oe_mesh_data) {
            .mesh = hfe_mesh_create_primitive_quad(2.25f * 3),
            .offset = { 0.f, 0.f, 0.f }
        };

        db_entry->platform.platforms_count = 1;
        oe_platform* platform = &db_entry->platform.platforms[0];
        oe_platform_init(platform);
        oe_platform_add_vertex(platform, (hf_vec3f){ -2.25f * 1.5f, 0.f,  2.25f * 1.5f });
        oe_platform_add_vertex(platform, (hf_vec3f){  2.25f * 1.5f, 0.f,  2.25f * 1.5f });
        oe_platform_add_vertex(platform, (hf_vec3f){  2.25f * 1.5f, 0.f, -2.25f * 1.5f });
        oe_platform_add_vertex(platform, (hf_vec3f){ -2.25f * 1.5f, 0.f, -2.25f * 1.5f });

        db_entry->wall.walls_count = 0;
    }

    {//terreno G
        oe_database_entry* db_entry = &world->database[internal_oe_prop_shape_ground_big];
        db_entry->mesh = (oe_mesh_data) {
            .mesh = hfe_mesh_create_primitive_quad(2.25f * 9),
            .offset = { 0.f, 0.f, 0.f }
        };

        db_entry->platform.platforms_count = 1;
        oe_platform* platform = &db_entry->platform.platforms[0];
        oe_platform_init(platform);
        oe_platform_add_vertex(platform, (hf_vec3f){ -2.25f * 4.5f, 0.f,  2.25f * 4.5f });
        oe_platform_add_vertex(platform, (hf_vec3f){  2.25f * 4.5f, 0.f,  2.25f * 4.5f });
        oe_platform_add_vertex(platform, (hf_vec3f){  2.25f * 4.5f, 0.f, -2.25f * 4.5f });
        oe_platform_add_vertex(platform, (hf_vec3f){ -2.25f * 4.5f, 0.f, -2.25f * 4.5f });

        db_entry->wall.walls_count = 0;
    }

    {//cerca
        oe_database_entry* db_entry = &world->database[internal_oe_prop_shape_fence];
        db_entry->mesh = (oe_mesh_data) {
            .mesh = hfe_mesh_create_primitive_rectangular_prism(2.25f, 2.25 / 4.f, 0.2f),
            .offset = { 0.f, 2.25f / 8.f, 0.f }
        };

        db_entry->platform.platforms_count = 1;
        oe_platform* platform = &db_entry->platform.platforms[0];
        oe_platform_init(platform);
        oe_platform_add_vertex(platform, (hf_vec3f){ -2.25f / 2.f, 2.25 / 4.f,  .1f });
        oe_platform_add_vertex(platform, (hf_vec3f){  2.25f / 2.f, 2.25 / 4.f,  .1f });
        oe_platform_add_vertex(platform, (hf_vec3f){  2.25f / 2.f, 2.25 / 4.f, -.1f });
        oe_platform_add_vertex(platform, (hf_vec3f){ -2.25f / 2.f, 2.25 / 4.f, -.1f });

        db_entry->wall.walls_count = 4;
        oe_wall* walls = db_entry->wall.walls;
        walls[0] = (oe_wall){
            .start = { { -2.25f / 2.f, 0.f,  .1f }, 2.25f / 4.f },
            .end =   { {  2.25f / 2.f, 0.f,  .1f }, 2.25f / 4.f },
        };
        walls[1] = (oe_wall){
            .start = { {  2.25f / 2.f, 0.f, -.1f }, 2.25f / 4.f },
            .end =   { { -2.25f / 2.f, 0.f, -.1f }, 2.25f / 4.f },
        };
        walls[2] = (oe_wall){
            .start = { {  2.25f / 2.f, 0.f,  .1f }, 2.25f / 4.f },
            .end =   { {  2.25f / 2.f, 0.f, -.1f }, 2.25f / 4.f },
        };
         walls[3] = (oe_wall){
            .start = { { -2.25f / 2.f, 0.f, -.1f }, 2.25f / 4.f },
            .end =   { { -2.25f / 2.f, 0.f,  .1f }, 2.25f / 4.f },
        };
    }

    {//elevador
        oe_database_entry* db_entry = &world->database[internal_oe_prop_shape_elevator];
        db_entry->mesh = (oe_mesh_data) {
            .mesh = hfe_mesh_create_primitive_rectangular_prism(2.25f, 0.2f, 2.25f),
            .offset = { 0.f, 0.f, 0.f }
        };

        db_entry->platform.platforms_count = 1;

        oe_platform* platforms = &db_entry->platform.platforms[0];
        oe_platform_init(&platforms[0]);
        oe_platform_add_vertex(&platforms[0], (hf_vec3f){ -2.25f * 0.5, 0.1f,  2.25f * 0.5f });
        oe_platform_add_vertex(&platforms[0], (hf_vec3f){  2.25f * 0.5, 0.1f,  2.25f * 0.5f });
        oe_platform_add_vertex(&platforms[0], (hf_vec3f){  2.25f * 0.5, 0.1f, -2.25f * 0.5f });
        oe_platform_add_vertex(&platforms[0], (hf_vec3f){ -2.25f * 0.5, 0.1f, -2.25f * 0.5f });

        db_entry->wall.walls_count = 0;
    }
    return true;
}

void oe_world_deinit(oe_world* world) {
    hfe_mesh_destroy(world->actor_mesh.mesh);
    hfe_mesh_destroy(world->activator_mesh.mesh);
    for(size_t i = 0; i < world->database_count; i++) {
        hfe_mesh_destroy(world->database[i].mesh.mesh);
    }
    free(world->database);
    world->database = NULL;

    oe_world_unload(world);
    free(world->platforms);
    world->platforms = NULL;
    free(world->walls);
    world->walls = NULL;
    free(world->props);
    world->props = NULL;
    free(world->interactables);
    world->interactables = NULL;
    free(world->actors);
    world->actors = NULL;
    free(world->activators);
    world->activators = NULL;
}

static bool internal_oe_file_read_line(FILE* file, char* buffer) {
    size_t index = 0;
    while(true) {
        int ci = fgetc(file);
        if(ci == EOF) {
            if(index == 0) {
                return false;
            }
            break;
        }

        char c = (char)ci;
        if(c == '\n') {
            break;
        }
        if(c != '\r') {
            buffer[index++] = c;
        }
    }

    buffer[index] = '\0';
    return true;
}

static void internal_oe_world_init_activators(oe_world* world) {
    for(size_t i = 0; i < world->activators_count; i++) {
        oe_activator* activator = &world->activators[i];
        float best_dist;
        oe_interactable* best_interactable = NULL;
        for(size_t j = 0; j < world->interactables_count; j++) {
            oe_interactable* interactable = &world->interactables[j];

            hf_vec3f diff;
            hf_vec3f_subtract(activator->position, interactable->position_base, diff);
            float new_dist = hf_vec3f_square_magnitude(diff);

            if(!best_interactable || new_dist < best_dist) {
                best_interactable = interactable;
                best_dist = new_dist;
            }
        }
        activator->interactable = best_interactable;
    }
}

void oe_world_load(oe_world* world, const char* path) {
    (void)world;

    FILE* file = fopen(path, "r");
    if(!file) {
        return;
    }

    char buffer[256];
    for(size_t i = 0; i < 13; i++) { internal_oe_file_read_line(file, buffer); }//TODO: por enquanto, ignora as 13 primeiras linhas
    while(true) {
        int group = 0;
        int item = 0;
        hf_vec3f position = { 0.f, 0.f, 0.f };
        hf_vec3f rotation = { 0.f, 0.f, 0.f };

        {//leitura do grupo
            if(!internal_oe_file_read_line(file, buffer)) {
                goto finish;
            }
            const char* str = buffer;
            str = hf_string_find_int(str, &group);
        }
        {//leitura do prop
            if(!internal_oe_file_read_line(file, buffer)) {
                goto finish;
            }
            const char* str = buffer;
            str = hf_string_find_int(str, &item);
        }
        {//leitura da posição
            if(!internal_oe_file_read_line(file, buffer)) {
                goto finish;
            }
            const char* str = buffer;
            str = hf_string_find_float(str, &position[0]);
            str = hf_string_find_float(str, &position[1]);
            str = hf_string_find_float(str, &position[2]);
            position[2] = -position[2];
        }
        {//leitura da rotação
            if(!internal_oe_file_read_line(file, buffer)) {
                goto finish;
            }

            float qx;
            float qy;
            float qz;
            float qw;
            const char* str = buffer;
            str = hf_string_find_float(str, &qx);
            str = hf_string_find_float(str, &qy);
            str = hf_string_find_float(str, &qz);
            str = hf_string_find_float(str, &qw);

            {// http://stackoverflow.com/questions/12088610/conversion-between-euler-quaternion-like-in-unity3d-engine
                float sqw = qw * qw;
                float sqx = qx * qx;
                float sqy = qy * qy;
                float sqz = qz * qz;
                float unit = sqx + sqy + sqz + sqw;
                float test = qx * qw - qy * qz;
                hf_vec3f v;

                if (test > 0.4995f * unit)
                {
                    v[1] = 2.f * atan2f(qy, qx);
                    v[0] = 3.1415f / 2.f;
                    v[2] = 0;
                }
                else if (test < -0.4995f * unit)
                {
                    v[1] = -2.f * atan2f(qy, qx);
                    v[0] = -3.1415f / 2.f;
                    v[2] = 0;
                }
                else {
                    float q[4] =  { qw, qz, qx, qy };
                    v[1] = (float)atan2f(2.f * q[0] * q[3] + 2.f * q[1] * q[2], 1 - 2.f * (q[2] * q[2] + q[3] * q[3]));
                    v[0] = (float)asinf(2.f * (q[0] * q[2] - q[3] * q[1]));
                    v[2] = (float)atan2f(2.f * q[0] * q[1] + 2.f * q[2] * q[3], 1 - 2.f * (q[1] * q[1] + q[2] * q[2]));
                }
                hf_vec3f_copy(v, rotation);
                if(group == 1 || group == 2) {
                    if(item == 2) {
                        rotation[1] -= 3.1415f / 2.f;
                    }
                }
                if(group != 5) {
                    rotation[0] += 3.1415f / 2.f;
                }
            }

            if(group == 6) {//grupo de trigger, tem que pular a linha extra de texto
                if(!internal_oe_file_read_line(file, buffer)) {
                    goto finish;
                }
            }
        }

        {//spawn dos props de acordo com as informações coletadas
            internal_oe_prop_shape shape = internal_oe_oses1_to_shape((size_t)group, (size_t)item);
            oe_prop_type type = internal_oe_oses1_to_type((size_t)group, (size_t)item);

            if(type == oe_prop_type_player) {
                oe_world_add_actor(world, position);
            }
            else if(type == oe_prop_type_activator_hold) {
                oe_world_add_activator(world, position, true);
            }
            else if(type == oe_prop_type_activator_permanent) {
                oe_world_add_activator(world, position, false);
            }

            if(shape == internal_oe_prop_shape_invalid) {
                continue;
            }
            oe_mesh_data mesh_data = world->database[shape].mesh;
            oe_prop* prop = oe_world_add_prop(world, mesh_data, position, rotation);

            oe_interactable* interactable = oe_world_add_interactable(world, type);

            if(interactable && prop) {
                oe_interactable_set_prop(interactable, prop);
            }

            oe_platform_data platform_data = world->database[shape].platform;
            for(size_t i = 0; i < platform_data.platforms_count; i++) {
                oe_platform* platform = oe_world_add_platform(world, &platform_data.platforms[i], position, rotation);
                if(interactable && platform) {
                    oe_interactable_add_platform(interactable, platform);
                }
            }

            oe_wall_data wall_data = world->database[shape].wall;
            for(size_t i = 0; i < wall_data.walls_count; i++) {
                oe_wall* wall = oe_world_add_wall(world, &wall_data.walls[i], position, rotation);
                if(interactable && wall) {
                    oe_interactable_add_wall(interactable, wall);
                }
            }
        }
    }

    finish:
        internal_oe_world_init_activators(world);
}

void oe_world_unload(oe_world* world) {
    world->props_count = 0;
    world->platforms_count = 0;
    world->walls_count = 0;
    world->interactables_count = 0;
    world->actors_count = 0;
}

oe_platform* oe_world_add_platform(oe_world* world, oe_platform* platform_blueprint, hf_vec3f position, hf_vec3f rotation) {
    if(platform_blueprint->vertices_count < 3) {
        return NULL;
    }

    if(world->platforms_count >= OE_WORLD_MAX_PLATFORMS) {
        return NULL;
    }

    oe_platform* new_platform = &world->platforms[world->platforms_count++];
    oe_platform_init(new_platform);

    hf_mat4f mat_rx;
    hf_transform3f_rotation_x(rotation[0], mat_rx);
    hf_mat4f mat_ry;
    hf_transform3f_rotation_y(rotation[1], mat_ry);
    hf_mat4f mat_rz;
    hf_transform3f_rotation_z(rotation[2], mat_rz);

    hf_mat4f mat;
    hf_mat4f_identity(mat);
    hf_mat4f_multiply_mat4f(mat, mat_rx, mat);
    hf_mat4f_multiply_mat4f(mat, mat_ry, mat);
    hf_mat4f_multiply_mat4f(mat, mat_rz, mat);

    for(size_t i = 0; i < platform_blueprint->vertices_count; i++) {
        hf_vec3f new_vert;
        hf_vec3f_copy(platform_blueprint->vertices[platform_blueprint->vertices_count - 1 - i], new_vert);
        hf_transform3f_apply(new_vert, mat, new_vert);
        hf_vec3f_add(new_vert, position, new_vert);
        oe_platform_add_vertex(new_platform, new_vert);
    }

    return new_platform;
}

oe_prop* oe_world_add_prop(oe_world* world, oe_mesh_data mesh_data, hf_vec3f position, hf_vec3f rotation) {
    if(world->props_count >= OE_WORLD_MAX_PROPS) {
        return NULL;
    }

    oe_prop* new_prop = &world->props[world->props_count++];
    *new_prop = (oe_prop){
        .mesh_data = mesh_data,
    };
    hf_vec3f_copy(position, new_prop->position);
    hf_vec3f_copy(rotation, new_prop->rotation);

    return new_prop;
}

oe_wall* oe_world_add_wall(oe_world* world, oe_wall* blueprint, hf_vec3f position, hf_vec3f rotation) {
    if(world->walls_count >= OE_WORLD_MAX_WALLS) {
        return NULL;
    }

    oe_wall* new_wall = &world->walls[world->walls_count++];
    *new_wall = *blueprint;

    hf_mat4f mat_rx;
    hf_transform3f_rotation_x(rotation[0], mat_rx);
    hf_mat4f mat_ry;
    hf_transform3f_rotation_y(rotation[1], mat_ry);
    hf_mat4f mat_rz;
    hf_transform3f_rotation_z(rotation[2], mat_rz);

    hf_mat4f mat;
    hf_mat4f_identity(mat);
    hf_mat4f_multiply_mat4f(mat, mat_rx, mat);
    hf_mat4f_multiply_mat4f(mat, mat_ry, mat);
    hf_mat4f_multiply_mat4f(mat, mat_rz, mat);

    hf_transform3f_apply(new_wall->start.position, mat, new_wall->start.position);
    hf_vec3f_add(new_wall->start.position, position, new_wall->start.position);
    hf_transform3f_apply(new_wall->end.position, mat, new_wall->end.position);
    hf_vec3f_add(new_wall->end.position, position, new_wall->end.position);

    return new_wall;
}

oe_interactable* oe_world_add_interactable(oe_world* world, oe_prop_type type) {
    if(type == oe_prop_type_static) {
        return NULL;
    }

    if(world->interactables_count >= OE_WORLD_MAX_INTERACTABLES) {
        return NULL;
    }

    oe_interactable* new_interactable = &world->interactables[world->interactables_count++];
    oe_interactable_init(new_interactable, type);
    return new_interactable;
}

oe_actor* oe_world_add_actor(oe_world* world, hf_vec3f position) {
    if(world->actors_count >= OE_WORLD_MAX_ACTORS) {
        return NULL;
    }

    oe_actor* new_actor = &world->actors[world->actors_count++];
    new_actor->height = 1.5f;
    new_actor->radius = .3f;
    oe_world_move_actor(world, new_actor, position);
    return new_actor;
}

oe_activator* oe_world_add_activator(oe_world* world, hf_vec3f position, bool hold) {
    if(world->activators_count >= OE_WORLD_MAX_ACTIVATORS) {
        return NULL;
    }

    oe_activator* new_activator = &world->activators[world->activators_count++];
    *new_activator = (oe_activator) {
        .interactable = NULL,
        .revert = hold
    };
    hf_vec3f_copy(position, new_activator->position);
    return new_activator;
}

float oe_world_height_at_max(oe_world* world, hf_vec3f position) {
    float height = -1.f;
    for(size_t i = 0; i < world->platforms_count; i++) {
        oe_platform* platform = &world->platforms[i];
        if(oe_platform_is_inside(platform, position)) {
            float new_height = oe_platform_height_at(platform, position);
            if(new_height > height) {
                height = new_height;
            }
        }
    }
    return height;
}

float oe_world_height_at_step(oe_world* world, hf_vec3f position, float step) {
    float height = position[1];
    float step_height = position[1] + step;
    for(size_t i = 0; i < world->platforms_count; i++) {
        oe_platform* platform = &world->platforms[i];
        if(oe_platform_is_inside(platform, position)) {
            float new_height = oe_platform_height_at(platform, position);
            if(new_height < step_height && new_height > height) {
                height = new_height;
            }
        }
    }
    return height;
}

// TODO: maybe put this into the shapes api
static bool internal_oe_intersect_line_plane(hf_vec3f plane_point, hf_vec3f plane_normal, hf_vec3f line_point, hf_vec3f line_normal, hf_vec3f out_point, float* out_distance) {
    float dot = hf_vec3f_dot(plane_normal, line_normal);
    if(dot == 0.f) {
        return false;
    }
    hf_vec3f line_to_plane;
    hf_vec3f_subtract(plane_point, line_point, line_to_plane);
    float distance = hf_vec3f_dot(line_to_plane, plane_normal) / dot;
    hf_vec3f offset;
    hf_vec3f_multiply(line_normal, distance, offset);
    if(out_point) {
        hf_vec3f_add(line_point, offset, out_point);
    }
    if(out_distance) {
        *out_distance = distance;
    }
    return true;
}

bool oe_world_raycast(oe_world* world, hf_vec3f position, hf_vec3f direction, oe_raycast_mask mask, oe_raycast_result* out_result) {
    bool hit = false;
    oe_raycast_result result = {
        .position = { 0.f, 0.f, 0.f },
        .normal = { 0.f, 0.f, 0.f },
        .distance = 0.f,
    };

    //raycast against platforms
    if(mask & oe_raycast_mask_platform) {
        for(size_t i = 0; i < world->platforms_count; i++) {
            oe_platform* platform = &world->platforms[i];
            hf_vec3f point;
            float distance;
            if(
                hf_vec3f_dot(platform->normal, direction) < 0.f &&
                internal_oe_intersect_line_plane(platform->vertices[0], platform->normal, position, direction, point, &distance) &&
                oe_platform_is_inside(platform, point)
            ) {
                if(distance >= 0.f && (!hit || distance < result.distance)) {
                    result.distance = distance;
                    hf_vec3f_copy(point, result.position);
                    hf_vec3f_copy(platform->normal, result.normal);
                }
                hit = true;
            }
        }
    }

    //raycast against walls
    if(mask & oe_raycast_mask_wall) {
        for(size_t i = 0; i < world->walls_count; i++) {
            oe_wall* wall = &world->walls[i];
            hf_vec3f point;
            float distance;

            hf_vec3f dir;
            hf_vec3f_subtract(wall->end.position, wall->start.position, dir);
            dir[1] = 0.f;
            hf_vec3f normal = { -dir[2], 0.f, dir[0] };
            hf_vec3f_normalize(normal, normal);
            if(
                hf_vec3f_dot(normal, direction) < 0.f &&
                internal_oe_intersect_line_plane(wall->start.position, normal, position, direction, point, &distance)
            ) {
                // guarantee point is along wall vertically
                hf_vec3f base;
                float height;
                internal_oe_wall_info_at(wall, point, base, &height);
                if(point[1] < base[1] || point[1] > (base[1] + height)) {
                    continue;
                }
                hf_vec3f to_start;
                hf_vec3f_subtract(wall->start.position, point, to_start);
                to_start[1] = 0.f;
                hf_vec3f to_end;
                hf_vec3f_subtract(wall->end.position, point, to_end);
                to_end[1] = 0.f;
                if(// guarantee point is along wall horizontally
                    hf_vec3f_dot(to_end, dir) < 0.f ||
                    hf_vec3f_dot(to_start, dir) > 0.f
                ) {
                    continue;
                }

                if(distance >= 0.f && (!hit || distance < result.distance)) {
                    result.distance = distance;
                    hf_vec3f_copy(point, result.position);
                    hf_vec3f_copy(normal, result.normal);
                }
                hit = true;
            }
        }
    }

    //raycast against actors
    if(mask & oe_raycast_mask_actor) {
        for(size_t i = 0; i < world->actors_count; i++) {
            oe_actor* actor = &world->actors[i];
            hf_vec2f actor_position_2d = { actor->position[0], actor->position[2] };
            float sqr_radius = actor->radius * actor->radius;
            if(hf_vec3f_dot(direction, (hf_vec3f) { 0.f, 1.f, 0.f }) <= 0.f) {
                hf_vec3f top_plane;
                hf_vec3f_add(actor->position, (hf_vec3f) { 0.f, actor->height, 0.f }, top_plane);
                hf_vec3f point;
                float distance;
                internal_oe_intersect_line_plane(top_plane, (hf_vec3f) { 0.f, 1.f, 0.f }, position, direction, point, &distance);
                if(!hit || distance < result.distance) {
                    if(hf_vec2f_square_distance(actor_position_2d, (hf_vec2f) { point[0], point[2] }) < sqr_radius) {
                        hit = true;
                        result.distance = distance;
                        hf_vec3f_copy(point, result.position);
                        hf_vec3f_copy((hf_vec3f) { 0.f, 1.f, 0.f }, result.normal);
                    }
                }
            }
            {
                hf_vec2f closest_point_2d;
                hf_vec2f position_2d = { position[0], position[2] };
                hf_vec2f direction_2d = { direction[0], direction[2] };
                float projection = hf_line2f_projection(position_2d, direction_2d, actor_position_2d);
                hf_vec2f_multiply(direction_2d, projection, closest_point_2d);
                hf_vec2f_add(closest_point_2d, position_2d, closest_point_2d);
                if(hf_vec2f_square_distance(closest_point_2d, actor_position_2d) < sqr_radius) {//possible hit
                    hf_vec3f closest_point = { closest_point_2d[0], position[1] + direction[1] * projection, closest_point_2d[1] };
                    float b = sqrtf(sqr_radius - hf_vec2f_square_distance(closest_point_2d, actor_position_2d));
                    b *= 1.f / hf_vec2f_magnitude(direction_2d);
                    hf_vec3f hit_point;
                    hf_vec3f_multiply(direction, -b, hit_point);
                    hf_vec3f_add(closest_point, hit_point, hit_point);
                    float distance = hf_vec3f_distance(position, hit_point);
                    if((!hit || distance < result.distance) && hit_point[1] > actor->position[1] && hit_point[1] < (actor->position[1] + actor->height)) {
                        hit = true;
                        result.distance = distance;
                        hf_vec3f_copy(hit_point, result.position);
                        hf_vec2f normal_2d;
                        hf_vec2f_subtract((hf_vec2f) { hit_point[0], hit_point[2] }, actor_position_2d, normal_2d);
                        hf_vec2f_normalize(normal_2d, normal_2d);
                        hf_vec3f_copy((hf_vec3f) { normal_2d[0], 0.f, normal_2d[1] }, result.normal);
                    }
                }
            }
        }
    }

    if(out_result) {
        *out_result = result;
    }
    return hit;
}

void oe_world_resolve_collision_walls(oe_world* world, hf_vec3f actor_position, float actor_radius, float actor_height, float actor_step) {
    for(size_t i = 0; i < world->walls_count; i++) {
        oe_wall* wall = &world->walls[i];
        oe_wall_resolve_collision(wall, actor_position, actor_radius, actor_height, actor_step);
    }
}

void oe_world_move_actor(oe_world* world, oe_actor* actor, hf_vec3f position) {
    hf_vec3f_copy(position, actor->position);
    actor->position[1] = oe_world_height_at_step(world, actor->position, 0.5f);
    oe_world_resolve_collision_walls(world, actor->position, actor->radius, actor->height, 0.3f);
}

void oe_world_update_activators(oe_world* world, float delta) {
    for(size_t i = 0; i < world->interactables_count; i++) {
        world->interactables[i].action = oe_interactable_action_none;
    }

    for(size_t i = 0; i < world->activators_count; i++) {
        oe_activator* activator = &world->activators[i];
        if(!activator->interactable) {
            continue;
        }

        for(size_t j = 0; j < world->actors_count; j++) {
            oe_actor* actor = &world->actors[j];
            hf_vec3f diff;
            hf_vec3f_subtract(actor->position, activator->position, diff);
            if(hf_vec3f_square_magnitude(diff) <= 1.f) {
                activator->interactable->action = oe_interactable_action_activate;
            }
        }
        if(activator->interactable->action == oe_interactable_action_none && activator->revert) {
            activator->interactable->action = oe_interactable_action_revert;
        }
    }

    for(size_t i = 0; i < world->interactables_count; i++) {
        oe_interactable* interactable = &world->interactables[i];
        switch(interactable->action) {
            case oe_interactable_action_activate:
                oe_interactable_set(world, interactable, interactable->slide + delta / 2.25f);
                break;
            case oe_interactable_action_revert:
                oe_interactable_set(world, interactable, interactable->slide - delta / 2.25f);
                break;
            default:
                break;
        }
    }
}

static void internal_oe_world_draw_mesh_data(oe_mesh_data* mesh_data, hf_vec3f position, hf_vec3f rotation, hfe_shader_property model_property) {
    hf_vec3f pos;
    hf_vec3f_add(position, mesh_data->offset, pos);

    hf_mat4f mat_t;
    hf_transform3f_translation(pos, mat_t);

    hf_mat4f mat_rx;
    hf_transform3f_rotation_x(rotation[0], mat_rx);
    hf_mat4f mat_ry;
    hf_transform3f_rotation_y(rotation[1], mat_ry);
    hf_mat4f mat_rz;
    hf_transform3f_rotation_z(rotation[2], mat_rz);

    hf_mat4f mat;
    hf_mat4f_identity(mat);
    hf_mat4f_multiply_mat4f(mat, mat_t, mat);
    hf_mat4f_multiply_mat4f(mat, mat_rx, mat);
    hf_mat4f_multiply_mat4f(mat, mat_ry, mat);
    hf_mat4f_multiply_mat4f(mat, mat_rz, mat);
    hfe_shader_property_set_mat4f(model_property, mat[0]);

    hfe_mesh_use(mesh_data->mesh);
    hfe_mesh_draw();
}

void oe_world_draw(oe_world* world) {// TODO: esta função não deveria calcular diretamente o uniform de modelo? Usar um batcher talvez??
    hfe_shader_property property_model = hfe_shader_property_get("uniform_Model");
    for(size_t i = 0; i < world->props_count; i++) {
        oe_prop* prop = &world->props[i];
        internal_oe_world_draw_mesh_data(&prop->mesh_data, prop->position, prop->rotation, property_model);
    }
    for(size_t i = 0; i < world->actors_count; i++) {
        oe_actor* actor = &world->actors[i];
        internal_oe_world_draw_mesh_data(&world->actor_mesh, actor->position, (hf_vec3f) { 0.f, 0.f, 0.f }, property_model);
    }
    for(size_t i = 0; i < world->activators_count; i++) {
        oe_activator* activator = &world->activators[i];
        internal_oe_world_draw_mesh_data(&world->activator_mesh, activator->position, (hf_vec3f) { 0.f, 0.f, 0.f }, property_model);
    }
}
