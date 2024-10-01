#include <stdbool.h>
#include <stdlib.h>
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

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "pngtuber",
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
        -0.0f,  0.0f,
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

    hfe_mesh ui_mesh = hfe_mesh_create_primitive_quad_ui();

    hf_ui_element ui_main;
    hf_ui_element_reset(&ui_main);
    ui_main.pivot[0] = 0.f;
    ui_main.pivot[1] = 0.f;
    ui_main.size[0] = 800.f;
    ui_main.size[1] = 800.f;

    hf_ui_element ui_debug_quad;
    hf_ui_element_reset(&ui_debug_quad);
    ui_debug_quad.parent = &ui_main;
    ui_debug_quad.pivot[0] = 1.f;
    ui_debug_quad.pivot[1] = 0.f;
    ui_debug_quad.anchor[0] = 1.f;
    ui_debug_quad.anchor[1] = 0.f;
    ui_debug_quad.size[0] = (float)(WINDOW_W / 4);
    ui_debug_quad.size[1] = (float)(WINDOW_W / 4);

    hf_ui_canvas canvas = {
        .left = -1,
        .right = 1,
        .top = 1,
        .bottom = -1,
        .w = WINDOW_W,
        .h = WINDOW_H,
    };

    float value = 0.f;

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
            boids_update(NULL, 0, FIXED_DELTA);
        }

        //render
        hf_mat4f mat_proj_ortho;
        hf_transform3f_projection_orthographic_size(WINDOW_W / 10, WINDOW_H / 10, -100.f, 100.f, mat_proj_ortho);

        hf_mat4f mat_view_cam;
        hf_vec3f cam_direction = { 0.0f, -.707f, -.707f };
        //hf_vec3f cam_direction = { 0.0f, 0.f, -1.f };
        hf_transform3f_view(cam_position, cam_direction, (hf_vec3f){ 0.f,  1.f, 0.f }, mat_view_cam);
        hf_mat4f mat_view_light;
        hf_vec3f light_position;
        hf_vec3f_add(actor ? actor->position : (hf_vec3f) { 0.0f, 0.0f, 0.0f }, (hf_vec3f){ 30.0f, 20.f, 20.0f }, light_position);
        hf_transform3f_view(light_position, (hf_vec3f){ -.577f, -.577f, -.577f }, (hf_vec3f){ 0.f,  1.f, 0.f }, mat_view_light);


        hf_mat4f mat_light;
        hf_mat4f_multiply_mat4f(mat_proj_ortho, mat_view_light, mat_light);

        for(size_t i = 0; i < sizeof(targets) / sizeof(targets[0]); i++) {
            hfe_render_target_use(*targets[i].target);
            glClearColor(0.f, .5f, .5f, 1.f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if(targets[i].light) {
                glViewport(0, 0, WINDOW_W * 4, WINDOW_H * 4);
            }
            else {
                glViewport(0, 0, WINDOW_W, WINDOW_H);
            }

            hfe_shader_program_use(program_3d);
            hfe_texture_use(texture, 0);
            hfe_shader_property_set_1i(hfe_shader_property_get("uniform_Texture"), 0);
            {//projection
                hf_mat4f proj;
                hf_mat4f_copy(targets[i].light ? mat_proj_ortho : mat_proj_persp, proj);
                hfe_shader_property_set_mat4f(hfe_shader_property_get("uniform_Projection"), proj[0]);
            }
            hfe_shader_property_set_mat4f(hfe_shader_property_get("uniform_View"), targets[i].light ? mat_view_light[0] : mat_view_cam[0]);

            hfe_texture_use(render_target_depth.texture, 1);
            hfe_shader_property_set_1i(hfe_shader_property_get("uniform_LightMap"), 1);
            hfe_shader_property_set_mat4f(hfe_shader_property_get("uniform_LightMat"), mat_light[0]);

            glEnable(GL_DEPTH_TEST);
            hf_vec2f mouse_position;
            hf_vec3f ray_position;
            hf_vec3f ray_dir;
            {
                int mx;
                int my;
                SDL_GetMouseState(&mx, &my);
                mouse_position[0] = ((float)mx / (float)WINDOW_W) - .5f;
                mouse_position[1] = -((float)my / (float)WINDOW_H) + .5f;
            }
            hf_vec3f_copy(cam_position, ray_position);
            {
                hf_vec3f cam_right;
                hf_vec3f_cross(cam_direction, (hf_vec3f) { 0.f, 1.f, 0.f }, cam_right);
                hf_vec3f_normalize(cam_right, cam_right);

                hf_vec3f cam_up;
                hf_vec3f_cross(cam_right, cam_direction, cam_up);
                hf_vec3f_normalize(cam_up, cam_up);

                hf_vec3f_multiply(cam_right, mouse_position[0], cam_right);
                hf_vec3f_multiply(cam_up, mouse_position[1], cam_up);

                hf_vec3f_add(ray_position, cam_direction, ray_position);
                hf_vec3f_add(ray_position, cam_right, ray_position);
                hf_vec3f_add(ray_position, cam_up, ray_position);
                hf_vec3f_subtract(ray_position, cam_position, ray_dir);
                hf_vec3f_normalize(ray_dir, ray_dir);
            }
            hfe_shader_property property_color = hfe_shader_property_get("uniform_Color");
            hfe_shader_property_set_3f(property_color, 1.f, 1.f, 1.f);
            oe_world_draw(&world);
            oe_raycast_result ray_result;
            if(oe_world_raycast(&world, ray_position, ray_dir, oe_raycast_mask_all, &ray_result)) {
                hf_vec3f_multiply(ray_result.normal, .1f, ray_result.normal);
                hf_vec3f hit_position;
                hf_vec3f_add(ray_result.position, ray_result.normal, hit_position);
                hfe_shader_property property_model = hfe_shader_property_get("uniform_Model");

                hf_mat4f mat_s;
                hf_transform3f_scale((hf_vec3f) { .2f, .05f, .2f }, mat_s);

                hf_mat4f mat_t;
                hf_transform3f_translation(ray_result.position, mat_t);
                hf_mat4f_multiply_mat4f(mat_t, mat_s, mat_t);
                hfe_shader_property_set_mat4f(property_model, mat_t[0]);
                hfe_shader_property_set_3f(property_color, 1.f, 0.f, 0.f);
                hfe_mesh_draw();

                hf_transform3f_translation(hit_position, mat_t);
                hf_mat4f_multiply_mat4f(mat_t, mat_s, mat_t);
                hfe_shader_property_set_mat4f(property_model, mat_t[0]);
                hfe_shader_property_set_3f(property_color, 0.f, 1.f, 0.f);
                hfe_mesh_draw();
            }
        }

        hfe_render_target_reset();
        glClearColor(0.f, .5f, .5f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);

        

        glDisable(GL_DEPTH_TEST);
        hfe_shader_program_use(program_2d);
        hfe_shader_property prop_2d = hfe_shader_property_get("uniform_Screen");
        hfe_mesh_use(ui_mesh);
        if(hfe_shader_property_valid(prop_2d)) {
            ui_debug_quad.rotation = value;
            hf_mat3f mat;
            hf_mat3f aux;
            hf_ui_canvas_transform(&canvas, aux);

            hf_ui_element_transform(&ui_debug_quad, mat);
            hf_mat3f_multiply_mat3f(aux, mat, mat);
            hfe_shader_property_set_mat3f(prop_2d, mat[0]);
            hfe_texture_use(render_target_depth.texture, 0);
            hfe_shader_property_set_1i(hfe_shader_property_get("uniform_Texture"), 0);
            hfe_mesh_draw();
        }

        SDL_GL_SwapWindow(window);
    }

    hfe_render_target_destroy(render_target);
    hfe_render_target_destroy(render_target_depth);

    oe_world_deinit(&world);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
