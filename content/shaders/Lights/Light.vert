#version 460


layout (location = 0) out vec2 outUV;

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


void main()
{
	outUV = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
	gl_Position = vec4(outUV * 2.0f - 1.0f, 0.0f, 1.0f);
}
