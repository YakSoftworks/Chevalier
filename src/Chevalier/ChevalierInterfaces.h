#pragma once

#include "ChevalierGraphicsCore.h"

class ChevalierMaterialInterface {

	// Add Material Properties to our given Command Buffer
	virtual void BindMaterial(VkCommandBuffer* buffer) { /* No nothing */ };

};

class ChevalierRenderObjectInterface {

	//Abstract Class for functions about drawing objects

public:

	// Function to bind object data to the given command buffer
	virtual void DrawObject(VkCommandBuffer buffer) { /* Do nothing */ };

	// Returns a bool of whether or not an object is drawable
	virtual bool IsObjectDrawable() { return false; }

	// Gets the resource to get information of the object's material
	virtual ChevalierMaterialInterface* GetObjectMaterial() { return nullptr; }

};

class ChevalierCameraInterface {

	// Abstract class for camera controls

public:
	virtual glm::mat4 GetCameraViewMatrix() {};

};
