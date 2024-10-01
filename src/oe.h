#ifndef OE_H
#define OE_H

#define OE_PLATFORM_MAX_VERTICES 10

#define OE_INTERACTABLE_MAX_WALLS 10
#define OE_INTERACTABLE_MAX_PLATFORMS 4

#include <stddef.h>
#include <stdbool.h>

#include "hf_lib/hf_vec.h"
#include "hfe.h"

//Plataforma sobre a qual uma personagem pode andar. Deve ser descrita como um polígono plano em sentido anti-horário e sua normal precisa minimamente apontar para cima.
typedef struct oe_platform_s {
    hf_vec3f vertices[OE_PLATFORM_MAX_VERTICES];//the 3d positions of the mesh
    hf_vec2f points[OE_PLATFORM_MAX_VERTICES];
    hf_vec3f normal;// Vetor normal pré-calculado. Padrão é (0, 1, 0), o valor é calculado quando um terceiro vértice é adicionado
    hf_vec3f slopex;
    size_t vertices_count;
} oe_platform;

void oe_platform_init(oe_platform* platform);
void oe_platform_add_vertex(oe_platform* platform, hf_vec3f vertex);
bool oe_platform_is_inside(oe_platform* platform, hf_vec3f position);
float oe_platform_height_at(oe_platform* platform, hf_vec3f position);//Calculates the height of the slope at the given position

typedef struct oe_platform_data_s {
    oe_platform platforms[4];//máximo de 4 plataformas por malha
    size_t platforms_count;
} oe_platform_data;

typedef struct oe_mesh_data_s {
    hfe_mesh mesh;
    hf_vec3f offset;
} oe_mesh_data;

typedef struct oe_wall_point_s {
    hf_vec3f position;
    float height;
} oe_wall_point;

typedef struct oe_wall_s {
    oe_wall_point start;
    oe_wall_point end;
} oe_wall;

void oe_wall_resolve_collision(oe_wall* wall, hf_vec3f actor_position, float actor_radius, float actor_height, float actor_step);//Resolve a colisão de um ator com uma parede, modificando actor_position, se necessário

typedef struct oe_wall_data_s {
    oe_wall walls[4];//máximo de 4 paredes por malha
    size_t walls_count;
} oe_wall_data;

typedef struct oe_prop_s {
    oe_mesh_data mesh_data;
    hf_vec3f position;
    hf_vec3f rotation;
} oe_prop;

void oe_prop_draw(oe_prop* prop);

typedef enum oe_interactable_type_e {
    oe_prop_type_static = -1,
    oe_prop_type_dynamic_elevator,
    oe_prop_type_dynamic_bridge,
    oe_prop_type_dynamic_gate,
    oe_prop_type_player,
    oe_prop_type_activator_permanent,
    oe_prop_type_activator_hold,
} oe_prop_type;

typedef enum oe_interactable_action_e {
    oe_interactable_action_none = 0,
    oe_interactable_action_activate,
    oe_interactable_action_revert,
} oe_interactable_action;

typedef struct oe_world_s oe_world;
typedef struct oe_interactable_s {
    oe_prop_type type;
    float slide;
    oe_interactable_action action;

    oe_prop* prop;
    hf_vec3f position_base;

    oe_platform* platforms[OE_INTERACTABLE_MAX_PLATFORMS];
    oe_platform platforms_base[OE_INTERACTABLE_MAX_PLATFORMS];//versão base das plataformas, para referência das transformações
    size_t platforms_count;

    oe_wall* walls[OE_INTERACTABLE_MAX_WALLS];
    oe_wall walls_base[OE_INTERACTABLE_MAX_WALLS];//versão base das paredes, para referência das transformações
    size_t walls_count;
} oe_interactable;

void oe_interactable_init(oe_interactable* interactable, oe_prop_type type);
void oe_interactable_set_prop(oe_interactable* interactable, oe_prop* prop);
void oe_interactable_add_wall(oe_interactable* interactable, oe_wall* wall);
void oe_interactable_add_platform(oe_interactable* interactable, oe_platform* platform);
void oe_interactable_set(oe_world* world, oe_interactable* interactable, float value);

typedef struct oe_actor_s {
    hf_vec3f position;
    float height;
    float radius;
} oe_actor;

typedef struct oe_activator_s {
    hf_vec3f position;
    oe_interactable* interactable;
    bool revert;
} oe_activator;

typedef struct oe_database_entry_s {
    oe_mesh_data mesh;
    oe_platform_data platform;
    oe_wall_data wall;
} oe_database_entry;

typedef enum oe_raycast_mask_e {
    oe_raycast_mask_platform = 1,
    oe_raycast_mask_wall     = 2,
    oe_raycast_mask_prop     = 3,
    oe_raycast_mask_player   = 4,
    oe_raycast_mask_zombie   = 8,
    oe_raycast_mask_actor    = 12,//both player and zombies
    oe_raycast_mask_all      = 15,
} oe_raycast_mask;

typedef struct oe_raycast_result_s {
    hf_vec3f position;
    hf_vec3f normal;
    float distance;
} oe_raycast_result;

//Agrupa os componentes do mundo de jogo, como plataformas, paredes e personagens, para facilitar a interação entre os componentes.
struct oe_world_s {
    oe_mesh_data actor_mesh;
    oe_mesh_data activator_mesh;
    oe_database_entry* database;
    size_t database_count;

    oe_platform* platforms;
    size_t platforms_count;
    oe_prop* props;
    size_t props_count;
    oe_wall* walls;
    size_t walls_count;
    oe_interactable* interactables;
    size_t interactables_count;
    oe_actor* actors;
    size_t actors_count;
    oe_activator* activators;
    size_t activators_count;
};

bool oe_world_init(oe_world* world);//Inicializa elementos, como meshes e plataformas padrão
void oe_world_deinit(oe_world* world);//Desativa elementos de mesh e plataformas do mundo
void oe_world_load(oe_world* world, const char* path);//carrega dados do mapa de um arquivo .omp
void oe_world_unload(oe_world* world);//descarrega dados do mapa, deixando-o vazio

oe_platform* oe_world_add_platform(oe_world* world, oe_platform* platform_blueprint, hf_vec3f position, hf_vec3f rotation);//adiciona uma plataforma ao mundo na posição e rotação desejada
oe_prop* oe_world_add_prop(oe_world* world, oe_mesh_data mesh, hf_vec3f position, hf_vec3f rotation);
oe_wall* oe_world_add_wall(oe_world* world, oe_wall* blueprint, hf_vec3f position, hf_vec3f rotation);
oe_interactable* oe_world_add_interactable(oe_world* world, oe_prop_type type);
oe_actor* oe_world_add_actor(oe_world* world, hf_vec3f position);
oe_activator* oe_world_add_activator(oe_world* world, hf_vec3f position, bool hold);

float oe_world_height_at_max(oe_world* world, hf_vec3f position);
float oe_world_height_at_step(oe_world* world, hf_vec3f position, float step);
bool oe_world_raycast(oe_world* world, hf_vec3f position, hf_vec3f direction, oe_raycast_mask mask, oe_raycast_result* out_result);
void oe_world_resolve_collision_walls(oe_world* world, hf_vec3f actor_position, float actor_radius, float actor_height, float actor_step);

void oe_world_move_actor(oe_world* world, oe_actor* actor, hf_vec3f position);

void oe_world_update_activators(oe_world* world, float delta);

void oe_world_draw(oe_world* world);


typedef struct oe_nav_data_s {
    oe_world* world;
    hf_vec3i dimensions;
    hf_vec3f* target;
} oe_nav_data;

#endif//OE_W
