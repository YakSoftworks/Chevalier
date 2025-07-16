#include "MaterialDescriptors.h"


#pragma region GlobalDescriptorDataManager

GlobalDescriptorDataManager::GlobalDescriptorDataManager()
{
	
	//If an instance already exists, throw an error
	if (instance != nullptr) {
		throw std::runtime_error("Creating a Global Descriptor Data when one already exists");
	}
	
	
	//Create One set per Frame in Flight
	//This data should only change once per frame

	VkDeviceSize bufferSize = sizeof(GlobalShadingData);
	
	globalDataBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	globalDataBufferMemory.resize(MAX_FRAMES_IN_FLIGHT);
	globalDataMappedMemory.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {

		//Create the Buffer on the device
		ChevalierEngineStatics::createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			globalDataBuffers[i],
			globalDataBufferMemory[i]
		);

		//Permanently map the memory so we caxn edit it
		vkMapMemory(
			ChevalierEngineStatics::getLogicalDevice(),
			globalDataBufferMemory[i],
			0,
			bufferSize,
			0,
			&globalDataMappedMemory[i]
		);



	}

	//Mark this as our instance for ease of access
	instance = this;

	
}

GlobalDescriptorDataManager::~GlobalDescriptorDataManager()
{
	//Cleanup our memory
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		VkDevice device = ChevalierEngineStatics::getLogicalDevice();
		//Delete Buffer
		vkDestroyBuffer(device, globalDataBuffers[i], nullptr);

		//Free Memory
		vkFreeMemory(device, globalDataBufferMemory[i], false);

	}


}

#pragma endregion
#pragma region MaterialSetBase
void MaterialDescriptorSetBase::createLayout(VkDescriptorSetLayoutBinding* binding0, VkDescriptorSetLayoutBinding* binding1, VkDescriptorSetLayoutBinding* binding2, VkDescriptorSetLayoutBinding* binding3)
{
	//create an array for each possible binding
	std::array<VkDescriptorSetLayoutBinding, 4> bindings;
	uint8_t numBindings = 0;


	//NOTE: Even if an earlier binding was invalid,
	//we will still enforce that the binding is
	// the binding specified by the function input


	//For each binding
	if (binding0) {
		bindings[numBindings] = *binding0;

		//Ensure binding value
		bindings[numBindings].binding = 0;

		numBindings++;
	}

	
	if (binding1) {
		bindings[numBindings] = *binding0;
		//Ensure binding value
		bindings[numBindings].binding = 1;

		numBindings++;
	}

	//For each binding
	if (binding2) {
		bindings[numBindings] = *binding0;
		//Ensure binding value
		bindings[numBindings].binding = 2;

		numBindings++;
	}

	//For each binding
	if (binding3) {
		bindings[numBindings] = *binding0;

		//Ensure binding value
		bindings[numBindings].binding = 3;

		numBindings++;
	}

	VkDescriptorSetLayoutCreateInfo layoutInfo{ };
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = numBindings;
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(ChevalierEngineStatics::getLogicalDevice(), &layoutInfo, nullptr, &materialSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}


}

#pragma endregion
#pragma region MaterialSet

void MaterialDescriptorSet::createLayout(VkDescriptorSetLayoutBinding* binding1, VkDescriptorSetLayoutBinding* binding2, VkDescriptorSetLayoutBinding* binding3)
{
	//Make Binding 0 - Global Shader Data
	VkDescriptorSetLayoutBinding globalDataBinding{};

	//Global data should always have binding 0
	globalDataBinding.binding = 0;
	//global data is uniform
	globalDataBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	//global data count is always 1 because it is per-pass
	globalDataBinding.descriptorCount = 1;
	// global data should be available in all stages
	globalDataBinding.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS;

	globalDataBinding.pImmutableSamplers = nullptr;

	//Pass to original function now using four bindings
	MaterialDescriptorSetBase::createLayout(
		&globalDataBinding,
		binding1,
		binding2,
		binding3);

}


#pragma endregion
