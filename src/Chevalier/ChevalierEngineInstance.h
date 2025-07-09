#pragma once
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include "ChevalierCore.h"
#include "ChevalierRenderer.h"
#include "ChevalierEngineStatics.h"


const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME

};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

//Main class of the engine... this runs the rest of the vulkan implementation
class ChevalierEngineInstance
{

	//TODO: MAKE SINGLETON

public:

	//Constructor
	ChevalierEngineInstance();

	//Function called by main to run the instance
	void RunInstance();

protected:



	//Main inits
	//Window
	void initWindow();

	//Vulkan
	void initVulkan();

	//Creations
	void createInstance();
	void setupDebugMessenger();
	void createSurface();
	void pickPhysicalDevice();
	void createLogicalDevice();

	//Cleanup
	void cleanup();




	//Singleton
	static ChevalierEngineInstance* mInstance;
	static ChevalierEngineInstance* getInstance() { return mInstance; }


	//Helpers
	bool isDeviceSuitable(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
public:
	
	//Friends
	friend ChevalierEngineStatics;
	
protected:
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	bool checkValidationLayerSupport();

	
	VkSampleCountFlagBits getMaxUsableSampleCount();

	//Instance Members

	//Vulkan Variables
	//Instance
	VkInstance vInstance;

	//Surface
	VkSurfaceKHR vSurface;

	//Window
	GLFWwindow* mWindow;

	//Physical Device - GPU
	VkPhysicalDevice vPhysicalDevice = VK_NULL_HANDLE;

	//Logical Device
	VkDevice vDevice;

	//Renderer Variables
	ChevalierRenderer mRenderer;

	//Debug Stuff
	VkDebugUtilsMessengerEXT vDebugMessenger;


	//Callbacks
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
		auto app = reinterpret_cast<ChevalierEngineInstance*>(glfwGetWindowUserPointer(window));
		app->mRenderer.SetFrameBufferResized(true);
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}

	//TODO: Should be separated
	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
	VkQueue graphicsQueue;
	VkQueue presentQueue;





};

