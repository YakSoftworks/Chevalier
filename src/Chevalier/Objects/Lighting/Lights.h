#pragma once

#include "Objects/Components/Component.h"


// Lighting Components for the different lighting types

// Point Light - Light in all directions
// Spot Light - Light within angle of forward direction
// Directional Light - Light from an infinite distance away

enum LightSourceType : uint32_t
{
	PointLight,
	SpotLight,
	DirectionalLight
};

struct LightShaderInfo {

	
	glm::mat4 lightTransform;
	glm::vec4 color;
	glm::vec4 info;
	int lightType;

};

// Base Class for Lighting Components
class LightComponent : public SceneComponent {

public:

	// Component Implementation

	virtual void PreInitializeComponent() override;
	virtual void InitializeComponent() override;
	virtual void PostInitializeComponent() override;


	// Light Component Functions
protected:

	void RegisterLightSource();
	void UnregisterLightSource();


	// Know what type of light you are!
	LightSourceType LightType;

public:

	LightShaderInfo mLightInfo;

	uint32_t lightSource_id;

	virtual void UpdateLightBufferInfo(LightShaderInfo* lightDataArray);

protected:

	void getDefaultLightInformation(LightShaderInfo& lightData);

	void submitLightInformation(const LightShaderInfo& lightData, LightShaderInfo* shaderInfoArray);

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

	virtual void UpdateLightBufferInfo(LightShaderInfo* lightDataArray) override;

};

class SpotLightComponent : public LightComponent {

public:
	//Default Constructor
	SpotLightComponent();

	virtual void UpdateLightBufferInfo(LightShaderInfo* lightDataArray) override;

};

class DirectionalLightComponent : public LightComponent {

public:
	//Default Constructor
	DirectionalLightComponent();

	virtual void UpdateLightBufferInfo(LightShaderInfo* lightDataArray) override;

};







