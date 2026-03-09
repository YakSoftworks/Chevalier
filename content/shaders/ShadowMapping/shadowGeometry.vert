#version 450

precision highp float;

layout(std140,binding = 0, set = 1) uniform GlobalDataObject {
    mat4 proj;
    mat4 view;
    mat4 projViewMat;
    mat4 debugModelMat;
    float timeSinceStart;
    int numLights;
} Globals;

struct ObjectData{
	mat4 model;
};

layout(std140,binding = 1, set = 1) readonly buffer objectDataArray {
    ObjectData objectArray[];
} PerObjectData;

layout(std140, push_constant ) uniform constants
{
    int objectID;
    mat4 lightTransform;
    uint lightID;

} PushConstants;

void main() {

    // Generate the depth of the vertex
    gl_Position = Globals.projViewMat * PushConstants.lightTransform * vec4(normalize(inPosition)*distance, 1.0f);

}