#include "ChevalierMaterial.h"
#include "Objects/ObjectTypes.h"

#include "RenderPassManager.h"


//Statics
GlobalDescriptorSet ChevalierMaterial::sGlobalDataManager{};


void ChevalierMaterial::createPipelineLayout()
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &sGlobalDataManager.mDescriptorSetLayout;

    VkPushConstantRange defaultDataPushRange{};
    defaultDataPushRange.size = 4;
    defaultDataPushRange.offset = 0;
    defaultDataPushRange.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS;

    pipelineLayoutInfo.pPushConstantRanges = &defaultDataPushRange;
    pipelineLayoutInfo.pushConstantRangeCount = 1;

    if (vkCreatePipelineLayout(VulkanLogicalDevice::getLogicalDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void ChevalierMaterial::createPipeline(RenderPassManager* renderPass, VkShaderModule vertShaderModule, VkShaderModule fragShaderModule, uint32_t subpass)
{

#pragma region Shader Modules

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

#pragma endregion

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;


    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    /*rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;*/

    // Pull from custom setup
    renderPass->GetPassRasterizationStateInfo(rasterizer);


    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_TRUE;
    multisampling.minSampleShading = .2f;
    multisampling.rasterizationSamples = MSAAResources::getMSAASampleCount();

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    VkPipelineDepthStencilStateCreateInfo depthCreateInfo{};
    depthCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthCreateInfo.depthTestEnable = VK_TRUE;
    depthCreateInfo.depthWriteEnable = VK_TRUE;
    depthCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
    depthCreateInfo.depthBoundsTestEnable = VK_FALSE;

    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.pDepthStencilState = &depthCreateInfo;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass->GetRenderPassRef();
    pipelineInfo.subpass = subpass;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;


    VkResult result = vkCreateGraphicsPipelines(VulkanLogicalDevice::getLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

}


void ChevalierMaterial::init_pipeline(RenderPassManager* renderPass, VkShaderModule vertShaderModule, VkShaderModule fragShaderModule, uint32_t subpass)
{

    createPipelineLayout();

    createPipeline(renderPass, vertShaderModule, fragShaderModule, subpass);

}

void ChevalierMaterial::BindMaterial(VkCommandBuffer* buffer, uint32_t currentFrame)
{
    vkCmdBindPipeline(*buffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        pipeline);

    vkCmdBindDescriptorSets(
        *buffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        pipelineLayout,
        0,
        1,
        &sGlobalDataManager.mDescriptorSets[currentFrame],
        0,
        0);
}

void ChevalierMaterial::UpdateGlobalDescriptor(GlobalDataObject* newGlobalData, uint32_t currentFrame)
{
    memcpy(sGlobalDataManager.globalDataBuffersMapped[currentFrame], newGlobalData, sizeof(GlobalDataObject));
}

VkShaderModule ChevalierMaterial::createShaderModule(const std::vector<char>& code)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(VulkanLogicalDevice::getLogicalDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}

#pragma region Global Descriptor Data

void GlobalDescriptorSet::CreateDescriptorSetLayout()
{

    VkDescriptorSetLayoutBinding globalLayoutBinding{};
    globalLayoutBinding.binding = 0;
    globalLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    globalLayoutBinding.descriptorCount = 1;
    globalLayoutBinding.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS; // Describes which shader stages we can access this. VK_SHADER_STAGE_ALL_GRAPHICS for all stages
    globalLayoutBinding.pImmutableSamplers = nullptr; //Relevant for image sampling

    VkDescriptorSetLayoutBinding objectDataLayoutBinding{};
    objectDataLayoutBinding.binding = 1;
    objectDataLayoutBinding.descriptorCount = 1;
    objectDataLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    objectDataLayoutBinding.pImmutableSamplers = nullptr;
    objectDataLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutBinding lightingDataLayoutBinding{};
    lightingDataLayoutBinding.binding = 2;
    lightingDataLayoutBinding.descriptorCount = 1;
    lightingDataLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    lightingDataLayoutBinding.pImmutableSamplers = nullptr;
    lightingDataLayoutBinding.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS;


    std::array<VkDescriptorSetLayoutBinding, 3> bindings = { globalLayoutBinding, objectDataLayoutBinding, lightingDataLayoutBinding };
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(VulkanLogicalDevice::getLogicalDevice(), &layoutInfo, nullptr, &mDescriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }

}

void GlobalDescriptorSet::CreateDescriptorPool()
{
    std::array<VkDescriptorPoolSize, 3> poolSizes{};
    
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(CHEVALIER_MAX_FRAMES_IN_FLIGHT);

    poolSizes[1].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(CHEVALIER_MAX_FRAMES_IN_FLIGHT);

    poolSizes[2].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    poolSizes[2].descriptorCount = static_cast<uint32_t>(CHEVALIER_MAX_FRAMES_IN_FLIGHT);


    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(CHEVALIER_MAX_FRAMES_IN_FLIGHT);

    if (vkCreateDescriptorPool(VulkanLogicalDevice::getLogicalDevice(), &poolInfo, nullptr, &mDescriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }

}

void GlobalDescriptorSet::CreateDescriptorSets()
{
    std::vector<VkDescriptorSetLayout> layouts(CHEVALIER_MAX_FRAMES_IN_FLIGHT, mDescriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = mDescriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(layouts.size()); // static_cast<uint32_t>(CHEVALIER_MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();
    

    mDescriptorSets.resize(CHEVALIER_MAX_FRAMES_IN_FLIGHT);


    if (vkAllocateDescriptorSets(VulkanLogicalDevice::getLogicalDevice(), &allocInfo, mDescriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }


    // Write Initial Values

    for (size_t i = 0; i < CHEVALIER_MAX_FRAMES_IN_FLIGHT; i++) {

        VkDescriptorBufferInfo globalBufferInfo{};
        globalBufferInfo.buffer = globalDataBuffers[i];
        globalBufferInfo.offset = 0;
        globalBufferInfo.range = sizeof(GlobalDataObject);


        VkDescriptorBufferInfo modelMatrixBufferInfo{};
        modelMatrixBufferInfo.buffer = modelMatrixBuffers[i];
        modelMatrixBufferInfo.offset = 0;
        modelMatrixBufferInfo.range = sizeof(ObjectShaderData) * CHEVALIER_CONSTANTS_INITIAL_MODEL_COUNT;


        VkDescriptorBufferInfo lightBufferInfo{};
        lightBufferInfo.buffer = lightingBuffers[i];
        lightBufferInfo.offset = 0;
        lightBufferInfo.range = sizeof(LightShaderInfo) * CHEVALIER_CONSTANTS_INITIAL_LIGHTING_COUNT;


        std::array<VkWriteDescriptorSet, 3> descriptorWrites{};


        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = mDescriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &globalBufferInfo;


        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = mDescriptorSets[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pBufferInfo = &modelMatrixBufferInfo;


        descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[2].dstSet = mDescriptorSets[i];
        descriptorWrites[2].dstBinding = 2;
        descriptorWrites[2].dstArrayElement = 0;
        descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        descriptorWrites[2].descriptorCount = 1;
        descriptorWrites[2].pBufferInfo = &lightBufferInfo;

        vkUpdateDescriptorSets(VulkanLogicalDevice::getLogicalDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

    }

}

void GlobalDescriptorSet::AllocateDescriptorMemory()
{


}

void GlobalDescriptorSet::createMemoryBuffers()
{

    // Global Data

    VkDeviceSize bufferSize = sizeof(GlobalDataObject);

    globalDataBuffers.resize(CHEVALIER_MAX_FRAMES_IN_FLIGHT);
    globalDataBuffersMemory.resize(CHEVALIER_MAX_FRAMES_IN_FLIGHT);
    globalDataBuffersMapped.resize(CHEVALIER_MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < CHEVALIER_MAX_FRAMES_IN_FLIGHT; i++) {
        VulkanBuffer::createBuffer(bufferSize,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            globalDataBuffers[i],
            globalDataBuffersMemory[i]);

        vkMapMemory(VulkanLogicalDevice::getLogicalDevice(), globalDataBuffersMemory[i], 0, bufferSize, 0, &globalDataBuffersMapped[i]);

    }


    // Model Matrices

    bufferSize = sizeof(ObjectShaderData) * CHEVALIER_CONSTANTS_INITIAL_MODEL_COUNT;

    modelMatrixBuffers.resize(CHEVALIER_MAX_FRAMES_IN_FLIGHT);
    modelMatrixBuffersMemory.resize(CHEVALIER_MAX_FRAMES_IN_FLIGHT);
    modelMatrixBuffersMapped.resize(CHEVALIER_MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < CHEVALIER_MAX_FRAMES_IN_FLIGHT; i++) {
        VulkanBuffer::createBuffer(bufferSize,
            VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            modelMatrixBuffers[i],
            modelMatrixBuffersMemory[i]);

        vkMapMemory(VulkanLogicalDevice::getLogicalDevice(), modelMatrixBuffersMemory[i], 0, bufferSize, 0, &modelMatrixBuffersMapped[i]);

    }



    // Lighting Data

    bufferSize = sizeof(LightShaderInfo) * CHEVALIER_CONSTANTS_INITIAL_LIGHTING_COUNT;

    lightingBuffers.resize(CHEVALIER_MAX_FRAMES_IN_FLIGHT);
    lightingBuffersMemory.resize(CHEVALIER_MAX_FRAMES_IN_FLIGHT);
    lightingBuffersMapped.resize(CHEVALIER_MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < CHEVALIER_MAX_FRAMES_IN_FLIGHT; i++) {
        VulkanBuffer::createBuffer(bufferSize,
            VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            lightingBuffers[i],
            lightingBuffersMemory[i]);

        vkMapMemory(VulkanLogicalDevice::getLogicalDevice(), lightingBuffersMemory[i], 0, bufferSize, 0, &lightingBuffersMapped[i]);

    }

}

void GlobalDescriptorSet::init()
{
    createMemoryBuffers();

    CreateDescriptorSetLayout();
    CreateDescriptorPool();
    CreateDescriptorSets();
    
}

#pragma endregion