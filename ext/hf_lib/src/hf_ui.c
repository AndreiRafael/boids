#include "../include/hf_ui.h"
#include "../include/hf_transform.h"

#include <stdlib.h>

void hf_ui_element_reset(hf_ui_element* element) {
    element->parent = NULL;
    hf_vec2f_copy((hf_vec2f) { 0.f, 0.f }, element->position);
    hf_vec2f_copy((hf_vec2f) { 1.f, 1.f }, element->size);
    hf_vec2f_copy((hf_vec2f) { 0.5f, 0.5f }, element->pivot);
    hf_vec2f_copy((hf_vec2f) { 0.5f, 0.5f }, element->anchor);
    element->rotation = 0.f;
}

static void internal_hf_ui_element_transform_base(hf_ui_element* element, hf_mat3f out_matrix) {
    hf_mat3f aux;
    if(element->parent) {
        internal_hf_ui_element_transform_base(element->parent, out_matrix);
        hf_transform2f_translation((hf_vec2f) {
            (element->anchor[0] - element->parent->pivot[0]) * element->parent->size[0],
            (element->anchor[1] - element->parent->pivot[1]) * element->parent->size[1]
        }, aux);
        hf_mat3f_multiply_mat3f(out_matrix, aux, out_matrix);
    }
    else {
        hf_mat3f_identity(out_matrix);
    }
    hf_transform2f_translation(element->position, aux);//position transformation
    hf_mat3f_multiply_mat3f(out_matrix, aux, out_matrix);
    hf_transform2f_rotation(element->rotation, aux);
    hf_mat3f_multiply_mat3f(out_matrix, aux, out_matrix);
}

void hf_ui_element_transform(hf_ui_element* element, hf_mat3f out_matrix) {
    internal_hf_ui_element_transform_base(element, out_matrix);
    hf_mat3f aux;
    hf_transform2f_scale((hf_vec2f) { element->size[0], element->size[1] }, aux);//size transformation
    hf_mat3f_multiply_mat3f(out_matrix, aux, out_matrix);
    hf_transform2f_translation((hf_vec2f) { -element->pivot[0], -element->pivot[1] }, aux);//pivot transformation
    hf_mat3f_multiply_mat3f(out_matrix, aux, out_matrix);
}

void hf_ui_element_transform_canvas(hf_ui_element* element, hf_mat3f canvas_transform, hf_mat3f out_matrix) {
    hf_ui_element_transform(element, out_matrix);
    hf_mat3f_multiply_mat3f(canvas_transform, out_matrix, out_matrix);
}

bool hf_ui_element_is_point_inside(hf_ui_element* element, hf_vec2f point) {
    hf_mat3f transform;
    hf_ui_element_transform(element, transform);

    hf_vec2f corners[4] = {
        { 0.f, 0.f },
        { 1.f, 0.f },
        { 1.f, 1.f },
        { 0.f, 1.f },
    };
    for(int i = 0; i < 4; i++) {
        hf_transform2f_apply(corners[i], transform, corners[i]);
    }

    for(int i = 0; i < 4; i++) {
        hf_vec2f edge;
        hf_vec2f_subtract(corners[i], corners[(i + 1) % 4], edge);
        hf_vec2f normal = { edge[1], -edge[0] };

        hf_vec2f direction;
        hf_vec2f_subtract(corners[i], point, direction);
        if(hf_vec2f_dot(direction, normal) > 0.f) {
            return false;
        }
    }
    return true;
}


void hf_ui_canvas_transform(hf_ui_canvas* canvas, hf_mat3f out_matrix) {
    hf_mat3f_identity(out_matrix);
    out_matrix[0][0] = (canvas->right - canvas->left) / canvas->w;
    out_matrix[1][1] = (canvas->top - canvas->bottom) / canvas->h;
    out_matrix[0][2] = canvas->left;
    out_matrix[1][2] = canvas->bottom;
}
