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

	// References to all registered lights
	std::vector<LightComponent*> LightSourceComponents;

	uint32_t numLightSourceComponents;

public:
	// Tell each light to update their lighting buffer object
	void UpdateLightBuffer(LightShaderInfo* ShaderInfoArray);

	uint32_t getNumLightSourceComponents() { return numLightSourceComponents; }

};

