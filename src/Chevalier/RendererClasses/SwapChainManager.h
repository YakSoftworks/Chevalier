#pragma once
#include "ChevalierCore.h"

//Container for swap chain support
struct SwapChainManager
{

	//Handle to the SwapChain
	VkSwapchainKHR swapchain;

	//List of the Images
	//Length is MAX_FRAMES_IN_FLIGHT
	std::vector<VkImage> swapchainImages;

	//Format settings for the swap chain images
	VkFormat swapchainImageFormat;

	//Extents of swapchain images
	VkExtent2D swapchainExtent;

	//List of Image Views
	//Length is the Max Frames in Flight
	std::vector<VkImageView> swapchainImageViews;

	//List of Frame Buffers
	//Length is the Max Frames in Flight
	std::vector<VkFramebuffer> swapchainFramebuffers;

public:

	void createSwapchain();

	void cleanupSwapchain();

	void createImageViews();

protected:

	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);


};

