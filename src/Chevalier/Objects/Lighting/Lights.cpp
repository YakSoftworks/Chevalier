#include "Lights.h"

#include "LightingManager.h"

void LightComponent::RegisterLightSource()
{
	LightingManager::GetLightingManager()->RegisterLightObject(LightType, this);
}

void LightComponent::UnregisterLightSource()
{
	// Uhhhh, lets not worry about that yet!
}
