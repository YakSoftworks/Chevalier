#version 450

// Cut from Vulkan-Samples

precision highp float;

layout(input_attachment_index = 0, set = 0, binding = 0) uniform subpassInput i_depth;
layout(input_attachment_index = 1, set = 0, binding = 1) uniform subpassInput i_albedo;
layout(input_attachment_index = 2, set = 0, binding = 2) uniform subpassInput i_normal;



layout(std140,binding = 0, set = 1) uniform GlobalDataObject {
    mat4 proj;
    mat4 view;
    mat4 projViewMat;
    mat4 inverseProjViewMat;
    float timeSinceStart;
	uint numLights;
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
	vec4 additional;   // additional.w represents light type
};

layout(std140, binding = 2, set = 1) readonly buffer lightDataArray {
    LightData lightArray[];
} PerLightData;


layout(location = 0) in vec2 in_uv;
layout(location = 0) out vec4 o_color;

vec3 apply_directional_light(LightData light, vec3 normal)
{
	vec3 lightPos = (light.transform * vec4(0.f, 0.f, 0.f, 1.f)).xyz;

	vec3 lightDir = (light.transform * vec4(0.f, 0.f, 1.f, 0.f)).xyz;

	float fDot = clamp(dot(normal, lightDir), 0.0, 1.0);
	return vec3(light.color.xyz * (fDot * light.color.w));
}

vec3 apply_point_light(LightData light, vec3 pos, vec3 normal)
{

	vec3 lightPos = (light.transform * vec4(0.f, 0.f, 0.f, 1.f)).xyz;
    vec3 world_to_light = normalize(lightPos - pos);

	float dist = length(world_to_light);
    float atten = 1.0 / (dist * dist);

    float ndotl = clamp(dot(normal, world_to_light), 0.0, 1.0);
    return light.color.xyz * ((ndotl * light.color.w) * atten);

}

vec3 apply_spot_light(LightData light, vec3 pos, vec3 normal)
{
	vec3 light_to_pixel = pos - (light.transform*vec4(0.f, 0.f, 0.f, 1.f)).xyz;

	float dist = length(light_to_pixel);

	if(dist > light.additional.z){
		return vec3(0.f, 0.f, 0.f);
	}

	vec3 light_to_pixel_norm = normalize(light_to_pixel);

	vec3 light_dir = (light.transform*vec4(0.f, 0.f, 1.f, 0.f)).xyz;

    float theta = dot(light_to_pixel_norm, normalize(light_dir));
    float inner_cone_angle = light.additional.x;
    float outer_cone_angle = light.additional.y;
    float intensity = (theta - outer_cone_angle) / (inner_cone_angle - outer_cone_angle);
    return light.color.xyz * (smoothstep(0.0, 1.0, intensity) * light.color.w);

}

void main() {
	
	vec3 L = vec3(0.f);

	vec4 albedo = subpassLoad(i_albedo);
	vec4 norm = subpassLoad(i_normal);
	vec4 depth = subpassLoad(i_depth);

	LightData currentLight;

	vec4 clip = vec4((in_uv * 2.0) - vec2(1.0), subpassLoad(i_depth).x, 1.0);
    vec4 world_w = Globals.inverseProjViewMat * clip;
    vec3 pos = world_w.xyz / vec3(world_w.w);


	for(int i=0; i < Globals.numLights; i++){
		currentLight = PerLightData.lightArray[i];
		
		if(currentLight.additional.w == 0.f){
			// Point Light
			L += apply_point_light(currentLight, pos, norm.xyz);
		}
		else if (currentLight.additional.w == 1.f){
			// Spot Light
			L += apply_spot_light(currentLight, pos, norm.xyz);
			
		}
		else if (currentLight.additional.w == 2.f){
			// Directional Light
			L += apply_directional_light(currentLight, norm.xyz);

		}

	
	}

	//o_color = vec4(1.0f, 0.f, 1.0f, 1.0f);
	o_color = vec4(L, 1.0f);

}

