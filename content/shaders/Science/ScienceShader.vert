#version 460

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(std140,binding = 0, set = 0) uniform GlobalDataObject {
    mat4 proj;
    mat4 view;
    mat4 projViewMat;
    mat4 debugModelMat;
} Globals;

struct ObjectData{
	mat4 model;
};

layout(std140,binding = 1, set = 0) readonly buffer objectDataArray {
    ObjectData objectArray[];
} PerObjectData;

layout(std140, push_constant ) uniform constants
{
    int objectID;
} PushConstants;


layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main() {

    // Set Y to our value

    inPosition.y = (x**2) + (z**2)

    ObjectData thisObjectData = PerObjectData.objectArray[PushConstants.objectID];

    gl_Position = Globals.projViewMat * thisObjectData.model * vec4(inPosition, 1.0f);
  
    fragColor = inColor;
    fragTexCoord = inTexCoord;




}