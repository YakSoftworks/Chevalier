#include "ChevalierRenderer.h"

void ChevalierRenderer::InitRenderer()
{
	// SwapChains
	SwapChainManager::createSwapchain();
	SwapChainManager::createImageViews();

	// RenderPass
	mRenderPass.CreateRenderPass(SwapChainManager::getSwapchainImageFormat());

	// Command Pool
	VulkanCommandPool::getCommandPool();

	// Framebuffer
	ChevFramebuffer::InitFramebuffers(nullptr, nullptr, mRenderPass.getRenderPass());
	

	// Commandbuffer
	mCommandBuffers.CreateCommandBuffers();


	// SyncObjects
	SyncObjects::createSyncObjects();

	// Materials

	// Models
}

void ChevalierRenderer::LoopRenderer()
{
	// Run Program Tick

	// Run Draw Tick

	
}

void ChevalierRenderer::CleanupRenderer()
{
}
