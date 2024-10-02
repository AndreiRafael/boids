#version 460
layout(location = 0)in vec2 vert_Position;

uniform mat4 u_Model;
uniform mat4 u_Projection;

out vec2 frag_Position;

void main() {
    frag_Position = (vec4(vert_Position, 0.0, 1.0) * u_Model).xy;

    gl_Position = vec4(vert_Position, 0.0, 1.0) * u_Model * u_Projection;
}
