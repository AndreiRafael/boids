#ifndef HF_SHAPE_H
#define HF_SHAPE_H

#include <stdbool.h>
#include <stddef.h>

#include "hf_vec.h"

//returns true if point is inside a convex polygon described by clockwise ordered points.
bool hf_shape_point_inside_polygon_convex(hf_vec2f* points, size_t points_count, hf_vec2f point);

//returns true if circle of given center and radius intersects with convex polygon described by clockwise ordered points.
bool hf_shape_circle_intersects_polygon_convex(hf_vec2f* points, size_t points_count, hf_vec2f center, float radius);

#endif//HF_SHAPE_H
