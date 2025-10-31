#pragma once

#include "ChevalierGraphicsCore.h"

struct ObjectShaderData;

class ChevalierMaterialInterface {

public:

	// Add Material Properties to our given Command Buffer
	virtual void BindMaterial(VkCommandBuffer* buffer, uint32_t currentFrame) { /* No nothing */ };

	virtual VkPipelineLayout getMaterialLayout() { return nullptr; };

};

class ChevalierRenderObjectInterface {

	//Abstract Class for functions about drawing objects

public:

	// Function to bind object data to the given command buffer
	virtual void DrawObject(VkCommandBuffer buffer, ObjectShaderData* objectDataArray) { /* Do nothing */ };

	// Returns a bool of whether or not an object is drawable
	virtual bool IsObjectDrawable() { return false; }

	// Gets the resource to get information of the object's material
	virtual ChevalierMaterialInterface* GetObjectMaterial() { return nullptr; }

	// ID used for indexing objects
	uint32_t renderObjectID = 0;

};

class ChevalierCameraInterface {

	// Abstract class for camera controls

public:
	virtual glm::mat4 GetCameraViewMatrix() {};

};
