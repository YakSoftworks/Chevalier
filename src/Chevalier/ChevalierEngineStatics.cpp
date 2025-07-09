#include "WyrmEngineStatics.h"

#include "WyrmEngineInstance.h"
#include "WyrmRenderer.h"

VkDevice WyrmEngineStatics::getLogicalDevice()
{
    return WyrmEngineInstance::getInstance()->vDevice;
}

VkPhysicalDevice WyrmEngineStatics::getPhysicalDevice()
{
    return WyrmEngineInstance::getInstance()->vPhysicalDevice;
}

VkSampleCountFlagBits WyrmEngineStatics::getSamplesCount()
{
    return WyrmEngineInstance::getInstance()->msaaSamples;
}

VkQueue WyrmEngineStatics::getGraphicsQueue()
{
    return WyrmEngineInstance::getInstance()->graphicsQueue;
}

VkQueue WyrmEngineStatics::getPresentQueue()
{
    return WyrmEngineInstance::getInstance()->presentQueue;
}

VkSurfaceKHR WyrmEngineStatics::getSurface()
{
    return WyrmEngineInstance::getInstance()->vSurface;
}

GLFWwindow* WyrmEngineStatics::getWindow()
{
    return WyrmEngineInstance::getInstance()->mWindow;
}

QueueFamilyIndices WyrmEngineStatics::findQueueFamilies(VkPhysicalDevice device) {
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
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, WyrmEngineStatics::getSurface(), &presentSupport);

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

SwapChainSupportDetails WyrmEngineStatics::querySwapChainSupport(VkPhysicalDevice device)
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, WyrmEngineStatics::getSurface(), &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, WyrmEngineStatics::getSurface(), &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, WyrmEngineStatics::getSurface(), &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, WyrmEngineStatics::getSurface(), &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, WyrmEngineStatics::getSurface(), &presentModeCount, details.presentModes.data());
    }

    return details;
    
}

VkDescriptorPool WyrmEngineStatics::getDescriptorPool()
{
    return WyrmRenderer::getInstance()->descriptorPool;
}

std::vector<char> WyrmEngineStatics::readFile(const std::string& filename)
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

VkCommandBuffer WyrmEngineStatics::beginSingleTimeCommands() {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = WyrmRenderer::getInstance()->commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(WyrmEngineStatics::getLogicalDevice(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void WyrmEngineStatics::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(WyrmEngineStatics::getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(WyrmEngineStatics::getGraphicsQueue());

    vkFreeCommandBuffers(WyrmEngineStatics::getLogicalDevice(), WyrmRenderer::getInstance()->commandPool, 1, &commandBuffer);
}

void WyrmEngineStatics::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
    //Creation Object
    VkBufferCreateInfo bufferInfo{};

    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;

    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(WyrmEngineStatics::getLogicalDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create buffer");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(WyrmEngineStatics::getLogicalDevice(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = WyrmEngineStatics::findMemoryType(memRequirements.memoryTypeBits, properties);


    if (vkAllocateMemory(WyrmEngineStatics::getLogicalDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate buffer memory");
    }

    vkBindBufferMemory(WyrmEngineStatics::getLogicalDevice(), buffer, bufferMemory, 0);
}

void WyrmEngineStatics::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    VkCommandBuffer commandBuffer = WyrmEngineStatics::beginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    WyrmEngineStatics::endSingleTimeCommands(commandBuffer);
}

uint32_t WyrmEngineStatics::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
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
