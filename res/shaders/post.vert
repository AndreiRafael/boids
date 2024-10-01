#version 460
layout(location = 0)in vec2 vert_Position;
layout(location = 0)in vec2 vert_TexCoord;

out vec2 frag_TexCoord;

void main() {
    gl_Position = vec4((vert_Position * 2.0) - vec2(1.0), 0.0, 1.0);

    frag_TexCoord = vert_TexCoord;
}
