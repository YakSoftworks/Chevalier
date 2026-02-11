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
	lightSource_id = LightingManager::GetLightingManager()->RegisterLightObject(LightType, this);
}

void LightComponent::UnregisterLightSource()
{
	// Uhhhh, lets not worry about that yet!
}

void LightComponent::UpdateLightBufferInfo(LightShaderInfo* lightDataArray)
{
	LightShaderInfo lightData{};
	
	getDefaultLightInformation(lightData);

	submitLightInformation(lightData, lightDataArray);

}

void LightComponent::getDefaultLightInformation(LightShaderInfo& lightData)
{

	lightData.lightTransform = componentTransform.getTransform();

	lightData.color = { 1.f, 1.f, 1.f, 1.f };

	lightData.lightType = LightType;

}

void LightComponent::submitLightInformation(const LightShaderInfo& lightData, LightShaderInfo* shaderInfoArray)
{

	shaderInfoArray[lightSource_id] = lightData;

}

#pragma endregion
#pragma region Point Lights

PointLightComponent::PointLightComponent() {

	//Sets our light type
	LightType = PointLight;

}

void PointLightComponent::UpdateLightBufferInfo(LightShaderInfo* lightDataArray)
{
	LightShaderInfo lightData{};

	getDefaultLightInformation(lightData);

	// Do custom adjustments

	submitLightInformation(lightData, lightDataArray);

}

#pragma endregion
#pragma region Spot Lights

SpotLightComponent::SpotLightComponent() {

	//Sets our light type
	LightType = SpotLight;

}

void SpotLightComponent::UpdateLightBufferInfo(LightShaderInfo* lightDataArray)
{
	LightShaderInfo lightData{};

	getDefaultLightInformation(lightData);

	// Do custom adjustments

	submitLightInformation(lightData, lightDataArray);
}

#pragma endregion
#pragma region Directional Lights

DirectionalLightComponent::DirectionalLightComponent() {

	//Sets our light type
	LightType = DirectionalLight;

}

void DirectionalLightComponent::UpdateLightBufferInfo(LightShaderInfo* lightDataArray)
{
	LightShaderInfo lightData{};

	getDefaultLightInformation(lightData);

	// Do custom adjustments
	

	submitLightInformation(lightData, lightDataArray);
}
