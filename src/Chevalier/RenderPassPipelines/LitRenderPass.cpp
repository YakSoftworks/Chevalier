#include "LitRenderPass.h"

#include "Objects/Materials/ChevalierMaterial.h"

VkRenderPass LitRenderPass::GetRenderPassRef()
{

    if (!mRenderPass) {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = SwapChainManager::getSwapchainImageFormat();
        colorAttachment.samples = MSAAResources::getMSAASampleCount();
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


        VkAttachmentDescription colorAttachmentResolve{};
        colorAttachmentResolve.samples = MSAAResources::getMSAASampleCount();
        colorAttachmentResolve.format = SwapChainManager::getSwapchainImageFormat();
        colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

        colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

        colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentReference normalAttachmentRef{};
        colorAttachmentRef.attachment = 1;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
     

        VkAttachmentReference depthAttachmentRef{};
        depthAttachmentRef.attachment = 2;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;


        VkAttachmentReference colorAttachmentResolveRef{};
        colorAttachmentResolveRef.attachment = 3;
        colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = DepthResources::findDepthFormat();
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.samples = MSAAResources::getMSAASampleCount();
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription albedoAttachment{};
        albedoAttachment.format = VK_FORMAT_R8G8B8A8_UNORM;
        albedoAttachment.samples = MSAAResources::getMSAASampleCount();
        albedoAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        albedoAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        albedoAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        albedoAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        albedoAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        albedoAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription normalAttachment{};
        normalAttachment.format = VK_FORMAT_A2B10G10R10_UNORM_PACK32;
        normalAttachment.samples = MSAAResources::getMSAASampleCount();
        normalAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        normalAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        normalAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        normalAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        normalAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        normalAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        std::array attachments = { colorAttachment,normalAttachment, depthAttachment, colorAttachmentResolve };

        VkSubpassDependency geometryDependency{};
        geometryDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        geometryDependency.dstSubpass = 0;
        geometryDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
        geometryDependency.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        geometryDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        geometryDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;


        VkSubpassDependency lightingDependency{};
        lightingDependency.srcSubpass = 0;
        lightingDependency.dstSubpass = 1;
        lightingDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
        lightingDependency.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        lightingDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        lightingDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;


        VkAttachmentReference colorAttachments[] = { colorAttachmentRef, normalAttachmentRef };


        VkSubpassDescription geometrySubpass{};
        geometrySubpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        geometrySubpass.colorAttachmentCount = 1;
        geometrySubpass.pColorAttachments = colorAttachments;
        geometrySubpass.pDepthStencilAttachment = &depthAttachmentRef;
        geometrySubpass.pResolveAttachments = &colorAttachmentResolveRef;
        

        VkAttachmentReference inputAttachments[] = { colorAttachmentRef, normalAttachmentRef, depthAttachmentRef };

        VkSubpassDescription lightingSubpass{};
        lightingSubpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        lightingSubpass.colorAttachmentCount = 1;
        lightingSubpass.pColorAttachments = &colorAttachmentRef;
        lightingSubpass.pDepthStencilAttachment = &depthAttachmentRef;
        lightingSubpass.pResolveAttachments = &colorAttachmentResolveRef;
        lightingSubpass.inputAttachmentCount = 3;
        lightingSubpass.pInputAttachments = inputAttachments;


        std::array<const VkSubpassDescription, 2> subpasses = { geometrySubpass, lightingSubpass };
        std::array<const VkSubpassDependency, 2> subpassDependencies = { geometryDependency, lightingDependency };



        VkRenderPassCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

        createInfo.dependencyCount = static_cast<uint32_t>(subpassDependencies.size());
        createInfo.pDependencies = subpassDependencies.data();

        createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        createInfo.pAttachments = attachments.data();

        createInfo.subpassCount = static_cast<uint32_t>(subpasses.size());
        createInfo.pSubpasses = subpasses.data();





        if (vkCreateRenderPass(VulkanLogicalDevice::getLogicalDevice(), &createInfo, nullptr, &mRenderPass) != VK_SUCCESS) {

            CHEV_MESSAGE_ERROR("Failed to create Render Pass!")
        }
    }

    return mRenderPass;
}

void LitRenderPass::RecordRenderPass(VkCommandBuffer buffer, uint32_t imageIndex)
{
    // Begin Render Pass

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = GetRenderPassRef();
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

    // Draw Geometry to Albedo Texture
    GeometryManager::getGeometryManager()->PerformGeometryPass(buffer, imageIndex);

    //// Says next subpass' commands are also in this buffer
    //vkCmdNextSubpass(buffer, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdNextSubpass(buffer, VK_SUBPASS_CONTENTS_INLINE);


    // Lighting Pass

    // Bind resources
    lightingMaterial->BindMaterial(&buffer, imageIndex);



    // Draw full screen shape
    vkCmdDraw(buffer, 3, 1, 0, 0);
    


    // End Render Pass
    vkCmdEndRenderPass(buffer);



}

void LitRenderPass::InitializeRP()
{

    // Lighting Material

    VkShaderModule lightingShaderVertModule = ChevalierMaterial::createShaderModule(FileReader::readFile("content/shaders/ChevalierII/LightVert.spv"));
    VkShaderModule lightingShaderFragModule = ChevalierMaterial::createShaderModule(FileReader::readFile("content/shaders/ChevalierII/LightFrag.spv"));

    lightingMaterial = new ChevalierMaterial();

    lightingMaterial->init_pipeline(this, lightingShaderVertModule, lightingShaderFragModule, 1);

}

void LitRenderPass::CreateFrameBuffers()
{

    InitRenderPipelineResources();

    SwapChainManager::swapchainFramebuffers.resize(SwapChainManager::swapchainImageViews.size());

    for (size_t i = 0; i < SwapChainManager::swapchainImageViews.size(); i++) {

        VkImageView attachments[] = {
            mAlbedoResources.colorImageView,
            mNormalResources.colorImageView,
            mDepthResources.depthImageView,
            SwapChainManager::swapchainImageViews[i]
        };

        VkFramebufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        bufferInfo.renderPass = GetRenderPassRef();
        bufferInfo.attachmentCount = 4;
        bufferInfo.pAttachments = attachments;
        bufferInfo.width = SwapChainManager::swapchainExtent.width;
        bufferInfo.height = SwapChainManager::swapchainExtent.height;
        bufferInfo.layers = 1;

        if (vkCreateFramebuffer(VulkanLogicalDevice::getLogicalDevice(), &bufferInfo, nullptr, &SwapChainManager::swapchainFramebuffers[i]) != VK_SUCCESS) {
            CHEV_MESSAGE_ERROR("Failed to create Framebuffer");
        }

    }


}

void LitRenderPass::InitRenderPipelineResources()
{
    mAlbedoResources.CreateColorResources(CHEVALIER_WINDOW_WIDTH_DEFAULT, CHEVALIER_WINDOW_HEIGHT_DEFAULT);
    mNormalResources.CreateColorResources(CHEVALIER_WINDOW_WIDTH_DEFAULT, CHEVALIER_WINDOW_HEIGHT_DEFAULT);
    mDepthResources.CreateDepthResources(CHEVALIER_WINDOW_WIDTH_DEFAULT, CHEVALIER_WINDOW_HEIGHT_DEFAULT);


}
