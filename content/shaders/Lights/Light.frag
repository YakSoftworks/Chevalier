#version 450

// Cut from Vulkan-Samples

precision highp float;

layout(input_attachment_index = 0, binding = 0) uniform subpassInput i_depth;
layout(input_attachment_index = 1, binding = 1) uniform subpassInput i_albedo;
layout(input_attachment_index = 2, binding = 2) uniform subpassInput i_normal;



layout(std140,binding = 0, set = 0) uniform GlobalDataObject {
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

layout(std140,binding = 1, set = 0) readonly buffer objectDataArray {
    ObjectData objectArray[];
} PerObjectData;


struct LightData{
	int lightType;
	mat4 transform;
	vec4 color;        // color.w represents light intensity
	vec3 additional;   // additional.x represents light type
};

layout(std140,binding = 2, set = 0) readonly buffer lightDataArray {
    LightData lightArray[];
} PerLightData;


layout(location = 0) in vec2 in_uv;

layout(location = 0) out vec4 o_color;

vec3 apply_directional_light(LightData light, vec3 normal)
{
	return vec3(0.0f);
}

vec3 apply_point_light(LightData light, vec3 pos, vec3 normal)
{
	return vec3(0.0f);
}

vec3 apply_spot_light(LightData light, vec3 pos, vec3 normal)
{
	return vec3(0.0f);
}

void main() {
	
	vec4 albedo = subpassLoad(i_albedo);


	o_color = vec4(albedo.rgb, 1.0);
	//o_color = vec4(1.0, 0.0, 1.0, 1.0);

}

