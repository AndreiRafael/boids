#version 460
in vec2 frag_TexCoord;
in vec3 frag_Normal;
out vec4 out_Color;

uniform sampler2D uniform_Texture;

void main() {
    vec2 coord = vec2(frag_TexCoord.x * (1.0 / 3.0), frag_TexCoord.y);

    out_Color = texture(uniform_Texture, coord);
    out_Color.rgb += 0.5 * (1.0 - out_Color.a);
    out_Color.a = 1.0;
    vec3 light_dir = normalize(vec3(0.1, -0.7, -0.3f));
    float light_intensity = max(dot(-light_dir, frag_Normal), 0.0);
    out_Color.rgb *= mix(0.6, 1.0, light_intensity);
}