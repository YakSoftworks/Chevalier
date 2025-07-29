#pragma once

#include "ChevalierCore.h"



// Container for descriptor layouts and sets for a material

//General Set Limited Additional Setup
struct MaterialDescriptorSetBase {

	VkDescriptorSetLayout materialSetLayout;

	//Sets, one per Frame_In_Flight
	std::vector<VkDescriptorSet> materialSets;
	
	//We will create our layout from four bindings
	virtual void createLayout(
		VkDescriptorSetLayoutBinding* binding1,
		VkDescriptorSetLayoutBinding* binding2,
		VkDescriptorSetLayoutBinding* binding3,
		VkDescriptorSetLayoutBinding* binding4
	);

	virtual void createSets();

	void cleanupDescriptorSet();

	~MaterialDescriptorSetBase();

};

//Set with Binding 0 being GlobalShadingData
struct MaterialDescriptorSet : public MaterialDescriptorSetBase {

	virtual void createLayout(
		VkDescriptorSetLayoutBinding* binding2,
		VkDescriptorSetLayoutBinding* binding3
	);

	~MaterialDescriptorSet();

};







struct GlobalShadingData {
	glm::mat4 viewMat;
	glm::mat4 projMat;
};

struct GlobalDescriptorDataManager {

	std::vector<VkBuffer> globalDataBuffers;
	std::vector<VkDeviceMemory> globalDataBufferMemory;
	std::vector<void*> globalDataMappedMemory;

	GlobalDescriptorDataManager();
	~GlobalDescriptorDataManager();

	static GlobalDescriptorDataManager* instance;

	void createObjectBuffers();
	void cleanupObjectBuffers();

	//Set of buffers - One per FRAME_IN_FLIGHT
	std::vector<AllocatedBuffer> objectBuffers;
	std::vector<void*> objectBufferMappedMemory;

};

