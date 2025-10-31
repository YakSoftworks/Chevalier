#include "ChevalierInstance.h"

#include "ChevalierGraphicsCore.h"

void ChevalierInstance::InitVulkan()
{

	//Window
	if (!ChevGLFWWindow::getGLFWWindow()) {
		CHEV_MESSAGE_ERROR("Unable to start Window!");
	}

	//Instance
	if (VulkanInstance::GetVulkanInstance() == nullptr) {
		CHEV_MESSAGE_ERROR("Unable to start vulkan: Instance Error");
	}

	//Debug
	mDebugMessenger.setupDebugMessenger();

	//Surface
	if (VulkanSurface::getSurface() == nullptr) {
		CHEV_MESSAGE_ERROR("Unable to start vulkan: Surface Error");
	}

	//Physical Device
	if (VulkanPhysicalDevice::getPhysicalDevice() == nullptr) {
		CHEV_MESSAGE_ERROR("Unable to start vulkan: Physical Device Error");
	}

	//Logical Device
	if (VulkanLogicalDevice::getLogicalDevice() == nullptr) {
		CHEV_MESSAGE_ERROR("Unable to start vulkan: Logical Device Error");
	}



}

void ChevalierInstance::LoopProgram()
{
	while (!CheckShouldClose()) {
		mRenderer.LoopRenderer();
	}

	vkDeviceWaitIdle(VulkanLogicalDevice::getLogicalDevice());
}

bool ChevalierInstance::CheckShouldClose()
{
	return mRenderer.CheckShouldClose();
}

void ChevalierInstance::InitChevalier()
{

	InitVulkan();

	//Renderer
	mRenderer.InitRenderer();

	CHEV_MESSAGE_LOG("Chevalier Application Setup Complete -- Commencing Render Loop");

	LoopProgram();
}

void ChevalierInstance::CleanupChevalier()
{

	//Destroy in reverse order of creation 

	mRenderer.CleanupRenderer();

	VulkanLogicalDevice::cleanup();

	VulkanSurface::cleanup();

	VulkanInstance::cleanup();

	ChevGLFWWindow::cleanup();

	CHEV_MESSAGE_LOG("Chevalier Application Teardown Complete");

}
