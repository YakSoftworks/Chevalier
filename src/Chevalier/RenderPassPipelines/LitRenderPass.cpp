#include "LitRenderPass.h"

#include "Objects/Materials/ChevalierMaterial.h"


VkRenderPass LitRenderPass::GetRenderPassRef()
{

    if (!mRenderPass) {


        // Attachments:

        // Geometry Pass Attachments:

        VkAttachmentDescription finalColorAttachment{};

        finalColorAttachment.format = SwapChainManager::getSwapchainImageFormat();
        finalColorAttachment.samples = MSAAResources::getMSAASampleCount();

        finalColorAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        finalColorAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        finalColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        finalColorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

        finalColorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        finalColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
        


        VkAttachmentDescription depthAttachment{};

        depthAttachment.format = mDepthResources.findDepthFormat();
        depthAttachment.samples = MSAAResources::getMSAASampleCount();

        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;




        VkAttachmentDescription albedoAttachment{};

        albedoAttachment.format = SwapChainManager::getSwapchainImageFormat();
        albedoAttachment.samples = MSAAResources::getMSAASampleCount();

        albedoAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        albedoAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        albedoAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        albedoAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

        albedoAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        albedoAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;


        VkAttachmentDescription normalAttachment{};

        normalAttachment.format = VK_FORMAT_A2B10G10R10_UNORM_PACK32;
        normalAttachment.samples = MSAAResources::getMSAASampleCount();

        normalAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        normalAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        normalAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        normalAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

        normalAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        normalAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;


        VkAttachmentReference geometryAlbedoAttachmentRef{};
        geometryAlbedoAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        geometryAlbedoAttachmentRef.attachment = 2;

        VkAttachmentReference geometryNormalAttachmentRef{};
        geometryNormalAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        geometryNormalAttachmentRef.attachment = 3;

        VkAttachmentReference geometryDepthAttachmentRef{};
        geometryDepthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
        geometryDepthAttachmentRef.attachment = 2;

        VkAttachmentReference geometryPassColorAttachmentRefs[] = { geometryAlbedoAttachmentRef, geometryNormalAttachmentRef };

        VkSubpassDescription geometryPassDescription{};
        geometryPassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        geometryPassDescription.inputAttachmentCount = 0;

        geometryPassDescription.colorAttachmentCount = 2;
        geometryPassDescription.pColorAttachments = geometryPassColorAttachmentRefs;

        geometryPassDescription.pDepthStencilAttachment = &geometryDepthAttachmentRef;

        VkAttachmentReference lightingPassFinalColorAttachmentRef{};
        lightingPassFinalColorAttachmentRef.attachment = 0;
        lightingPassFinalColorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentReference lightingPassDepthAttachmentRef{};
        lightingPassDepthAttachmentRef.attachment = 1;
        lightingPassDepthAttachmentRef.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        VkAttachmentReference lightingPassAlbedoAttachmentRef{};
        lightingPassAlbedoAttachmentRef.attachment = 2;
        lightingPassAlbedoAttachmentRef.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        VkAttachmentReference lightingPassNormalAttachmentRef{};
        lightingPassNormalAttachmentRef.attachment = 3;
        lightingPassNormalAttachmentRef.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;


        VkAttachmentReference lightingPassInputAttachmentRefs[] = { 
            lightingPassDepthAttachmentRef,
            lightingPassAlbedoAttachmentRef,
            lightingPassNormalAttachmentRef
        };

        VkSubpassDescription lightingPassDescription{};
        lightingPassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

        lightingPassDescription.inputAttachmentCount = 3;
        lightingPassDescription.pInputAttachments = lightingPassInputAttachmentRefs;

        lightingPassDescription.colorAttachmentCount = 1;
        lightingPassDescription.pColorAttachments = &lightingPassFinalColorAttachmentRef;

        // Leave Depth Stencil null


        // Create Dependencies

        VkSubpassDependency geometryColorAttachmentDependency{};
        geometryColorAttachmentDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        geometryColorAttachmentDependency.dstSubpass = 1;

        geometryColorAttachmentDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        geometryColorAttachmentDependency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        geometryColorAttachmentDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        geometryColorAttachmentDependency.dstAccessMask = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;

        geometryColorAttachmentDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

        VkSubpassDependency geometryDepthAttachmentDependency{};

        geometryDepthAttachmentDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

        geometryDepthAttachmentDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        geometryDepthAttachmentDependency.dstSubpass = 1;

        geometryColorAttachmentDependency.srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
        geometryColorAttachmentDependency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;

        geometryColorAttachmentDependency.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        geometryColorAttachmentDependency.dstAccessMask = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;

        VkAttachmentDescription renderPassAttachments[] = { finalColorAttachment, depthAttachment, albedoAttachment, normalAttachment };
        VkSubpassDependency renderPassDependencies[] = { geometryColorAttachmentDependency, geometryDepthAttachmentDependency };
        VkSubpassDescription renderPassSubpasses[] = { geometryPassDescription, lightingPassDescription };

        VkRenderPassCreateInfo createInfo{};
        createInfo.attachmentCount = 4;
        createInfo.pAttachments = renderPassAttachments;

        createInfo.dependencyCount = 2;
        createInfo.pDependencies = renderPassDependencies;

        createInfo.subpassCount = 2;
        createInfo.pSubpasses = renderPassSubpasses;

        createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;


        VkResult result = vkCreateRenderPass(VulkanLogicalDevice::getLogicalDevice(), &createInfo, nullptr, &mRenderPass);
        if(result != VK_SUCCESS) {

            CHEV_MESSAGE_ERROR("Failed to create Render Pass!")

        }

        sRenderPassManager = this;


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

    InitRenderPipelineResources();

    lightingMaterial = new LightingShader();

    lightingMaterial->init_pipeline(this, lightingShaderVertModule, lightingShaderFragModule, 1);

    

}

void LitRenderPass::CreateFrameBuffers()
{

    SwapChainManager::swapchainFramebuffers.resize(SwapChainManager::swapchainImageViews.size());

    for (size_t i = 0; i < SwapChainManager::swapchainImageViews.size(); i++) {

        VkImageView attachments[] = {
            SwapChainManager::swapchainImageViews[i],
            mDepthResources.depthImageView,
            mAlbedoResources.colorImageView,
            mNormalResources.colorImageView
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

std::vector<VkImageView> LitRenderPass::getRenderPassImageViews()
{
    std::vector<VkImageView> result = { mDepthResources.depthImageView, mAlbedoResources.colorImageView, mNormalResources.colorImageView };
    return result;
}
