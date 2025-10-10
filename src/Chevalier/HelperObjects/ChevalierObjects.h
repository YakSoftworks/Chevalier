#pragma once

#include "ChevalierGraphicsCore.h"

struct DepthResources {

protected:

	// Contains objects for depth buffering
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

public:

	static VkFormat findDepthFormat();

};

struct MSAAResources {
	
protected:

	static VkSampleCountFlagBits msaaSample;
	
public:

	friend struct VulkanPhysicalDevice;

	static VkSampleCountFlagBits getMSAASampleCount() { return msaaSample; }

};

struct SyncObjects {

protected:

	//Vectors containing an object for each frame in flight
	static std::vector<VkSemaphore> imageAvailableSemaphore;
	static std::vector<VkSemaphore> renderFinishedSemaphore;
	static std::vector<VkFence> inFlightFences;
	static uint32_t currentFrameIndex;


public:

	static void createSyncObjects();

};

struct ChevFramebuffer {

	static void InitFramebuffers(VkImageView colorImageView, VkImageView depthImageView, VkRenderPass renderPass);

};
