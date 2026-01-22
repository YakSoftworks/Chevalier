#pragma once

#include "ChevalierGraphicsCore.h"

#include "HelperObjects/ChevalierObjects.h"

#include "Objects/Map.h"
#include "ChevalierInterfaces.h"
#include "Objects/Lighting/LightingManager.h"

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
	
	// DEPTH RESOURCES
	DepthResources mDepthResources;

	//TODO: COLOR RESOURCES
	ColorResources mColorResources;

	// Current Drawobjects
	std::vector<ChevalierRenderObjectInterface*> RenderObjects;

	// Current Materials
	std::vector<ChevalierMaterialInterface*> Materials;

	uint32_t currentFrame = 0;
	
	//Boolean for resizing frame buffer
	bool framebufferResized = false;





	// Temporary Object Management
	std::vector<Actor*> actors; 

	// Managment for lights
	LightingManager mLightManager;

	// RenderPass management
	RenderPassManager* mRenderPassManager;


protected:

	// Helpers

	void recreateWindowResources();

protected:

	// Perform the geometry pass
	void performGeometryPass(VkCommandBuffer buffer);

public:

	bool CheckShouldClose();

};

