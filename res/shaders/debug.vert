#version 460
layout(location = 0)in vec3 vert_Position;
layout(location = 1)in vec2 vert_TexCoord;
layout(location = 2)in vec3 vert_Normal;

uniform mat4 uniform_Model;
uniform mat4 uniform_View;
uniform mat4 uniform_Projection;
uniform mat4 uniform_LightMat;

out float frag_LightDepth;
out vec2 frag_LightScreenPosition;
out vec2 frag_TexCoord;
out vec3 frag_Position;
out vec3 frag_Normal;

void main() {
    gl_Position = vec4(vert_Position, 1.0) * uniform_Model * uniform_View * uniform_Projection;

    frag_TexCoord = vert_TexCoord;
    frag_Position = (vec4(vert_Position, 1.0) * uniform_Model).xyz;
    frag_Normal = vert_Normal * mat3(transpose(inverse(uniform_Model)));

    vec4 lightPos = vec4(vert_Position, 1.0) * uniform_Model * uniform_LightMat;
    frag_LightDepth = (lightPos.z / lightPos.w) * 0.5 + 0.5;
    frag_LightScreenPosition = lightPos.xy / lightPos.w;
}
