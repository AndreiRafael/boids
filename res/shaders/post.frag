#version 460
in vec2 frag_TexCoord;

uniform sampler2D uniform_Texture;

out vec4 out_Color;

void main() {
    out_Color = texture(uniform_Texture, frag_TexCoord);
}
