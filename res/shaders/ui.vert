#version 460
layout(location = 0)in vec2 vert_Position;
layout(location = 0)in vec2 vert_TexCoord;

uniform mat3 uniform_Screen;

out vec2 frag_TexCoord;

void main() {
    gl_Position = vec4(vec3(vert_Position, 1.0) * uniform_Screen, 1.0);

    frag_TexCoord = vert_TexCoord;
}
