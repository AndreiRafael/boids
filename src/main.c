#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "glad/glad.h"
#include "sdl2/SDL.h"
#include "stb/stb_image.h"
#include "hf_lib/hf_mat.h"
#include "hf_lib/hf_transform.h"
#include "hf_lib/hf_shape.h"
#include "hf_lib/hf_string.h"
#include "hf_lib/hf_ui.h"

#include "hfe.h"
#include "boids.h"

#define WINDOW_W 800
#define WINDOW_H 800

#define BOIDS_COUNT 100

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "boids",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_W, WINDOW_H,
        SDL_WINDOW_OPENGL
    );

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GLContext gl_context =  SDL_GL_CreateContext(window);
    if(!gl_context) {
        return EXIT_FAILURE;
    }

    gladLoadGLLoader(SDL_GL_GetProcAddress);
    hfe_init_opengl();

    glEnable(GL_BLEND);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float verts[] = {
        -0.0f, -0.2f,
         0.5f, -0.5f,
         0.0f,  0.5f,
        -0.5f, -0.5f,
    };
    unsigned short tris[] = {
        0, 1, 2,
        0, 2, 3,
    };
    hfe_mesh mesh = hfe_mesh_create_indexed_f(verts, 4, tris, 2, hfe_vertex_spec_width_two);

    hfe_shader vert_shader = hfe_shader_create_from_file(hfe_shader_type_vertex, "./res/shaders/shader.vert");
    hfe_shader frag_shader = hfe_shader_create_from_file(hfe_shader_type_fragment, "./res/shaders/shader.frag");
    hfe_shader_program program = hfe_shader_program_create((hfe_shader[]){ vert_shader, frag_shader }, 2);
    hfe_shader_program_use(program);
    hfe_shader_destroy(vert_shader);
    hfe_shader_destroy(frag_shader);

    hf_vec2f world_size = { WINDOW_W / 15, WINDOW_H / 15 };
    boids_set_bounds(-world_size[0] / 2.f, -world_size[1] / 2.f, world_size[0] / 2.f, world_size[1] / 2.f);

    srand((unsigned int)time(NULL));
    boid boids[BOIDS_COUNT] = { 0 };
    for(size_t i = 0; i < BOIDS_COUNT; i++) {
        hf_vec2f vel;
        vel[0] = (float)((rand() % 101) - 50) / 50.f;
        vel[1] = (float)((rand() % 101) - 50) / 50.f;
        hf_vec2f_copy(vel, boids[i].velocity);

        hf_vec2f pos;
        pos[0] = (float)((rand() % 1001) - 500);
        pos[1] = (float)((rand() % 1001) - 500);
        hf_vec2f_copy(pos, boids[i].position);

        if(i >= 3) {
            boids[i].id = rand() % 4;
        }
        else {
            boids[i].id = 4;
        }
    }

    SDL_GL_SetSwapInterval(1);
    bool quit = false;
    Uint64 ticks_zero = SDL_GetTicks64();
    Uint64 ticks_prev = ticks_zero;
    float fixed_time = 0.f;
    #define FIXED_DELTA (0.005f)
    while(!quit) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) {
                quit = true;
            }
            if(e.type == SDL_KEYDOWN) {
                if(e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    quit = true;
                }
            }
        }

        Uint64 ticks_new = SDL_GetTicks64();
        float delta = (float)(ticks_new - ticks_prev) / 1000.f;
        ticks_prev = ticks_new;

        fixed_time += delta;
        while(fixed_time > FIXED_DELTA) {
            fixed_time -= FIXED_DELTA;

            boids_update(boids, BOIDS_COUNT, FIXED_DELTA);
        }

        //render
        hf_mat4f mat_proj_ortho;
        hf_transform3f_projection_orthographic_size(world_size[0], world_size[1], -100.f, 100.f, mat_proj_ortho);

        hfe_shader_program_use(program);
        hfe_shader_property_set_mat4f(hfe_shader_property_get("u_Projection"), mat_proj_ortho[0]);


        glClearColor(.3f, .4f, .7f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);

        boids_draw(boids, BOIDS_COUNT, mesh);

        SDL_GL_SwapWindow(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
