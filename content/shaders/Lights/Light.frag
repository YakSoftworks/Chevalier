#version 460

// Cut from Vulkan-Samples

precision highp float;

layout(input_attachment_index = 0, binding = 0) uniform subpassInput i_depth;
layout(input_attachment_index = 1, binding = 1) uniform subpassInput i_albedo;
layout(input_attachment_index = 2, binding = 2) uniform subpassInput i_normal;

#include "ChevalierConstants.h"

struct Light
{
	vec4 position;         // position.w represents type of light
	vec4 color;            // color.w represents light intensity
	vec4 direction;        // direction.w represents range
	vec2 info;             // (only used for spot lights) info.x represents light inner cone angle, info.y represents light outer cone angle
};

vec3 apply_directional_light(Light light, vec3 normal)
{
	vec3 world_to_light = -light.direction.xyz;
	world_to_light      = normalize(world_to_light);
	float ndotl         = clamp(dot(normal, world_to_light), 0.0, 1.0);
	return ndotl * light.color.w * light.color.rgb;
}

vec3 apply_point_light(Light light, vec3 pos, vec3 normal)
{
	vec3  world_to_light = light.position.xyz - pos;
	float dist           = length(world_to_light) * 0.005;
	float atten          = 1.0 / (dist * dist);
	world_to_light       = normalize(world_to_light);
	float ndotl          = clamp(dot(normal, world_to_light), 0.0, 1.0);
	return ndotl * light.color.w * atten * light.color.rgb;
}

vec3 apply_spot_light(Light light, vec3 pos, vec3 normal)
{
	vec3  light_to_pixel   = normalize(pos - light.position.xyz);
	float theta            = dot(light_to_pixel, normalize(light.direction.xyz));
	float inner_cone_angle = light.info.x;
	float outer_cone_angle = light.info.y;
	float intensity        = (theta - outer_cone_angle) / (inner_cone_angle - outer_cone_angle);
	return smoothstep(0.0, 1.0, intensity) * light.color.w * light.color.rgb;
}


layout(binding = 0, set = 0) uniform LightingBufferObject {
	
	uint numPointLights;
	uint maxPointLights;

	uint numSpotLights;
	uint maxPointLights;

	uint numDirectionalLights;
	uint maxDirectionalLights;

} lightingInfo;

layout(binding = 1, set=0) uniform LightSourcesBuffer {

	Light PointLights[maxPointLights];
	Light SpotLights[maxSpotLights];
	Light DirectionalLights[maxDirectionalLights];


}





void main() {





	// Calculate lighting
	vec3 L = vec3(0.0);

	// For each light, add the light value
	for (uint i = 0; i < numDirectionalLights; i++)
	{
		L += apply_directional_light(lightingInfo.directionalLightData[i], normal);
	}

	for (uint i = 0; i < numPointLights; i++)
	{
		L += apply_point_light(lightingInfo.pointLightData[i], pos, normal);
	}

	for (uint i = 0; i < numSpotLights; i++)
	{
		L += apply_spot_light(lightingInfo.spotLightInfo[i], pos, normal);
	}

	vec3 ambient_color = vec3(0.2) * albedo.xyz;
	
	o_color = vec4(ambient_color + L * albedo.xyz, 1.0);


}

