#version 460
in vec2 frag_TexCoord;

uniform sampler2D uniform_Texture;
uniform float uniform_Time;

out vec4 out_Color;

void main() {
    vec2 coord = frag_TexCoord;
    float zoom = 0.8;
    coord *= zoom;
    coord += vec2((1.0 - zoom) * 0.5);

    vec2 diff = frag_TexCoord - vec2(0.5);
    float intensity = diff.x * diff.x + diff.y * diff.y;
    vec2 distortion = diff * intensity * 0.6;

    vec2 finalCoord = coord + distortion;
    if(finalCoord.x > 1.0 || finalCoord.x < 0.0 || finalCoord.y > 1.0 || finalCoord.y < 0.0) {
        out_Color = vec4(vec3(0.0), 1.0);
        return;
    }

    vec4 tex = texture(uniform_Texture, coord + distortion);
    float avg = (tex.r + tex.g + tex.b) / 3.0;
    int avgi = int(avg * 255.0);
    avgi /= 30;
    avgi *= 30;
    out_Color = vec4(vec3(float(avgi) / 255.0), 1.0);
    out_Color = tex;
    out_Color.rgb *= 1.0 - (0.1 * (1.0 + sin((finalCoord.y + uniform_Time * 0.05) * 20.0)));
    int px = int(finalCoord.x * 400.0);
    int py = int(finalCoord.y * 400.0);

    int ppx = px / 2;
    if(ppx % 3 == 0) {
        out_Color.gb *= 0.9;
    }
    else if((ppx + 1) % 3 == 0) {
        out_Color.rb *= 0.9;
    }
    else {
        out_Color.rg *= 0.9;
    }
    if(py % 4 == 0 || px % 2 == 0) {
        out_Color.rgb *= 0.6;
    }
    out_Color.rgb *= 1.5;
}
