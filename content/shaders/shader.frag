#version 450

//layout(binding = 2) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 inNorm;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outNormal;

void main() {

    outColor = vec4(fragColor, 1.0);
    outNormal = vec4(inNorm, 1.0);

}