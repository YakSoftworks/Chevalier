#include "ChevalierEngineStatics.h"

#include "ChevalierEngineInstance.h"
#include "ChevalierRenderer.h"

VkDevice ChevalierEngineStatics::getLogicalDevice()
{
    return ChevalierEngineInstance::getInstance()->vDevice;
}

VkPhysicalDevice ChevalierEngineStatics::getPhysicalDevice()
{
    return ChevalierEngineInstance::getInstance()->vPhysicalDevice;
}

VkSampleCountFlagBits ChevalierEngineStatics::getSamplesCount()
{
    return ChevalierEngineInstance::getInstance()->msaaSamples;
}

VkQueue ChevalierEngineStatics::getGraphicsQueue()
{
    return ChevalierEngineInstance::getInstance()->graphicsQueue;
}

VkQueue ChevalierEngineStatics::getPresentQueue()
{
    return ChevalierEngineInstance::getInstance()->presentQueue;
}

VkSurfaceKHR ChevalierEngineStatics::getSurface()
{
    return ChevalierEngineInstance::getInstance()->vSurface;
}

GLFWwindow* ChevalierEngineStatics::getWindow()
{
    return ChevalierEngineInstance::getInstance()->mWindow;
}

QueueFamilyIndices ChevalierEngineStatics::findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, ChevalierEngineStatics::getSurface(), &presentSupport);

        if (presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}

SwapChainSupportDetails ChevalierEngineStatics::querySwapChainSupport(VkPhysicalDevice device)
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, ChevalierEngineStatics::getSurface(), &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, ChevalierEngineStatics::getSurface(), &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, ChevalierEngineStatics::getSurface(), &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, ChevalierEngineStatics::getSurface(), &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, ChevalierEngineStatics::getSurface(), &presentModeCount, details.presentModes.data());
    }

    return details;
    
}

VkDescriptorPool ChevalierEngineStatics::getDescriptorPool()
{
    return ChevalierRenderer::getInstance()->descriptorPool;
}

std::vector<char> ChevalierEngineStatics::readFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

VkCommandBuffer ChevalierEngineStatics::beginSingleTimeCommands() {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = ChevalierRenderer::getInstance()->commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(ChevalierEngineStatics::getLogicalDevice(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void ChevalierEngineStatics::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(ChevalierEngineStatics::getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(ChevalierEngineStatics::getGraphicsQueue());

    vkFreeCommandBuffers(ChevalierEngineStatics::getLogicalDevice(), ChevalierRenderer::getInstance()->commandPool, 1, &commandBuffer);
}

void ChevalierEngineStatics::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
    //Creation Object
    VkBufferCreateInfo bufferInfo{};

    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;

    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(ChevalierEngineStatics::getLogicalDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create buffer");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(ChevalierEngineStatics::getLogicalDevice(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = ChevalierEngineStatics::findMemoryType(memRequirements.memoryTypeBits, properties);


    if (vkAllocateMemory(ChevalierEngineStatics::getLogicalDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate buffer memory");
    }

    vkBindBufferMemory(ChevalierEngineStatics::getLogicalDevice(), buffer, bufferMemory, 0);
}

void ChevalierEngineStatics::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    VkCommandBuffer commandBuffer = ChevalierEngineStatics::beginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    ChevalierEngineStatics::endSingleTimeCommands(commandBuffer);
}

uint32_t ChevalierEngineStatics::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(getPhysicalDevice(), &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}
