#version 460
in float frag_LightDepth;
in vec2 frag_LightScreenPosition;
in vec2 frag_TexCoord;
in vec3 frag_Position;
in vec3 frag_Normal;
out vec4 out_Color;

uniform sampler2D uniform_Texture;
uniform sampler2D uniform_LightMap;
uniform vec3 uniform_Color;

float shadow_strength_offset(vec2 offset) {
    vec2 lightCoord = frag_LightScreenPosition * 0.5 + vec2(0.5) + offset;
    float mapDepth = texture(uniform_LightMap, lightCoord).r;
    if(frag_LightDepth > (mapDepth + 0.00025)) {
        return 1.0;
    }
    return 0.0;
}

float shadow_strength() {
    int steps = 2;
    float strength = 0.0;
    float offset_value = 0.00015;
    for(int x = -steps; x <= steps; x++) {
        for(int y = -steps; y <= steps; y++) {
            vec2 offset = vec2(float(x) * offset_value, float(y) * offset_value);
            strength += shadow_strength_offset(offset);
        }
    }
    int size = (steps * 2 + 1);
    return strength / float(size * size);
}

void main() {
    float px = abs(dot(vec3(1.0, 0.0, 0.0), frag_Normal));
    float py = abs(dot(vec3(0.0, 1.0, 0.0), frag_Normal));
    float pz = abs(dot(vec3(0.0, 0.0, 1.0), frag_Normal));
    vec2 cx = vec2(frag_Position.z, frag_Position.y) / 2.25;
    vec2 cy = vec2(frag_Position.x, frag_Position.z) / 2.25;
    vec2 cz = vec2(frag_Position.x, frag_Position.y) / 2.25;

    out_Color =
        texture(uniform_Texture, cx) * px +
        texture(uniform_Texture, cy) * py +
        texture(uniform_Texture, cz) * pz
    ;
    out_Color /= (px + py + pz);
    out_Color.rgb += 0.5 * (1.0 - out_Color.a);
    out_Color.a = 1.0;
    out_Color.rgb *= uniform_Color;
    vec3 light_dir = normalize(vec3(-0.577, -0.577, -0.577));
    float light_intensity = max(dot(-light_dir, normalize(frag_Normal)) + frag_Position.y * 0.1, 0.0);

    light_intensity = mix(light_intensity, 0.0, shadow_strength());

    out_Color.rgb *= mix(0.6, 1.0, light_intensity);
}
