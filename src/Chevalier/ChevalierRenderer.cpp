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
	

	// Depth Resources
	mDepthResources.CreateDepthResources(CHEVALIER_WINDOW_WIDTH_DEFAULT, CHEVALIER_WINDOW_HEIGHT_DEFAULT);


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

void ChevalierRenderer::drawFrame()
{
	// Check Sync status


	// Record Command Buffer

	// Submit Buffer to Present Queue
}

void ChevalierRenderer::recordCommandBuffer(VkCommandBuffer buffer, uint32_t imageIndex)
{
	// Begin Command Buffer

	// Begin Render Pass

	// Set Viewport/Scissor/etc

	// Record Objects

	// End Render Pass

	// End Command Buffer
}
