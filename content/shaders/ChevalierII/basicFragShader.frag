#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNorm;

layout(location = 0) out vec4 o_albedo;
layout(location = 1) out vec4 o_norm;

void main() {

    o_albedo = vec4(fragColor, 1.0f);
    o_norm = vec4(fragNorm, 1.0);


}