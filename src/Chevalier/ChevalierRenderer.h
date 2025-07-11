#pragma once

#include "ChevalierCore.h"

#include "RendererClasses/SwapChainManager.h"

class ChevalierRenderObject;



class ChevalierRenderer
{


public:

	ChevalierRenderer();

	void SetFrameBufferResized(bool value);

	static void RegisterRenderObject(ChevalierRenderObject* newObject);
	static void UnregisterRenderObject(ChevalierRenderObject* object);

	void initRenderer();

	//Friend Access for statics
	friend class ChevalierEngineStatics;



protected:
	//Singleton
	static ChevalierRenderer* mInstance;

	static ChevalierRenderer* getInstance() { return mInstance; }

protected:



	//Creations

	void createDescriptorSetLayout();

	void createGraphicsPipeline();

	void createDescriptorPool();
	void createDescriptorSets();
	void createCommandBuffers();
	void createSyncObjects();
	void createUniformBuffers();

	void createFramebuffers();
	void createCommandPool();

	void createRenderPass();

	//Members

	//Vulkan members

	//Swap Chain + properties
	SwapChainManager swapchainManager;




	//Sync Objects
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	uint32_t currentFrame = 0;

	//Boolean for resizing frame buffer
	bool framebufferResized = false;

	//Depth Image
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	//Color Image
	VkImage colorImage;
	VkDeviceMemory colorImageMemory;
	VkImageView colorImageView;

	//Render Objects
	std::vector<ChevalierRenderObject*> renderObjects;

	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	uint32_t mipLevels;
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;

	VkImageView textureImageView;
	VkSampler textureSampler;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> unformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

	//Not Permanent
	VkRenderPass renderPass;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	VkShaderModule createShaderModule(const std::vector<char>& code);

	//Chaos
	void updateUniformBuffer(uint32_t currentImage);


    void createTextureImage();

    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageAspectFlags aspectMask, uint32_t mipLevel);

    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    void createTextureImageView();

    void createTextureSampler();

    void createDepthResources();

    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    VkFormat findDepthFormat();

    bool hasStencilComponent(VkFormat format);

    void generateMipmaps(VkImage image, VkFormat imageFormat, uint32_t texWidth, uint32_t texHeight, uint32_t mipLevels);

    VkSampleCountFlagBits getMaxUsableSampleCount();

    void createColorResources();

	//Management Functions

	void recreateSwapChain();

	//Cleanup Functions
public:
	void cleanup();
protected:
	

	void cleanupDepthResources();
	void cleanupColorResources();


	//Loop Functions
public:
	void mainLoop();
protected:
	void drawFrame();

	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	






};

