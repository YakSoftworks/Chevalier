#include "LitRenderPass.h"

#include "Objects/Materials/ChevalierMaterial.h"


VkRenderPass LitRenderPass::GetRenderPassRef()
{

    if (!mRenderPass) {


        // Attachments:
            // 0 - Swap Chain Image
            // 1 - Depth Image
            // 2 - Albedo Image
            // 3 - Normal Image



        // Geometry Pass Attachments:

        VkAttachmentDescription finalColorAttachment{};

        finalColorAttachment.format = SwapChainManager::getSwapchainImageFormat();
        finalColorAttachment.samples = MSAAResources::getMSAASampleCount();

        finalColorAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        finalColorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        finalColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
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
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;




        VkAttachmentDescription albedoAttachment{};

        albedoAttachment.format = SwapChainManager::getSwapchainImageFormat();
        albedoAttachment.samples = MSAAResources::getMSAASampleCount();

        albedoAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        albedoAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

        albedoAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        albedoAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

        albedoAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        albedoAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        VkAttachmentDescription normalAttachment{};

        normalAttachment.format = VK_FORMAT_A2B10G10R10_UNORM_PACK32;
        normalAttachment.samples = MSAAResources::getMSAASampleCount();

        normalAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        normalAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

        normalAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        normalAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

        normalAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        normalAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;


        VkAttachmentReference geometryDepthAttachmentRef{};
        geometryDepthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        geometryDepthAttachmentRef.attachment = 1;

        VkAttachmentReference geometryAlbedoAttachmentRef{};
        geometryAlbedoAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        geometryAlbedoAttachmentRef.attachment = 2;

        VkAttachmentReference geometryNormalAttachmentRef{};
        geometryNormalAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        geometryNormalAttachmentRef.attachment = 3;


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





        // Create Dependencies

        VkSubpassDependency subpassDependencies[4]{};

        subpassDependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
        subpassDependencies[0].dstSubpass = 0;
        subpassDependencies[0].srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;;
        subpassDependencies[0].dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;;
        subpassDependencies[0].srcAccessMask = 0;
        subpassDependencies[0].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        subpassDependencies[0].dependencyFlags = 0;

        subpassDependencies[1].srcSubpass = VK_SUBPASS_EXTERNAL;
        subpassDependencies[1].dstSubpass = 0;
        subpassDependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependencies[1].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependencies[1].srcAccessMask = 0;
        subpassDependencies[1].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        subpassDependencies[1].dependencyFlags = 0;

        // Manages the writes to the color attachments and gets them to input attachment
        subpassDependencies[2].srcSubpass = 0;
        subpassDependencies[2].dstSubpass = 1;
        subpassDependencies[2].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependencies[2].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        subpassDependencies[2].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        subpassDependencies[2].dstAccessMask = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
        subpassDependencies[2].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

        subpassDependencies[3].srcSubpass = 1;
        subpassDependencies[3].dstSubpass = VK_SUBPASS_EXTERNAL;
        subpassDependencies[3].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependencies[3].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        subpassDependencies[3].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        subpassDependencies[3].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        subpassDependencies[3].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;


        VkAttachmentDescription renderPassAttachments[] = { finalColorAttachment, depthAttachment, albedoAttachment, normalAttachment };
        
        VkSubpassDescription renderPassSubpasses[] = { geometryPassDescription, lightingPassDescription };

        VkRenderPassCreateInfo createInfo{};
        createInfo.attachmentCount = 4;
        createInfo.pAttachments = renderPassAttachments;

        createInfo.dependencyCount = 4;
        createInfo.pDependencies = subpassDependencies;

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

    //// VK_SUBPASS_CONTENTS_INLINE - Says next subpass' commands are also in this buffer
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

    mDepthResources.CreateDepthResources(CHEVALIER_WINDOW_WIDTH_DEFAULT, CHEVALIER_WINDOW_HEIGHT_DEFAULT);
    mAlbedoResources.CreateColorResources(CHEVALIER_WINDOW_WIDTH_DEFAULT, CHEVALIER_WINDOW_HEIGHT_DEFAULT);
    mNormalResources.CreateColorResources(CHEVALIER_WINDOW_WIDTH_DEFAULT, CHEVALIER_WINDOW_HEIGHT_DEFAULT);
    


}

void LitRenderPass::getRenderPassImageViews(std::vector<VkImageView>& imageViewResources )
{
    imageViewResources.push_back(mDepthResources.depthImageView);
    imageViewResources.push_back(mAlbedoResources.colorImageView);
    imageViewResources.push_back(mNormalResources.colorImageView);
}
