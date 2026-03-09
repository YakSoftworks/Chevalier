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


struct LightData{
	mat4 transform;
	vec4 color;        // color.w represents light intensity
	vec4 additional;   // additional.x represents light type
	uint lightType;
};

layout(std140,binding = 2, set = 1) readonly buffer lightDataArray {
    LightData lightArray[];
} PerLightData;


layout(std140, push_constant ) uniform constants
{
    mat4 LightTransform;
    uint lightID;

} PushConstants;

layout(input_attachment_index = 0, set = 0, binding = 0) uniform subpassInput i_depth;
layout(input_attachment_index = 1, set = 0, binding = 1) uniform subpassInput i_albedo;
layout(input_attachment_index = 2, set = 0, binding = 2) uniform subpassInput i_normal;
layout(binding = 3, set = 0) uniform sampler2D i_shadowDepth;

layout(location = 0) in vec2 in_uv;

layout(location = 0) out vec4 o_color;

void main() {

    // Compute xyz of fragment
    float depth = subpassLoad(i_depth).r;

    vec4 worldSpacePos = ((proj * PushConstants.lightTransform * vec4(in_uv, depth, 1.f)).xyz, 1.f);


    // Get u,v,d from ProjxLightx(x,y,z,1)
    vec4 lightSpaceCoords = Globals.proj * PushConstants.LightTransform * worldSpacePos;

    // d' = sample(shadowDepth, u,v)
    float d2 = texture(i_shadowDepth, lightSpaceCoords.xy).r;


    // if d == d' -> Apply Light
    if(d2 == worldSpacePos.z){

        // Apply Light
        o_color = vec4(PerLightData.lightArray[PushConstants., 1.f);

    }

}