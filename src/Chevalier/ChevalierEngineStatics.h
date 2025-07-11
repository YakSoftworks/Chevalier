#pragma once

#include "ChevalierCore.h"

struct GLFWwindow;

class ChevalierEngineStatics
{
public:

	static VkDevice getLogicalDevice();
	static VkPhysicalDevice getPhysicalDevice();
	static VkSampleCountFlagBits getSamplesCount();
	static VkQueue getGraphicsQueue();
	static VkQueue getPresentQueue();
	static VkSurfaceKHR getSurface();
	static GLFWwindow* getWindow();

	static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	

	static void getWindowFrameBufferExtent(int* pWidth, int* pHeight);



	static std::vector<char> readFile(const std::string& filename);

    static VkCommandBuffer beginSingleTimeCommands();

    static void endSingleTimeCommands(VkCommandBuffer commandBuffer);

	static void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	static void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
public:
	static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

	static VkDescriptorPool getDescriptorPool();

	static VkImageView createImageView(VkImage image, VkFormat imageFormat, VkImageAspectFlags aspectFlags, uint32_t mipLevel);

	static void createImage(uint32_t width, 
		uint32_t height, 
		uint32_t mipLevel, 
		VkSampleCountFlagBits numSamples, 
		VkFormat format, 
		VkImageTiling tiling,
		VkImageUsageFlags usage, 
		VkMemoryPropertyFlags properties, 
		VkImage& image, 
		VkDeviceMemory& imageMemory);

};


