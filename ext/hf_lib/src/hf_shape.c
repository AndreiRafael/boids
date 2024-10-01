#include "../include/hf_shape.h"
#include "../include/hf_line.h"

bool hf_shape_point_inside_polygon_convex(hf_vec2f* points, size_t points_count, hf_vec2f point) {
    if(points_count < 3) {
        return false;
    }

    for(size_t i = 0; i < points_count; i++) {
        hf_vec2f face;
        hf_vec2f_subtract(points[(i + 1) % points_count], points[i], face);
        hf_vec2f normal = { face[1], -face[0] };

        hf_vec2f vec;
        hf_vec2f_subtract(point, points[i], vec);

        if(hf_vec2f_dot(normal, vec) > 0.f) {
            return false;
        }
    }
    return true;
}

bool hf_shape_circle_intersects_polygon_convex(hf_vec2f* points, size_t points_count, hf_vec2f center, float radius) {
    if(hf_shape_point_inside_polygon_convex(points, points_count, center)) {
        return true;
    }

    float radius_squared = radius * radius;
    for(size_t i = 0; i < points_count; i++) {
        hf_vec2f point;
        hf_segment2f_closest_point(points[i], points[(i + 1) % points_count], center, point);

        hf_vec2f diff;
        hf_vec2f_subtract(center, point, diff);
        if(hf_vec2f_square_magnitude(diff) < radius_squared) {
            return true;
        }
    }
    return false;
}
