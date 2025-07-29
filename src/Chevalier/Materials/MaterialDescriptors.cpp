#include "MaterialDescriptors.h"
#include "ChevalierEngineStatics.h"

#pragma region GlobalDescriptorDataManager

GlobalDescriptorDataManager* GlobalDescriptorDataManager::instance = nullptr;


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

		GlobalShadingData* mData = static_cast<GlobalShadingData*>(globalDataMappedMemory[i]);
		

			mData->viewMat = glm::lookAt(glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

			int width, height = 0;

			ChevalierEngineStatics::getWindowFrameBufferExtent(&width, &height);

			mData->projMat = glm::perspective(
				glm::radians(45.f),
				(float) width / (float)height,
				0.1f,
				10.f);
	}

	createObjectBuffers();

	//Mark this as our instance for ease of access
	instance = this;

	std::cout << "Global Descriptor Setup Complete " << std::endl;
	
}

GlobalDescriptorDataManager::~GlobalDescriptorDataManager()
{
	//Cleanup our memory
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		VkDevice device = ChevalierEngineStatics::getLogicalDevice();
		//Delete Buffer
		vkDestroyBuffer(device, globalDataBuffers[i], nullptr);

		//Free Memory
		vkFreeMemory(device, globalDataBufferMemory[i], nullptr);

	}

	cleanupObjectBuffers();


}

void GlobalDescriptorDataManager::createObjectBuffers()
{

	objectBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	objectBufferMappedMemory.resize(MAX_FRAMES_IN_FLIGHT);

	VkDeviceSize bufferSize = sizeof(PerObjectData) * MAX_OBJECTS;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {

		ChevalierEngineStatics::createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			objectBuffers[i].Buffer,
			objectBuffers[i].BufferMemory
		);

		vkMapMemory(ChevalierEngineStatics::getLogicalDevice(),
			objectBuffers[i].BufferMemory,
			0,
			bufferSize,
			0,
			&objectBufferMappedMemory[i]);


	}
}

void GlobalDescriptorDataManager::cleanupObjectBuffers()
{
	VkDevice device = ChevalierEngineStatics::getLogicalDevice();
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {

		//Cleanup each buffer
		vkDestroyBuffer(device, objectBuffers[i].Buffer, nullptr);
		vkFreeMemory(device, objectBuffers[i].BufferMemory, nullptr);

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
		bindings[numBindings] = *binding1;
		//Ensure binding value
		bindings[numBindings].binding = 1;

		numBindings++;
	}

	//For each binding
	if (binding2) {
		bindings[numBindings] = *binding2;
		//Ensure binding value
		bindings[numBindings].binding = 2;

		numBindings++;
	}

	//For each binding
	if (binding3) {
		bindings[numBindings] = *binding3;

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

void MaterialDescriptorSetBase::createSets()
{

	std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, materialSetLayout);

	//Create our alloc info
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = ChevalierEngineStatics::getDescriptorPool();
	allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();

	//Prepare our list
	materialSets.resize(MAX_FRAMES_IN_FLIGHT);
	
	//Allocate the sets
	if (vkAllocateDescriptorSets(ChevalierEngineStatics::getLogicalDevice(), &allocInfo, materialSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	//Setup the sets - THIS INSTANCE ONLY

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		VkDescriptorBufferInfo globalInfo{};
		globalInfo.buffer = GlobalDescriptorDataManager::instance->globalDataBuffers[i];
		globalInfo.offset = 0;
		globalInfo.range = sizeof(GlobalShadingData);

		VkDescriptorBufferInfo objectInfo{};
		objectInfo.buffer = GlobalDescriptorDataManager::instance->objectBuffers[i].Buffer;
		objectInfo.offset = 0;
		objectInfo.range = sizeof(PerObjectData);


		std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = materialSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &globalInfo;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = materialSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pBufferInfo = &objectInfo;

		vkUpdateDescriptorSets(
			ChevalierEngineStatics::getLogicalDevice(),
			static_cast<uint32_t>(descriptorWrites.size()),
			descriptorWrites.data(),
			0,
			nullptr);
	}

}

void MaterialDescriptorSetBase::cleanupDescriptorSet()
{

	vkDestroyDescriptorSetLayout(
		ChevalierEngineStatics::getLogicalDevice(),
		materialSetLayout,
		nullptr);
}

MaterialDescriptorSetBase::~MaterialDescriptorSetBase()
{
	//cleanup set layout
	cleanupDescriptorSet();
}

#pragma endregion
#pragma region MaterialSet

void MaterialDescriptorSet::createLayout(VkDescriptorSetLayoutBinding* binding2, VkDescriptorSetLayoutBinding* binding3)
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


	VkDescriptorSetLayoutBinding objectDataBinding{};
	objectDataBinding.binding = 1;
	objectDataBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	objectDataBinding.pImmutableSamplers = nullptr;
	objectDataBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	objectDataBinding.descriptorCount = 1;


	//Pass to original function now using four bindings
	MaterialDescriptorSetBase::createLayout(
		&globalDataBinding,
		&objectDataBinding,
		binding2,
		binding3);

}

MaterialDescriptorSet::~MaterialDescriptorSet()
{
	//call for cleanup
	cleanupDescriptorSet();
}


#pragma endregion
