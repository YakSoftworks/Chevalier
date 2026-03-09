//
// Created by Jack on 3/2/26.
//

#include "ShadowLightingMaterial.h"

#include "RenderPassManager.h"

void ShadowLightDescriptor::CreateDescriptorSetLayout() {
    VkDescriptorSetLayoutBinding depthBinding{};
    depthBinding.binding = 0;
    depthBinding.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
    depthBinding.descriptorCount = 1;
    depthBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT; // Describes which shader stages we can access this. VK_SHADER_STAGE_ALL_GRAPHICS for all stages

    VkDescriptorSetLayoutBinding albedoBinding{};
    albedoBinding.binding = 1;
    albedoBinding.descriptorCount = 1;
    albedoBinding.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
    albedoBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutBinding normalBinding{};
    normalBinding.binding = 2;
    normalBinding.descriptorCount = 1;
    normalBinding.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
    normalBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutBinding shadowDepthBinding{};
    normalBinding.binding = 3;
    normalBinding.descriptorCount = 1;
    normalBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    normalBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;



    std::array<VkDescriptorSetLayoutBinding, 4> bindings = { depthBinding, albedoBinding, normalBinding, shadowDepthBinding };
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();


    if (vkCreateDescriptorSetLayout(VulkanLogicalDevice::getLogicalDevice(), &layoutInfo, nullptr, &mDescriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

void ShadowLightDescriptor::CreateDescriptorPool() {
    std::array<VkDescriptorPoolSize, 4> poolSizes{};

    poolSizes[0].type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
    poolSizes[0].descriptorCount = 4;

    poolSizes[1].type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
    poolSizes[1].descriptorCount = 4;

    poolSizes[2].type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
    poolSizes[2].descriptorCount = 4;

    poolSizes[3].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[3].descriptorCount = 4;


    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(CHEVALIER_MAX_FRAMES_IN_FLIGHT);

    if (vkCreateDescriptorPool(VulkanLogicalDevice::getLogicalDevice(), &poolInfo, nullptr, &mDescriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

void ShadowLightDescriptor::CreateDescriptorSets() {
    std::array<VkDescriptorSetLayout, CHEVALIER_MAX_FRAMES_IN_FLIGHT> layouts { mDescriptorSetLayout, mDescriptorSetLayout };
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = mDescriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(CHEVALIER_MAX_FRAMES_IN_FLIGHT); // static_cast<uint32_t>(CHEVALIER_MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();


    mDescriptorSets.resize(CHEVALIER_MAX_FRAMES_IN_FLIGHT);


    VkResult result = vkAllocateDescriptorSets(VulkanLogicalDevice::getLogicalDevice(), &allocInfo, mDescriptorSets.data());
    if(result != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    std::vector<VkImageView> renderPassResources;
    RenderPassManager::getRenderPassManager()->getRenderPassImageViews(renderPassResources);
    if (renderPassResources.size() != 4) {
        CHEV_MESSAGE_ERROR("Invalid Lighting Resources Retrieved!");
    }

    // Write Initial Values

    VkSamplerCreateInfo SamplerInfo{};
    SamplerInfo.magFilter = VK_FILTER_LINEAR;
    SamplerInfo.minFilter = VK_FILTER_LINEAR;
    SamplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    SamplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    SamplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    SamplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    SamplerInfo.mipLodBias = 0.0f;
    SamplerInfo.anisotropyEnable = VK_TRUE;
    SamplerInfo.compareEnable = VK_FALSE;
    SamplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

    vkCreateSampler(VulkanLogicalDevice::getLogicalDevice(), &SamplerInfo, nullptr, &depthImageSampler);



    std::array<VkDescriptorImageInfo, 4> images{};
    images[0].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    images[1].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    images[2].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    images[3].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    images[0].imageView = renderPassResources[0];
    images[1].imageView = renderPassResources[1];
    images[2].imageView = renderPassResources[2];
    images[2].imageView = renderPassResources[3];

    images[0].sampler = VK_NULL_HANDLE;
    images[1].sampler = VK_NULL_HANDLE;
    images[2].sampler = VK_NULL_HANDLE;
    images[3].sampler = depthImageSampler;

    // Set Values


    for (size_t i = 0; i < CHEVALIER_MAX_FRAMES_IN_FLIGHT; i++) {

        std::array<VkWriteDescriptorSet, 3> descriptorWrites{};


        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = mDescriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pImageInfo = &images[0];


        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = mDescriptorSets[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &images[1];


        descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[2].dstSet = mDescriptorSets[i];
        descriptorWrites[2].dstBinding = 2;
        descriptorWrites[2].dstArrayElement = 0;
        descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
        descriptorWrites[2].descriptorCount = 1;
        descriptorWrites[2].pImageInfo = &images[2];

        descriptorWrites[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[3].dstSet = mDescriptorSets[i];
        descriptorWrites[3].dstBinding = 2;
        descriptorWrites[3].dstArrayElement = 0;
        descriptorWrites[3].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[3].descriptorCount = 1;
        descriptorWrites[3].pImageInfo = &images[3];

        CHEV_MESSAGE_LOG("Update Lighting Descriptor Sets");

        vkUpdateDescriptorSets(VulkanLogicalDevice::getLogicalDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

    }
}

void ShadowLightingMaterial::createPipelineLayout() {
    materialInputAttachmentDescriptorSet.InitDescriptor();

    VkDescriptorSetLayout setLayouts[] = { materialInputAttachmentDescriptorSet.mDescriptorSetLayout, sGlobalDataManager.mDescriptorSetLayout };

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 2;
    pipelineLayoutInfo.pSetLayouts = setLayouts;

    pipelineLayoutInfo.pPushConstantRanges = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;

    if (vkCreatePipelineLayout(VulkanLogicalDevice::getLogicalDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void ShadowLightingMaterial::BindMaterial(VkCommandBuffer *buffer, uint32_t currentFrame) {

    vkCmdBindPipeline(*buffer,
                      VK_PIPELINE_BIND_POINT_GRAPHICS,
                      pipeline);


    VkDescriptorSet lightingSets[] = { materialInputAttachmentDescriptorSet.mDescriptorSets[currentFrame], sGlobalDataManager.mDescriptorSets[currentFrame] };

    vkCmdBindDescriptorSets(
            *buffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            pipelineLayout,
            0,
            2,
            lightingSets,
            0,
            0);
}
