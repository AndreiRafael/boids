#ifndef HF_UI_H
#define HF_UI_H

#include <stdbool.h>

#include "hf_vec.h"
#include "hf_mat.h"

typedef struct hf_ui_element_s hf_ui_element;
struct hf_ui_element_s {
    hf_ui_element* parent;
    hf_vec2f position;
    hf_vec2f size;
    hf_vec2f anchor;
    hf_vec2f pivot;
    float rotation;
};

typedef struct hf_ui_canvas_s {
    float left;
    float right;
    float bottom;
    float top;
    float w;
    float h;
} hf_ui_canvas;

//Reset element values to default ones
void hf_ui_element_reset(hf_ui_element* element);
//calculates transformation matrix for the element
void hf_ui_element_transform(hf_ui_element* element, hf_mat3f out_matrix);
//calculates transformation matrix for the element, given a canvas transfomation matrix
void hf_ui_element_transform_canvas(hf_ui_element* element, hf_mat3f canvas_transform, hf_mat3f out_matrix);
//returns true if point is inside given element
bool hf_ui_element_is_point_inside(hf_ui_element* element, hf_vec2f point);

//calculates transformation matrix for the canvas
void hf_ui_canvas_transform(hf_ui_canvas* canvas, hf_mat3f out_matrix);

#endif//HF_UI_H
