#pragma once

#include "ChevalierGraphicsCore.h"

#include "HelperObjects/ChevalierObjects.h"

class ChevalierRenderer {

public:

	// Management Functions
	void InitRenderer();

	void LoopRenderer();

	void CleanupRenderer();

protected:

	void drawFrame();

	void recordCommandBuffer(VkCommandBuffer buffer, uint32_t imageIndex);

protected:

	//Owned Objects

	VulkanRenderPass mRenderPass;

	VulkanCommandBuffers mCommandBuffers; 
	
	//DEPTH RESOURCES
	DepthResources mDepthResources;

	//TODO: COLOR RESOURCES

};

