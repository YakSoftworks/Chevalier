
#version 460

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

layout(set = 0, binding = 0) uniform GlobalData {
    mat4 view;
    mat4 proj;

} globalData;

struct ObjectData{
    mat4 model;
};

//all object matrices
layout(std140,set = 0, binding = 1) readonly buffer ObjectBuffer{

	ObjectData objects[];
} objectBuffer;

void main() {

    mat4 modelMatrix = objectBuffer.objects[gl_BaseInstance].model;

    gl_Position = globalData.proj * globalData.view * modelMatrix * vec4(inPosition, 1.0);
  
    fragColor = inColor;
    fragTexCoord = inTexCoord;

}