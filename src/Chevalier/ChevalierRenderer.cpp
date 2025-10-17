#include "ChevalierRenderer.h"

#include "Objects/Materials/ChevalierMaterial.h"

void ChevalierRenderer::InitRenderer()
{
	// SwapChains
	SwapChainManager::createSwapchain();
	SwapChainManager::createImageViews();

	// RenderPass
	mRenderPass.CreateRenderPass(SwapChainManager::getSwapchainImageFormat());

	// Command Pool
	VulkanCommandPool::getCommandPool();

    // Depth Resources
    mDepthResources.CreateDepthResources(CHEVALIER_WINDOW_WIDTH_DEFAULT, CHEVALIER_WINDOW_HEIGHT_DEFAULT);

    // Color Resources
    mColorResources.CreateColorResources(CHEVALIER_WINDOW_WIDTH_DEFAULT, CHEVALIER_WINDOW_HEIGHT_DEFAULT);


	// Framebuffer
	ChevFramebuffer::InitFramebuffers(mColorResources.colorImageView, mDepthResources.depthImageView, mRenderPass.getRenderPass());
	



	// Commandbuffer
	mCommandBuffers.CreateCommandBuffers();



	// SyncObjects
	SyncObjects::createSyncObjects();

    //Init our global shader data
    ChevalierMaterial::sGlobalDataManager.init();

	// Materials



	// Models
}

void ChevalierRenderer::LoopRenderer()
{

    float deltaTime = 0.f;

    // Run Program Tick
    if (CurrentMap)
    {
        CurrentMap->Tick(deltaTime);
        // Run Draw Tick
        drawFrame();
    }
	
}

void ChevalierRenderer::CleanupRenderer()
{
    // Perform destructions
}

void ChevalierRenderer::drawFrame()
{
	// Check Sync status

	vkWaitForFences(VulkanLogicalDevice::getLogicalDevice(), 1, &SyncObjects::inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(VulkanLogicalDevice::getLogicalDevice(), SwapChainManager::getSwapChain(), UINT64_MAX, SyncObjects::imageAvailableSemaphore[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateWindowResources();
        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    vkResetFences(VulkanLogicalDevice::getLogicalDevice(), 1, &SyncObjects::inFlightFences[currentFrame]);

    vkResetCommandBuffer(mCommandBuffers.getCommandBufferAt(currentFrame), /*VkCommandBufferResetFlagBits*/ 0);
    

	// Consider updating our buffers - in theory we do that during the tick

	// Record Command Buffer
	recordCommandBuffer(mCommandBuffers.getCommandBufferAt(currentFrame), imageIndex);


	// Submit Buffer to Present Queue

	VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { SyncObjects::imageAvailableSemaphore[currentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = mCommandBuffers.getCommandBufferRefAt(currentFrame);

    VkSemaphore signalSemaphores[] = { SyncObjects::renderFinishedSemaphore[currentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(VulkanLogicalDevice::getGraphicsQueue(), 1, &submitInfo, SyncObjects::inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapchains[] = { SwapChainManager::getSwapChain() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapchains;

    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(VulkanLogicalDevice::getPresentQueue(), &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
        framebufferResized = false;
        recreateWindowResources();
    }
    else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % CHEVALIER_MAX_FRAMES_IN_FLIGHT;



}

void ChevalierRenderer::recordCommandBuffer(VkCommandBuffer buffer, uint32_t imageIndex)
{
	// Begin Command Buffer
	VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(buffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }


	// Begin Render Pass

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = mRenderPass.getRenderPass();
    renderPassInfo.framebuffer = SwapChainManager::getSwapChainFramebufferAt(imageIndex);;
	


    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = SwapChainManager::getExtent();

    VkClearValue clearColor[2] = { {{0.0f, 0.0f, 0.0f, 1.0f}}, {1.0f, 0} };
    renderPassInfo.clearValueCount = 2;
    renderPassInfo.pClearValues = clearColor;

    vkCmdBeginRenderPass(buffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	// Set Viewport/Scissor/etc

	VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)SwapChainManager::getExtent().width;
    viewport.height = (float)SwapChainManager::getExtent().height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(buffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = SwapChainManager::getExtent();
    vkCmdSetScissor(buffer, 0, 1, &scissor);



	// TODO: Record Objects

	for( ChevalierRenderObjectInterface* object : RenderObjects){

		if(object->IsObjectDrawable()){
			ChevalierMaterialInterface* material = object->GetObjectMaterial();

			// Bind Material

			//draw Object
			object->DrawObject(buffer);

		}

	}



	// End Render Pass
	vkCmdEndRenderPass(buffer);

	// End Command Buffer
    if (vkEndCommandBuffer(buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}

void ChevalierRenderer::recreateWindowResources() {

	// Fix our swap chain
	int width = 0, height = 0;
    glfwGetFramebufferSize(ChevGLFWWindow::getGLFWWindow(), &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(ChevGLFWWindow::getGLFWWindow(), &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(VulkanLogicalDevice::getLogicalDevice());

    SwapChainManager::cleanupSwapchain();

    SwapChainManager::createSwapchain();
    SwapChainManager::createImageViews();

	// Cleanup our swapchain dependent resources
	mColorResources.cleanup();
	mDepthResources.cleanup();


	// Recreate Resources
	mColorResources.CreateColorResources(width, height);
	mDepthResources.CreateDepthResources(width, height);

	ChevFramebuffer::InitFramebuffers(mColorResources.colorImageView, mDepthResources.depthImageView, mRenderPass.getRenderPass());


}