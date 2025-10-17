#include "VulkanObjects.h"



// Static Objects
VkInstance VulkanInstance::ProjectInstance = nullptr;
GLFWwindow* ChevGLFWWindow::pWindow = nullptr;
VkSurfaceKHR VulkanSurface::vSurface = nullptr;

VkPhysicalDevice VulkanPhysicalDevice::physicalDevice = nullptr;
VkDevice VulkanLogicalDevice::logicalDevice = nullptr;
VkQueue VulkanLogicalDevice::graphicsQueue = nullptr;
VkQueue VulkanLogicalDevice::presentQueue = nullptr;

VkSwapchainKHR SwapChainManager::swapchain = nullptr;
std::vector<VkImage> SwapChainManager::swapchainImages;
VkFormat SwapChainManager::swapchainImageFormat;
VkExtent2D SwapChainManager::swapchainExtent;
std::vector<VkImageView> SwapChainManager::swapchainImageViews;
std::vector<VkFramebuffer> SwapChainManager::swapchainFramebuffers;

VkCommandPool VulkanCommandPool::commandPool = nullptr;

#pragma region VulkanInstance

void VulkanInstance::CreateInstance()
{
    //Application Info
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Chevalier";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "ChevalierEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    //Creation Info
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    //Get Extensions
    auto extensions = VulkanExtentions::getRequiredExtentions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();


    //TODO: DEBUG MESSENGER


    if (vkCreateInstance(&createInfo, nullptr, &ProjectInstance) != VK_SUCCESS)
    {
        CHEV_MESSAGE_ERROR("Failed to Create Instance!");
    }

}

VkInstance VulkanInstance::GetVulkanInstance()
{
    //If reference is invalid, create valid reference
    if (ProjectInstance == nullptr) {
        CreateInstance();
    }

    return ProjectInstance;
}

void VulkanInstance::cleanup()
{
    vkDestroyInstance(ProjectInstance, nullptr);
}

#pragma endregion

#pragma region VulkanExtensions

std::vector<const char*> VulkanExtentions::getRequiredExtentions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (CHEVALIER_ENABLE_VALIDATION_LAYERS) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

#pragma endregion

#pragma region ChevGLFWWindow

void ChevGLFWWindow::initWindow()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    pWindow = glfwCreateWindow(
        CHEVALIER_WINDOW_WIDTH_DEFAULT,
        CHEVALIER_WINDOW_HEIGHT_DEFAULT,
        "Chevalier",
        nullptr,
        nullptr);

    if (pWindow == nullptr) {
        CHEV_MESSAGE_ERROR("INVALID WINDOW SETUP");
    }

}

GLFWwindow* ChevGLFWWindow::getGLFWWindow()
{
    if (pWindow == nullptr) {
        initWindow();
    }
    return pWindow;
}

void ChevGLFWWindow::cleanup() {
    glfwDestroyWindow(pWindow);

    glfwTerminate();
}

void ChevGLFWWindow::getWindowFrameBufferExtent(int* pWidth, int* pHeight)
{
    glfwGetFramebufferSize(pWindow, pWidth, pHeight);
}

#pragma endregion

#pragma region VulkanPhysicalDevice

void VulkanPhysicalDevice::pickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(
        VulkanInstance::GetVulkanInstance(),
        &deviceCount,
        nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    vkEnumeratePhysicalDevices(
        VulkanInstance::GetVulkanInstance(),
        &deviceCount,
        physicalDevices.data());

    // If we have no physical devices, then throw an error
    if (deviceCount == 0) {

        CHEV_MESSAGE_ERROR("Failed to find physical devices!");
    
    }

    // Pick the first suitable device
    for (const VkPhysicalDevice& sample : physicalDevices) {
        
        if (isDeviceSuitable(sample)) {
            
            physicalDevice = sample; // Set our chosen device

            MSAAResources::msaaSample = getMaxUsableSampleCount();    // Get the MSAA data from the device



            break;
        }
    }


    
    // If no device was suitable throw an error
    if (!physicalDevice) {

        CHEV_MESSAGE_ERROR("No suitable physical device found");
    
    }

}

bool VulkanPhysicalDevice::isDeviceSuitable(VkPhysicalDevice device)
{
    //Returns true if device is suitable to the project setup

    QueueFamilyIndices indices = QueueFamilyIndices();
    indices.findQueueFamilies(device);

    bool extentsionsSupported = checkDeviceExtensionSupport(device);

    SwapChainSupportDetails swapchainSupport = querySwapChainSupport(device);
    bool swapchainSupported = !swapchainSupport.formats.empty() && !swapchainSupport.presentModes.empty();


    return indices.isComplete() && extentsionsSupported && swapchainSupported;
}

SwapChainSupportDetails VulkanPhysicalDevice::querySwapChainSupport(VkPhysicalDevice device)
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, VulkanSurface::getSurface(), &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, VulkanSurface::getSurface(), &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, VulkanSurface::getSurface(), &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, VulkanSurface::getSurface(), &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, VulkanSurface::getSurface(), &presentModeCount, details.presentModes.data());
    }

    return details;
}

bool VulkanPhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(
        CHEVALIER_DEVICE_EXTENSIONS_DEFAULTS.begin(),
        CHEVALIER_DEVICE_EXTENSIONS_DEFAULTS.end()
    );

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

VkSampleCountFlagBits VulkanPhysicalDevice::getMaxUsableSampleCount()
{
    VkPhysicalDeviceProperties physicalDeviceProperties;

    vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

    VkSampleCountFlags counts = ( physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts ) ;

    //Uhh do this?
    if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
    if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
    if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
    if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
    if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
    if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

    return VK_SAMPLE_COUNT_1_BIT;

}

VkPhysicalDevice VulkanPhysicalDevice::getPhysicalDevice()
{
    if (!physicalDevice) {
        pickPhysicalDevice();
    }

    return physicalDevice;
}

void VulkanPhysicalDevice::cleanup() {
    // Do nothing???
}


#pragma endregion

#pragma region VulkanSurface

void VulkanSurface::createSurface()
{

    if ( glfwCreateWindowSurface(
        VulkanInstance::GetVulkanInstance(),
        ChevGLFWWindow::getGLFWWindow(),
        nullptr,
        &vSurface) != VK_SUCCESS) {

        CHEV_MESSAGE_ERROR("Failed to create window surface!");

    }
}

VkSurfaceKHR VulkanSurface::getSurface()
{
    if (!vSurface) {

        createSurface();

    }

    return vSurface;

}

void VulkanSurface::cleanup() {
    
    vkDestroySurfaceKHR(
        VulkanInstance::GetVulkanInstance(),
        vSurface,
        nullptr
    );

}

#pragma endregion

#pragma region QueueFamilyIndices

void QueueFamilyIndices::findQueueFamilies(VkPhysicalDevice device)
{
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
    
    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, VulkanSurface::getSurface(), &presentSupport);

        if (presentSupport) {
            presentFamily = i;
        }

        if (isComplete()) {
            break;
        }

        i++;
    }

}

#pragma endregion

#pragma region VulkanLogicalDevice

void VulkanLogicalDevice::createLogicalDevice()
{
    QueueFamilyIndices indices = {};
    indices.findQueueFamilies(VulkanPhysicalDevice::getPhysicalDevice());


    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;
    deviceFeatures.sampleRateShading = VK_TRUE;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(CHEVALIER_DEVICE_EXTENSIONS_DEFAULTS.size());
    createInfo.ppEnabledExtensionNames = CHEVALIER_DEVICE_EXTENSIONS_DEFAULTS.data();

    if (vkCreateDevice(
        VulkanPhysicalDevice::getPhysicalDevice(),\
        &createInfo,
        nullptr,
        &logicalDevice) != VK_SUCCESS) {
        CHEV_MESSAGE_ERROR("Failed to Create Logical Device!");
    }

    vkGetDeviceQueue(logicalDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(logicalDevice, indices.presentFamily.value(), 0, &presentQueue);

}

VkDevice VulkanLogicalDevice::getLogicalDevice()
{
    if (!logicalDevice) {
        createLogicalDevice();
    }

    return logicalDevice;
}

void VulkanLogicalDevice::cleanup() {

    vkDestroyDevice(logicalDevice, nullptr);

}

#pragma endregion

#pragma region VulkanRenderPass

void VulkanRenderPass::CreateRenderPass(VkFormat swapchainImageFormat)
{

    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapchainImageFormat;
    colorAttachment.samples = MSAAResources::getMSAASampleCount();
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription colorAttachmentResolve{};
    colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachmentResolve.format = swapchainImageFormat;
    colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

    colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

    colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


    VkAttachmentReference colorAttachmentResolveRef{};
    colorAttachmentResolveRef.attachment = 2;
    colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;



    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = DepthResources::findDepthFormat();
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.samples = MSAAResources::getMSAASampleCount();
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    std::array attachments = { colorAttachment, depthAttachment, colorAttachmentResolve };


    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;
    subpass.pResolveAttachments = &colorAttachmentResolveRef;



    VkRenderPassCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

    createInfo.dependencyCount = 1;
    createInfo.pDependencies = &dependency;

    createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    createInfo.pAttachments = attachments.data();

    createInfo.subpassCount = 1;
    createInfo.pSubpasses = &subpass;


    if (vkCreateRenderPass(VulkanLogicalDevice::getLogicalDevice(), &createInfo, nullptr, &vRenderPass) != VK_SUCCESS) {

        CHEV_MESSAGE_ERROR("Failed to create Render Pass!")

    }

}

#pragma endregion

#pragma region SwapChainManager

void SwapChainManager::createSwapchain()
{
    SwapChainSupportDetails swapchainSupport = VulkanPhysicalDevice::querySwapChainSupport(VulkanPhysicalDevice::getPhysicalDevice());

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapchainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapchainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapchainSupport.capabilities);

    uint32_t imageCount = swapchainSupport.capabilities.minImageCount + 1;
    if (swapchainSupport.capabilities.maxImageCount > 0 && imageCount > swapchainSupport.capabilities.maxImageCount) {
        imageCount = swapchainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = VulkanSurface::getSurface();

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices;
    indices.findQueueFamilies(VulkanPhysicalDevice::getPhysicalDevice());
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = swapchainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    if (vkCreateSwapchainKHR(VulkanLogicalDevice::getLogicalDevice(), &createInfo, nullptr, &swapchain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(VulkanLogicalDevice::getLogicalDevice(), swapchain, &imageCount, nullptr);
    swapchainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(VulkanLogicalDevice::getLogicalDevice(), swapchain, &imageCount, swapchainImages.data());

    swapchainImageFormat = surfaceFormat.format;
    swapchainExtent = extent;
}

void SwapChainManager::cleanupSwapchain()
{
    VkDevice device = VulkanLogicalDevice::getLogicalDevice();
    for (auto framebuffer : swapchainFramebuffers) {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }

    for (auto imageView : swapchainImageViews) {
        vkDestroyImageView(device, imageView, nullptr);
    }

    vkDestroySwapchainKHR(device, swapchain, nullptr);
}

void SwapChainManager::createImageViews()
{
    swapchainImageViews.resize(swapchainImages.size());

    for (size_t i = 0; i < swapchainImages.size(); i++) {
        swapchainImageViews[i] = VulkanImageView::CreateImageView(swapchainImages[i], swapchainImageFormat, VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }
}

VkExtent2D SwapChainManager::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }
    else {
        int width, height;

        ChevGLFWWindow::getWindowFrameBufferExtent(&width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

VkPresentModeKHR SwapChainManager::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkSurfaceFormatKHR SwapChainManager::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

#pragma endregion

#pragma region VulkanImageView

VkImageView VulkanImageView::CreateImageView(VkImage image, VkFormat imageFormat, VkImageAspectFlags aspectFlags, uint32_t mipLevel)
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = imageFormat;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = mipLevel;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(VulkanLogicalDevice::getLogicalDevice(), &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        CHEV_MESSAGE_ERROR("failed to create image view!");
    }

    return imageView;
}

#pragma endregion

VkFormat VulkanImageFormat::getSupportedFormat(const std::vector<VkFormat> candidates, VkImageTiling tiling, VkFormatFeatureFlagBits features)
{
    for (const auto format : candidates) {
        VkFormatProperties properties;
        vkGetPhysicalDeviceFormatProperties(VulkanPhysicalDevice::getPhysicalDevice(), format, &properties);


        if (tiling == VkImageTiling::VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & features) == features) {
            return format;
        }

        if (tiling == VkImageTiling::VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    CHEV_MESSAGE_ERROR("Failed to find Supported Format");
}

void VulkanCommandPool::CreateCommandPool()
{
    QueueFamilyIndices queueFam;
    queueFam.findQueueFamilies(VulkanPhysicalDevice::getPhysicalDevice());

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFam.graphicsFamily.value();

    if (vkCreateCommandPool(
        VulkanLogicalDevice::getLogicalDevice(),
        &poolInfo,
        nullptr,
        &commandPool) != VK_SUCCESS) {
        CHEV_MESSAGE_ERROR("Failed to create Command Pool");
    }


}

void VulkanCommandBuffers::CreateCommandBuffers()
{
    commandBuffers.resize(CHEVALIER_MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = VulkanCommandPool::getCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

    if (vkAllocateCommandBuffers(VulkanLogicalDevice::getLogicalDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        CHEV_MESSAGE_ERROR("Failed to allocate Buffers");
    }

}

VkCommandBuffer VulkanCommandBuffers::beginSingleTimeCommands()
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = VulkanCommandPool::getCommandPool();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(VulkanLogicalDevice::getLogicalDevice(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void VulkanCommandBuffers::endSingleTimeCommands(VkCommandBuffer buffer)
{
    vkEndCommandBuffer(buffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &buffer;

    vkQueueSubmit(VulkanLogicalDevice::getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(VulkanLogicalDevice::getGraphicsQueue());

    vkFreeCommandBuffers(VulkanLogicalDevice::getLogicalDevice(), VulkanCommandPool::getCommandPool(), 1, &buffer);
}

void VulkanImage::createImage(const ImageCreationInfo& info, VkImage& image, VkDeviceMemory& imageMemory)
{
    VkImageCreateInfo imageInfo{};

    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = info.width;
    imageInfo.extent.height = info.height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = info.mipLevel;
    imageInfo.arrayLayers = 1;
    imageInfo.format = info.format;
    imageInfo.tiling = info.tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = info.usage;
    imageInfo.samples = MSAAResources::getMSAASampleCount();
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(VulkanLogicalDevice::getLogicalDevice(), &imageInfo, nullptr, &image) != VK_SUCCESS) {
        CHEV_MESSAGE_ERROR(" Failed to create Vulkan Image ");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(VulkanLogicalDevice::getLogicalDevice(), image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, info.properties);

    if (vkAllocateMemory(VulkanLogicalDevice::getLogicalDevice(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        CHEV_MESSAGE_ERROR(" Failed to allocate Image memory ");
    }

    vkBindImageMemory(VulkanLogicalDevice::getLogicalDevice(), image, imageMemory, 0);


}

VkImageView VulkanImage::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevel)
{
    VkImageViewCreateInfo info{};

    info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    info.image = image;
    info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    info.format = format;
    info.subresourceRange.aspectMask = aspectFlags;
    info.subresourceRange.baseMipLevel = 0;
    info.subresourceRange.levelCount = mipLevel;
    info.subresourceRange.baseArrayLayer = 0;
    info.subresourceRange.layerCount = 1;

    VkImageView newImageView;

    if (vkCreateImageView(VulkanLogicalDevice::getLogicalDevice(), &info, nullptr, &newImageView) != VK_SUCCESS)
    {
        CHEV_MESSAGE_ERROR(" Failed to create Image View ");
    }

    return newImageView;

}

void VulkanImage::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageAspectFlags aspectMask, uint32_t mipLevel)
{
    VkCommandBuffer commandBuffer = VulkanCommandBuffers::beginSingleTimeCommands();

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;

    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

    barrier.image = image;
    //barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT | VK_IMAGE_ASPECT_DEPTH_BIT;
    barrier.subresourceRange.baseMipLevel = 0;

    if (mipLevel == 0) {
        throw std::runtime_error("caught mip error in transition image layout");
    }

    barrier.subresourceRange.levelCount = mipLevel;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = 0;

    barrier.subresourceRange.aspectMask = aspectMask;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = {};
        barrier.dstAccessMask = VkAccessFlagBits::VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VkImageLayout::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VkAccessFlagBits::VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VkAccessFlagBits::VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else {
        throw std::invalid_argument("unsupported layout transition!");
    }


    vkCmdPipelineBarrier(
        commandBuffer,
        sourceStage, destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier

    );


    VulkanCommandBuffers::endSingleTimeCommands(commandBuffer);
}

uint32_t VulkanImage::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(VulkanPhysicalDevice::getPhysicalDevice(), &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    CHEV_MESSAGE_ERROR(" Failed to find Memory Type ");


}

VkBuffer VulkanBuffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
    //Creation Object
    VkBufferCreateInfo bufferInfo{};

    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;

    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;


    VkDevice logicalDevice = VulkanLogicalDevice::getLogicalDevice();


    if (vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create buffer");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(logicalDevice, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = VulkanImage::findMemoryType(memRequirements.memoryTypeBits, properties);


    if (vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate buffer memory");
    }

    vkBindBufferMemory(logicalDevice, buffer, bufferMemory, 0);
}
