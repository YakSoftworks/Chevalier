#pragma once

#include "Core.h"
#include "Lights.h"


class LightingManager
{
public:
	LightingManager();

protected:

	static LightingManager* singletonManager;

public:

	static LightingManager* GetLightingManager() { return singletonManager; }




public:

	// Returns a uint index for the light
	uint32_t RegisterLightObject(LightSourceType LightType, LightComponent* component);;


protected:

	//Contains Vectors for each lighting type as specified by constants

	// Point Lights
	std::array<LightBufferObject, CHEVALIER_CONSTANTS_MAX_POINT_LIGHTS> PointLightArray;
	uint32_t numPointLights = 0;

	// Spot Lights
	std::array<LightBufferObject, CHEVALIER_CONSTANTS_MAX_SPOT_LIGHTS> SpotLightArray;
	uint32_t numSpotLights = 0;

	// Directional Lights
	std::array<LightBufferObject, CHEVALIER_CONSTANTS_MAX_DIRECTIONAL_LIGHTS> DirectionalLightArray;
	uint32_t numDirectionalLights = 0;

	// References to all registered lights
	std::vector<LightComponent*> LightSourceComponents;

protected:

	// Tell each light to update their lighting buffer object
	void UpdateAllLights();

public:

	void SendBufferObjectInfo(LightSourceType LightType, uint32_t index, const LightBufferObject& newBufferObject);
	

};

