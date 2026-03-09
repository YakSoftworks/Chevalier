#pragma once

#include "ChevalierGraphicsCore.h"

#include "HelperObjects/ChevalierObjects.h"

#include "Objects/Map.h"
#include "ChevalierInterfaces.h"

#include "Objects/Lighting/LightingManager.h"
#include "Objects/GeometryManager.h"

class RenderPassManager;

class ChevalierRenderer {

public:

	ChevalierRenderer() {}

	// Management Functions
	void InitRenderer();

	void LoopRenderer();

	void CleanupRenderer();

protected:

	void drawFrame();

	void recordCommandBuffer(VkCommandBuffer buffer, uint32_t imageIndex);

protected:

	//Owned Objects

	//VulkanRenderPass mRenderPass;

	VulkanCommandBuffers mCommandBuffers; 


	uint32_t currentFrame = 0;
	
	//Boolean for resizing frame buffer
	bool framebufferResized = false;

	// Temporary Object Management
	std::vector<Actor*> actors; 

	// Managment for lights
	LightingManager mLightManager;

	// Geometry Manager
	GeometryManager mGeometryManager;

	// RenderPass management
	RenderPassManager* mRenderPassManager;


protected:

	// Helpers

	void recreateWindowResources();


public:

	bool CheckShouldClose();

};

