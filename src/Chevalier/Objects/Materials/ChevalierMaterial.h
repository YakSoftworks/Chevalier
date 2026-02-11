#pragma once
#include "Core.h"

#include "ChevalierInterfaces.h"

#include "Objects/ObjectTypes.h"

class RenderPassManager;

struct MaterialCreateInfo {

	//Filepaths to shaders
	std::string vertexShaderPath;
	std::string fragmentShaderPath;



};


struct MaterialDescriptorBase {
	//Base struct for Descriptor Data

	// Layout
	VkDescriptorSetLayout mDescriptorSetLayout;

	// Keep a Descriptor Pool
	VkDescriptorPool mDescriptorPool;

	// Keep Descriptor Sets
	std::vector<VkDescriptorSet> mDescriptorSets;

	// Create Descriptor Set Layout
	virtual void CreateDescriptorSetLayout() { /* Do Nothing */ };

	// Create Descriptor Pool
	virtual void CreateDescriptorPool() { /* Do Nothing */ };

	// Create Descriptor Sets
	virtual void CreateDescriptorSets() { /* Do nothing */ };

	// Allocate Descriptor Set Data
	virtual void AllocateDescriptorMemory() { /* Do nothing */ };

	

};


struct GlobalDescriptorSet : MaterialDescriptorBase {

	// MaterialDescriptorBase Implementation

	void CreateDescriptorSetLayout() override;
	void CreateDescriptorPool() override;
	void CreateDescriptorSets() override;
	void AllocateDescriptorMemory() override;

	//Memory for the buffers

	void createMemoryBuffers();

	void init();


	// GlobalDataObject
	std::vector<VkBuffer> globalDataBuffers;
	std::vector<VkDeviceMemory> globalDataBuffersMemory;
	std::vector<void*> globalDataBuffersMapped;

	// ModelMat Buffer
	std::vector<VkBuffer> modelMatrixBuffers;
	std::vector<VkDeviceMemory> modelMatrixBuffersMemory;
	std::vector<void*> modelMatrixBuffersMapped;

	// Lighting Buffer
	std::vector<VkBuffer> lightingBuffers;
	std::vector<VkDeviceMemory> lightingBuffersMemory;
	std::vector<void*> lightingBuffersMapped;

};


class ChevalierMaterialInstance : public ChevalierMaterialInterface {

	// Chevalier Material, but with unique data for our sets

};


class ChevalierMaterial : public ChevalierMaterialInterface
{

public:
	 
	// Constructor / Destructor
	ChevalierMaterial() {};
	~ChevalierMaterial() {};

	// Init with data from the renderer
	void init_pipeline(RenderPassManager* renderPass, VkShaderModule vertShaderModule, VkShaderModule fragShaderModule, uint32_t subpass);


	//ChevalierMaterialInterface Implementation - Means we are bindable
	virtual void BindMaterial(VkCommandBuffer* buffer, uint32_t currentFrame) override;
	virtual VkPipelineLayout getMaterialLayout() { return pipelineLayout; }


	//Pipeline information
	VkPipelineLayout pipelineLayout;
	VkPipeline pipeline;

	//Global Descriptor Information
	static GlobalDescriptorSet sGlobalDataManager;
	static void UpdateGlobalDescriptor(GlobalDataObject* newGlobalData, uint32_t currentFrame);

public:

	static VkShaderModule createShaderModule(const std::vector<char>& code);

protected:

	void createPipelineLayout();
	virtual void createPipeline(RenderPassManager* renderPass, VkShaderModule vertShaderModule, VkShaderModule fragShaderModule, uint32_t subpass);

	// Called in the destructor - Cleans up pipeline resources
	void cleanup();

};

