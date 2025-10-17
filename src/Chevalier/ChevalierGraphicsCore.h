#pragma once

#include "Core.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include "HelperObjects/VulkanObjects.h"

const std::vector<const char*> CHEVALIER_DEVICE_EXTENSIONS_DEFAULTS =
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};


