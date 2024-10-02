#version 460
in vec2 frag_Position;

uniform vec3 u_Color;

out vec4 out_Color;

void main() {
    out_Color = vec4(u_Color, 1.0);
}
