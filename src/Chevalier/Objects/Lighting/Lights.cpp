#include "Lights.h"

#include "LightingManager.h"


#pragma region Default Light Component

void LightComponent::PreInitializeComponent()
{


}

void LightComponent::InitializeComponent()
{
	//Register Component with Manager
	RegisterLightSource();

}

void LightComponent::PostInitializeComponent()
{


}

void LightComponent::RegisterLightSource()
{
	LightingManager::GetLightingManager()->RegisterLightObject(LightType, this);
}

void LightComponent::UnregisterLightSource()
{
	// Uhhhh, lets not worry about that yet!
}

#pragma endregion
#pragma region Point Lights

PointLightComponent::PointLightComponent() {

	//Sets our light type
	LightType = PointLight;

}

#pragma endregion
#pragma region Spot Lights

SpotLightComponent::SpotLightComponent() {

	//Sets our light type
	LightType = SpotLight;

}

#pragma endregion
#pragma region Directional Lights

DirectionalLightComponent::DirectionalLightComponent() {

	//Sets our light type
	LightType = DirectionalLight;

}