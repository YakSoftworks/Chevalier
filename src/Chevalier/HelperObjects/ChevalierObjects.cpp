#include "ChevalierObjects.h"

//Static Variables
VkSampleCountFlagBits MSAAResources::msaaSample = VK_SAMPLE_COUNT_1_BIT;

std::vector<VkSemaphore> SyncObjects::imageAvailableSemaphore = {};
std::vector<VkSemaphore> SyncObjects::renderFinishedSemaphore = {};
std::vector<VkFence> SyncObjects::inFlightFences = {};
uint32_t SyncObjects::currentFrameIndex = 0;

#pragma region DepthResources

void DepthResources::CreateDepthResources(uint32_t width, uint32_t height)
{
    //Get our format settings
    VkFormat depthFormat = findDepthFormat();

    //Depth Image

    ImageCreationInfo info{};
    info.width = width;
    info.height = height;
    info.mipLevel = 1;
    info.format = depthFormat;
    info.tiling = VK_IMAGE_TILING_OPTIMAL;
    info.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    

    VulkanImage::createImage(info, depthImage, depthImageMemory);

    depthImageView = VulkanImage::createImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);

    VulkanImage::transitionImageLayout(depthImage, depthFormat,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_ASPECT_DEPTH_BIT,
        1 );

}

void DepthResources::cleanup() {
    VkDevice device = VulkanLogicalDevice::getLogicalDevice();
    vkDestroyImageView(device, depthImageView, nullptr);
    vkDestroyImage(device, depthImage, nullptr);
    vkFreeMemory(device, depthImageMemory, nullptr);
}

VkFormat DepthResources::findDepthFormat()
{
    return VulkanImageFormat::getSupportedFormat(
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

#pragma endregion

#pragma region ColorResources

void ColorResources::CreateColorResources(uint32_t width, uint32_t height){
    VkFormat colorFormat = SwapChainManager::getSwapchainImageFormat();

    ImageCreationInfo colorImageInfo{};
    colorImageInfo.width = width;
    colorImageInfo.height = height;
    colorImageInfo.format = colorFormat;
    colorImageInfo.mipLevel = 1;
    colorImageInfo.numSamples = MSAAResources::getMSAASampleCount();
    colorImageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    colorImageInfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    colorImageInfo.usage = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    
    VulkanImage::createImage(colorImageInfo, colorImage, colorImageMemory);

    VulkanImageView::CreateImageView(colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);

}

void ColorResources::cleanup(){
    VkDevice device = VulkanLogicalDevice::getLogicalDevice();
    vkDestroyImage(device, colorImage, nullptr);
    vkDestroyImageView(device, colorImageView, nullptr);
    vkFreeMemory(device, colorImageMemory, nullptr);
}

#pragma endregion

#pragma region SyncObjects

void SyncObjects::createSyncObjects()
{
    imageAvailableSemaphore.resize(CHEVALIER_MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphore.resize(CHEVALIER_MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(CHEVALIER_MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < CHEVALIER_MAX_FRAMES_IN_FLIGHT; i++) {
        if (
            vkCreateSemaphore(VulkanLogicalDevice::getLogicalDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphore[i]) != VK_SUCCESS ||
            vkCreateSemaphore(VulkanLogicalDevice::getLogicalDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphore[i]) != VK_SUCCESS ||
            vkCreateFence(VulkanLogicalDevice::getLogicalDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS
            ) {
            CHEV_MESSAGE_ERROR("Failed to create Sync Objects")
        }
    }


}

#pragma endregion

#pragma region ChevFrameBuffers

void ChevFramebuffer::InitFramebuffers(VkImageView colorImageView, VkImageView depthImageView, VkRenderPass renderPass)
{
    SwapChainManager::swapchainFramebuffers.resize(SwapChainManager::swapchainImageViews.size());

    for (size_t i = 0; i < SwapChainManager::swapchainImageViews.size(); i++) {

        VkImageView attachments[] = {
            colorImageView,
            depthImageView,
            SwapChainManager::swapchainImageViews[i]
        };

        VkFramebufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        bufferInfo.renderPass = renderPass;
        bufferInfo.attachmentCount = 3;
        bufferInfo.pAttachments = attachments;
        bufferInfo.width = SwapChainManager::swapchainExtent.width;
        bufferInfo.height = SwapChainManager::swapchainExtent.height;
        bufferInfo.layers = 1;

        if (vkCreateFramebuffer(VulkanLogicalDevice::getLogicalDevice(), &bufferInfo, nullptr, &SwapChainManager::swapchainFramebuffers[i]) != VK_SUCCESS) {
            CHEV_MESSAGE_ERROR("Failed to create Framebuffer");
        }

    }
}

#pragma endregion