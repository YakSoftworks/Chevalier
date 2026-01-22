#pragma once

#include "Objects/Components/Component.h"


// Lighting Components for the different lighting types

// Point Light - Light in all directions
// Spot Light - Light within angle of forward direction
// Directional Light - Light from an infinite distance away

enum LightSourceType : uint8_t
{
	PointLight,
	SpotLight,
	DirectionalLight
};

struct LightInfoBase {

	glm::vec4 position;
	glm::vec4 color;
	glm::vec4 direction;
	glm::vec2 info;

};

struct LightBufferObject {

	uint32_t numPointLights;
	uint32_t maxPointLights = CHEVALIER_CONSTANTS_MAX_POINT_LIGHTS;

	uint32_t numSpotLights;
	uint32_t maxSpotLights = CHEVALIER_CONSTANTS_MAX_SPOT_LIGHTS;

	uint32_t numDirectionalLights;
	uint32_t maxDirectionalLights = CHEVALIER_CONSTANTS_MAX_DIRECTIONAL_LIGHTS;


};

struct LightSourceBufferObject {



};

// Base Class for Lighting Components
class LightComponent : public SceneComponent {

protected:

	// Component Implementation

	virtual void PreInitializeComponent() override;
	virtual void InitializeComponent() override;
	virtual void PostInitializeComponent() override;


	// Light Component Functions


	void RegisterLightSource();
	void UnregisterLightSource();


	// Know what type of light you are!
	LightSourceType LightType;

public:

	LightInfoBase mLightInfo;

};

//struct PointLightInfo : public LightInfoBase {
//
//	// How far does this light cast
//	float AttenuationRadius;
//
//};

class PointLightComponent : public LightComponent {

public:
	//Default Constructor
	PointLightComponent();

};

class SpotLightComponent : public LightComponent {

public:
	//Default Constructor
	SpotLightComponent();

};

class DirectionalLightComponent : public LightComponent {

public:
	//Default Constructor
	DirectionalLightComponent();

};







