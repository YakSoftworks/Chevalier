#pragma once

#include "ChevalierGraphicsCore.h"

class ChevalierRenderer {

public:

	// Management Functions
	void InitRenderer();

	void LoopRenderer();

	void CleanupRenderer();

protected:

	//Owned Objects

	VulkanRenderPass mRenderPass;

	VulkanCommandBuffers mCommandBuffers; 
	
	//TODO: DEPTH RESOURCES

	//TODO: COLOR RESOURCES

};

