#pragma once

#include "Core.h"

#include "ChevalierGraphicsCore.h"

#include <GLFW/glfw3.h>

#include "ChevalierObjects.h"


struct QueueFamilyIndices {

	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}

	void findQueueFamilies(VkPhysicalDevice device);

};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};



//Struct Containing the vkInstance creator
struct VulkanInstance {

protected:

	static VkInstance ProjectInstance; //Static Reference for storing reference

	static void CreateInstance();

public:

	static VkInstance GetVulkanInstance(); //Returns current instance - Will create one if one does not exist

	static void cleanup();

};

struct VulkanExtentions {

	static std::vector<const char*> getRequiredExtentions();

};

struct ChevGLFWWindow {

protected:
	static GLFWwindow* pWindow;

	static void initWindow();

public:
	
	static GLFWwindow* getGLFWWindow();

	static void cleanup();

	static void getWindowFrameBufferExtent(int* pWidth, int* pHeight);

};

struct VulkanSurface {

protected:

	static VkSurfaceKHR vSurface;

	static void createSurface();

public:

	static VkSurfaceKHR getSurface();

	static void cleanup();

};

struct VulkanPhysicalDevice {

	// Maintains static reference to the vkPhysicalDevice for this application
protected:

	static VkPhysicalDevice physicalDevice;

	static void pickPhysicalDevice();

	// helpers to query device
	static bool isDeviceSuitable(VkPhysicalDevice device);

	static bool checkDeviceExtensionSupport(VkPhysicalDevice device);

	static VkSampleCountFlagBits getMaxUsableSampleCount();

public:

	static VkPhysicalDevice getPhysicalDevice();

	static void cleanup();

	//helper functions the use query the physical device

	static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

};

struct VulkanLogicalDevice {

protected:

	static VkDevice logicalDevice;

	static VkQueue graphicsQueue;
	static VkQueue presentQueue;

	static void createLogicalDevice();

public:

	static VkDevice getLogicalDevice();

	static VkQueue getGraphicsQueue() { return graphicsQueue; }
	static VkQueue getPresentQueue() { return presentQueue; }

	static void cleanup();

};

struct VulkanRenderPass {

protected:

	VkRenderPass vRenderPass;

public:

	void CreateRenderPass(VkFormat swapchainImageFormat);

	VkRenderPass getRenderPass() {
		return vRenderPass;
	}

};

struct SwapChainManager {

protected:

	friend struct ChevFramebuffer;

	//Handle to the SwapChain
	static VkSwapchainKHR swapchain;

	//List of the Images
	//Length is MAX_FRAMES_IN_FLIGHT
	static std::vector<VkImage> swapchainImages;

	//Format settings for the swap chain images
	static VkFormat swapchainImageFormat;

	//Extents of swapchain images
	static VkExtent2D swapchainExtent;

	//List of Image Views
	//Length is the Max Frames in Flight
	static std::vector<VkImageView> swapchainImageViews;

	//List of Frame Buffers
	//Length is the Max Frames in Flight
	static std::vector<VkFramebuffer> swapchainFramebuffers;

public:

	static VkSwapchainKHR getSwapChain() { return swapchain; }

	static void createSwapchain();

	static void cleanupSwapchain();

	static void createImageViews();

	static VkFormat getSwapchainImageFormat() { return swapchainImageFormat; }

	static VkFramebuffer getSwapChainFramebufferAt(uint32_t i) { return swapchainFramebuffers[i]; }

	static VkExtent2D getExtent() { return swapchainExtent; }

protected:

	static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

};

struct VulkanImageView {

public:

	static VkImageView CreateImageView(VkImage image, VkFormat imageFormat, VkImageAspectFlags aspectFlags, uint32_t mipLevel);

};

struct VulkanImageFormat {

	static VkFormat getSupportedFormat(const std::vector<VkFormat> candidates, VkImageTiling tiling, VkFormatFeatureFlagBits features);

};

struct VulkanCommandPool {

protected:

	static void CreateCommandPool();

	static VkCommandPool commandPool;

public:

	static VkCommandPool getCommandPool() {
		if (!commandPool) {
			CreateCommandPool();
		}
		return commandPool;
	}

};

struct VulkanCommandBuffers {

protected:

	std::vector<VkCommandBuffer> commandBuffers;

public:

	void CreateCommandBuffers();

	static VkCommandBuffer beginSingleTimeCommands();
	static void endSingleTimeCommands(VkCommandBuffer buffer);

	VkCommandBuffer getCommandBufferAt(uint32_t i) { return commandBuffers[i]; }
	VkCommandBuffer* getCommandBufferRefAt(uint32_t i) {return &commandBuffers[i]; }

};

struct ImageCreationInfo {

	uint32_t width;
	uint32_t height;
	uint32_t mipLevel;
	VkSampleCountFlagBits numSamples;
	VkFormat format;
	VkImageTiling tiling;
	VkImageUsageFlags usage;
	VkMemoryPropertyFlags properties;

};

struct VulkanImage {

	static void createImage(const ImageCreationInfo& info, VkImage& image, VkDeviceMemory& imageMemory);

	static VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevel);

	static void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageAspectFlags aspectMask, uint32_t mipLevel);

	static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

};

struct VulkanBuffer {

	static VkBuffer createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);


};