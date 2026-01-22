#include "ChevalierRenderer.h"

#include "Objects/Materials/ChevalierMaterial.h"

#include "RenderPassManager.h"

// Types of render passes
#include "RenderPassPipelines/UnlitRenderPass.h"
#include "RenderPassPipelines/WireframeRenderPass.h"


//DEBUG
#include "Objects/Components/MeshComponent.h"

void ChevalierRenderer::InitRenderer()
{
	// SwapChains
	SwapChainManager::createSwapchain();
	SwapChainManager::createImageViews();

	// RenderPass
	//mRenderPass.CreateRenderPass(SwapChainManager::getSwapchainImageFormat());

    //mRenderPassManager = new UnlitRenderPass();
    mRenderPassManager = new WireframeRenderPass();
    mRenderPassManager->GetRenderPassRef();

	// Command Pool
	VulkanCommandPool::getCommandPool();

    // Depth Resources
    mDepthResources.CreateDepthResources(CHEVALIER_WINDOW_WIDTH_DEFAULT, CHEVALIER_WINDOW_HEIGHT_DEFAULT);

    // Color Resources
    mColorResources.CreateColorResources(CHEVALIER_WINDOW_WIDTH_DEFAULT, CHEVALIER_WINDOW_HEIGHT_DEFAULT);


	// Framebuffer
	ChevFramebuffer::InitFramebuffers(mColorResources.colorImageView, mDepthResources.depthImageView, mRenderPassManager->GetRenderPassRef());
	



	// Commandbuffer
	mCommandBuffers.CreateCommandBuffers();



	// SyncObjects
	SyncObjects::createSyncObjects();


    //Init our global shader data
    ChevalierMaterial::sGlobalDataManager.init();

	// Materials

	// Models

    /*Actor* pActor = new Actor();
    MeshComponent* actorCube = new SphereComponent();
    actorCube->LoadMeshComponent();
    actorCube->renderObjectID = 0;

    Actor* pActor2 = new Actor();
    CylinderComponent* actorCube2 = new CylinderComponent();
    actorCube2->LoadMeshComponent();
    actorCube2->renderObjectID = 1;


    RenderObjects.push_back(actorCube);
    RenderObjects.push_back(actorCube2);

    ChevalierMaterial* myMaterial = new ChevalierMaterial();

    actorCube->pMaterial = myMaterial;
    actorCube2->pMaterial = myMaterial;

    myMaterial->init_pipeline(mRenderPass.getRenderPass());

    pActor->AddComponentToActor(actorCube);
    pActor2->AddComponentToActor(actorCube2);

    actors.push_back(pActor);
    actors.push_back(pActor2);*/




    //Debug:

    //Bowling Scene

    //Setting Object Transforms
    MeshComponent* floor = new PlaneComponent();
    floor->renderObjectID = 1;
    floor->componentTransform.position = glm::vec3(0.f, 0.f, 1.5f);
    floor->componentTransform.rotation = glm::vec3(0.f, 90.f, 90.f);
    floor->componentTransform.scale = glm::vec3(7.f, 1.f, 5.f);
    floor->LoadMeshComponent();
    RenderObjects.push_back(floor);

    //////Ball
    MeshComponent* ball = new SphereComponent();
    ball->renderObjectID = 2;
    ball->componentTransform.position = glm::vec3(0.f, 2.f, 1.f);
    ball->componentTransform.rotation = glm::vec3(0.f, 0.f, 0.f);
    ball->componentTransform.scale = glm::vec3(.5f, .5f, .5f);
    ball->LoadMeshComponent();
    RenderObjects.push_back(ball);

    //////Pin
    MeshComponent* pin1 = new CylinderComponent();
    pin1->renderObjectID = 3;
    pin1->componentTransform.position = glm::vec3(0.f, -2.f, 1.f);
    pin1->componentTransform.rotation = glm::vec3(0.f, 0.f, 0.f);
    pin1->componentTransform.scale = glm::vec3(.25f, .25f, .5f);
    pin1->LoadMeshComponent();
    RenderObjects.push_back(pin1);

    MeshComponent* pin2 = new CylinderComponent();
    pin2->renderObjectID = 4;
    pin2->componentTransform.position = glm::vec3(-.5f, -2.5f, 1.f);
    pin2->componentTransform.rotation = glm::vec3(0.f, 0.f, 0.f);
    pin2->componentTransform.scale = glm::vec3(.25f, .25f, .5f);
    pin2->LoadMeshComponent();
    RenderObjects.push_back(pin2);

    MeshComponent* pin3 = new CylinderComponent();
    pin3->renderObjectID = 5;
    pin3->componentTransform.position = glm::vec3(.5f, -2.5f, 1.f);
    pin3->componentTransform.rotation = glm::vec3(0.f, 0.f, 0.f);
    pin3->componentTransform.scale = glm::vec3(.25f, .25f, .5f);
    pin3->LoadMeshComponent();
    RenderObjects.push_back(pin3);


    LightComponent* light1 = new PointLightComponent();
    light1->componentTransform.position = glm::vec3(0.f, 4.f, 0.f);
    light1->mLightInfo.color = glm::vec4(0.f, 0.f, 1.f, 1.f);


    ChevalierMaterial* myMaterial = new ChevalierMaterial();
    myMaterial->init_pipeline(mRenderPassManager);

    floor->pMaterial = myMaterial;
    ball->pMaterial = myMaterial;
    pin1->pMaterial = myMaterial;
    pin2->pMaterial = myMaterial;
    pin3->pMaterial = myMaterial;

    
    // N Plane Sim

    /*MeshComponent* simPlane = new SphereComponent();
    simPlane->renderObjectID = 7;
    simPlane->componentTransform.position = glm::vec3(0.f, 0.f, 0.f);
    simPlane->componentTransform.rotation = glm::vec3(0.f, 0.f, 0.f);
    simPlane->componentTransform.scale = glm::vec3(1.f, 1.f, 1.f);
    simPlane->LoadMeshComponent();
    RenderObjects.push_back(simPlane);


    ChevalierMaterial* SimMaterial = new ChevalierMaterial();
    SimMaterial->init_pipeline(mRenderPass.getRenderPass());

    simPlane->pMaterial = SimMaterial;*/

}

void ChevalierRenderer::LoopRenderer()
{
    glfwPollEvents();

    float deltaTime = 0.f;

    // Draw Current Frame
    drawFrame();
    
	
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

    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    GlobalDataObject globalDataThisFrame{};
    globalDataThisFrame.viewMat = glm::lookAt(glm::vec3(5, 0.0f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f));
    globalDataThisFrame.perspectiveMat = glm::perspective(
        glm::radians(45.f),
        ((float)CHEVALIER_WINDOW_WIDTH_DEFAULT / (float)CHEVALIER_WINDOW_HEIGHT_DEFAULT),
        0.01f,
        10000.f
    );

    globalDataThisFrame.timeSinceStart = 1.f;

    //CHEV_MESSAGE_LOG("Current Time: " << 2);
     
    globalDataThisFrame.projViewMat = globalDataThisFrame.perspectiveMat * globalDataThisFrame.viewMat;

    globalDataThisFrame.debugModelMat = glm::rotate(glm::mat4(1.0f), time * glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    
    ChevalierMaterial::UpdateGlobalDescriptor(&globalDataThisFrame, currentFrame);


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
    renderPassInfo.renderPass = mRenderPassManager->GetRenderPassRef();
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

    // Draw Geometry
    performGeometryPass(buffer);

    //// Says next subpass' commands are also in this buffer
    //vkCmdNextSubpass(buffer, VK_SUBPASS_CONTENTS_INLINE);


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

	ChevFramebuffer::InitFramebuffers(mColorResources.colorImageView, mDepthResources.depthImageView, mRenderPassManager->GetRenderPassRef());


}

void ChevalierRenderer::performGeometryPass(VkCommandBuffer buffer)
{
    ObjectShaderData* PerObjectDataThisFrame = static_cast<ObjectShaderData*>(ChevalierMaterial::sGlobalDataManager.modelMatrixBuffersMapped[currentFrame]);

    for (ChevalierRenderObjectInterface* object : RenderObjects) {

        if (object->IsObjectDrawable()) {
            //ChevalierMaterialInterface* material = object->GetObjectMaterial();

            // Bind Material
            object->GetObjectMaterial()->BindMaterial(&buffer, currentFrame);
            //draw Object

            object->DrawObject(buffer, PerObjectDataThisFrame);

        }
    }
}

bool ChevalierRenderer::CheckShouldClose() {
    return glfwWindowShouldClose(ChevGLFWWindow::getGLFWWindow());
}