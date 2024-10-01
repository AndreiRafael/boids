#version 460
in vec2 frag_TexCoord;

uniform sampler2D uniform_Texture;
out vec4 out_Color;

void main() {
    vec4 tex = texture(uniform_Texture, frag_TexCoord);
    out_Color = vec4(vec3(tex.r), 1.0);
}
