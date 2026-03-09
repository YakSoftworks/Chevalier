#include "ShadowMappingPass.h"

#include "Objects/Materials/ChevalierMaterial.h"
#include "LightingManager.h"

#include "CustomMaterials/ShadowMappingMaterials/ShadowLightingMaterial.h"
#include "CustomMaterials/ShadowMappingMaterials/ShadowGeometryMaterial.h"

VkRenderPass ShadowMappingPass::GetRenderPassRef()
{

    if (!mRenderPass) {


        // Attachments:
            // 0 - Swap Chain Image
            // 1 - Depth Image
            // 2 - Albedo Image
            // 3 - Normal Image
            // 4 - Light Depth Map



        // Describe all Attachments:

        // Attachment 0: Swap Chain Image
        VkAttachmentDescription finalColorAttachment{};
        finalColorAttachment.format = SwapChainManager::getSwapchainImageFormat();
        finalColorAttachment.samples = MSAAResources::getMSAASampleCount();
        finalColorAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        finalColorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        finalColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        finalColorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        finalColorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        finalColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;

        // Attachment 1: Standard Depth Image
        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = mDepthResources.findDepthFormat();
        depthAttachment.samples = MSAAResources::getMSAASampleCount();
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;



        // Attachment 2: Albedo Attachment
        VkAttachmentDescription albedoAttachment{};
        albedoAttachment.format = SwapChainManager::getSwapchainImageFormat();
        albedoAttachment.samples = MSAAResources::getMSAASampleCount();
        albedoAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        albedoAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        albedoAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        albedoAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        albedoAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        albedoAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        // Attachment 3: Normal Attachment
        VkAttachmentDescription normalAttachment{};
        normalAttachment.format = VK_FORMAT_A2B10G10R10_UNORM_PACK32;
        normalAttachment.samples = MSAAResources::getMSAASampleCount();
        normalAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        normalAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        normalAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        normalAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        normalAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        normalAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;


        // Attachment 4: Shadow Depth Attachment
        VkAttachmentDescription shadowDepthAttachment{};
        depthAttachment.format = mShadowDepthResources.findDepthFormat();
        depthAttachment.samples = MSAAResources::getMSAASampleCount();
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        VkAttachmentDescription attachments[] = {
            finalColorAttachment,
            depthAttachment,
            albedoAttachment,
            normalAttachment,
            shadowDepthAttachment
        };


        VkAttachmentReference shadowDepthResourceReference{};
        shadowDepthResourceReference.attachment = 4;
        shadowDepthResourceReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkSubpassDescription defaultShadowDepthSubpass{};
        defaultShadowDepthSubpass.colorAttachmentCount=0;
        defaultShadowDepthSubpass.inputAttachmentCount=0;
        defaultShadowDepthSubpass.preserveAttachmentCount=0;
        defaultShadowDepthSubpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        defaultShadowDepthSubpass.pDepthStencilAttachment = &shadowDepthResourceReference;


        VkAttachmentReference finalColourAttachmentRef{};
        finalColourAttachmentRef.attachment=0;
        finalColourAttachmentRef.layout=VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentReference lightApplicationPassRefs[4];
        lightApplicationPassRefs[0].attachment = 1;
        lightApplicationPassRefs[0].layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        lightApplicationPassRefs[1].attachment = 2;
        lightApplicationPassRefs[1].layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        lightApplicationPassRefs[2].attachment = 3;
        lightApplicationPassRefs[2].layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        lightApplicationPassRefs[3].attachment = 4;
        lightApplicationPassRefs[3].layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;


        VkSubpassDescription defaultLightApplicationSubpass{};
        defaultShadowDepthSubpass.colorAttachmentCount = 1;
        defaultShadowDepthSubpass.pColorAttachments = &finalColourAttachmentRef;

        defaultShadowDepthSubpass.inputAttachmentCount = 4;
        defaultShadowDepthSubpass.pInputAttachments = lightApplicationPassRefs;

        defaultShadowDepthSubpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;


        VkSubpassDependency defaultShadowMappingDependency{};
        defaultShadowMappingDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        defaultShadowMappingDependency.dstSubpass = 0;
        defaultShadowMappingDependency.srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;;
        defaultShadowMappingDependency.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;;
        defaultShadowMappingDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        defaultShadowMappingDependency.dstAccessMask = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
        defaultShadowMappingDependency.dependencyFlags = 0;

        VkSubpassDependency  defaultApplyShadowDependency{};
        defaultApplyShadowDependency.srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
        defaultApplyShadowDependency.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;;
        defaultApplyShadowDependency.srcAccessMask = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
        defaultApplyShadowDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        defaultApplyShadowDependency.dependencyFlags = 0;



        // For each light:
        // Build Subpass Requirements
        const std::vector<LightComponent*>& lightComponents = LightingManager::GetLightingManager()->GetLightComponents();

        uint32_t currentSubpassID = 0;

        std::vector<VkSubpassDescription> subpassDescriptions;
        subpassDescriptions.resize(lightComponents.size()); // TODO: Get lighting count from LightingManager

        std::vector<VkSubpassDependency> subpassDependencies;
        subpassDependencies.resize(lightComponents.size());

        for(LightComponent* LightSource : lightComponents){

            VkSubpassDependency tempDependency{};

            // Build subpass Requirements based on light type
            if(LightSource->mLightInfo.lightType == 0){
                // Point Light

                // Up
                subpassDescriptions.push_back(defaultShadowDepthSubpass);
                subpassDescriptions.push_back(defaultLightApplicationSubpass);

                // Dependencies
                tempDependency = defaultShadowMappingDependency;
                tempDependency.srcSubpass = currentSubpassID;
                tempDependency.dstSubpass = currentSubpassID+1;
                subpassDependencies.push_back(tempDependency);

                tempDependency = defaultApplyShadowDependency;
                tempDependency.srcSubpass = currentSubpassID+1;
                tempDependency.dstSubpass = currentSubpassID+2;
                subpassDependencies.push_back(tempDependency);

                currentSubpassID+=2;

                // Down
                subpassDescriptions.push_back(defaultShadowDepthSubpass);
                subpassDescriptions.push_back(defaultLightApplicationSubpass);

                // Dependencies
                tempDependency = defaultShadowMappingDependency;
                tempDependency.srcSubpass = currentSubpassID;
                tempDependency.dstSubpass = currentSubpassID+1;
                subpassDependencies.push_back(tempDependency);

                tempDependency = defaultApplyShadowDependency;
                tempDependency.srcSubpass = currentSubpassID+1;
                tempDependency.dstSubpass = currentSubpassID+2;
                subpassDependencies.push_back(tempDependency);

                currentSubpassID+=2;

                // Left
                subpassDescriptions.push_back(defaultShadowDepthSubpass);
                subpassDescriptions.push_back(defaultLightApplicationSubpass);

                // Dependencies
                tempDependency = defaultShadowMappingDependency;
                tempDependency.srcSubpass = currentSubpassID;
                tempDependency.dstSubpass = currentSubpassID+1;
                subpassDependencies.push_back(tempDependency);

                tempDependency = defaultApplyShadowDependency;
                tempDependency.srcSubpass = currentSubpassID+1;
                tempDependency.dstSubpass = currentSubpassID+2;
                subpassDependencies.push_back(tempDependency);

                currentSubpassID+=2;

                // Right
                subpassDescriptions.push_back(defaultShadowDepthSubpass);
                subpassDescriptions.push_back(defaultLightApplicationSubpass);

                // Dependencies
                tempDependency = defaultShadowMappingDependency;
                tempDependency.srcSubpass = currentSubpassID;
                tempDependency.dstSubpass = currentSubpassID+1;
                subpassDependencies.push_back(tempDependency);

                tempDependency = defaultApplyShadowDependency;
                tempDependency.srcSubpass = currentSubpassID+1;
                tempDependency.dstSubpass = currentSubpassID+2;
                subpassDependencies.push_back(tempDependency);

                currentSubpassID+=2;

                // Forwards
                subpassDescriptions.push_back(defaultShadowDepthSubpass);
                subpassDescriptions.push_back(defaultLightApplicationSubpass);

                // Dependencies
                tempDependency = defaultShadowMappingDependency;
                tempDependency.srcSubpass = currentSubpassID;
                tempDependency.dstSubpass = currentSubpassID+1;
                subpassDependencies.push_back(tempDependency);

                tempDependency = defaultApplyShadowDependency;
                tempDependency.srcSubpass = currentSubpassID+1;
                tempDependency.dstSubpass = currentSubpassID+2;
                subpassDependencies.push_back(tempDependency);

                currentSubpassID+=2;

                // Backwards
                subpassDescriptions.push_back(defaultShadowDepthSubpass);
                subpassDescriptions.push_back(defaultLightApplicationSubpass);

                // Dependencies
                tempDependency = defaultShadowMappingDependency;
                tempDependency.srcSubpass = currentSubpassID;
                tempDependency.dstSubpass = currentSubpassID+1;
                subpassDependencies.push_back(tempDependency);

                tempDependency = defaultApplyShadowDependency;
                tempDependency.srcSubpass = currentSubpassID+1;
                tempDependency.dstSubpass = currentSubpassID+2;
                subpassDependencies.push_back(tempDependency);

                currentSubpassID+=2;


            } else if (LightSource->mLightInfo.lightType == 1) {
                // Spot Light

                subpassDescriptions.push_back(defaultShadowDepthSubpass);
                subpassDescriptions.push_back(defaultLightApplicationSubpass);

                // Dependencies
                tempDependency = defaultShadowMappingDependency;
                tempDependency.srcSubpass = currentSubpassID;
                tempDependency.dstSubpass = currentSubpassID+1;
                subpassDependencies.push_back(tempDependency);

                tempDependency = defaultApplyShadowDependency;
                tempDependency.srcSubpass = currentSubpassID+1;
                tempDependency.dstSubpass = currentSubpassID+2;
                subpassDependencies.push_back(tempDependency);

                currentSubpassID+=2;


            } else if (LightSource->mLightInfo.lightType == 2) {
                // Directional Light

                subpassDescriptions.push_back(defaultShadowDepthSubpass);
                subpassDescriptions.push_back(defaultLightApplicationSubpass);

                // Dependencies
                tempDependency = defaultShadowMappingDependency;
                tempDependency.srcSubpass = currentSubpassID;
                tempDependency.dstSubpass = currentSubpassID+1;
                subpassDependencies.push_back(tempDependency);

                tempDependency = defaultApplyShadowDependency;
                tempDependency.srcSubpass = currentSubpassID+1;
                tempDependency.dstSubpass = currentSubpassID+2;
                subpassDependencies.push_back(tempDependency);

                currentSubpassID+=2;


            }
        }


        

        VkRenderPassCreateInfo createInfo{};
        createInfo.attachmentCount = 5;
        createInfo.pAttachments = attachments;

        createInfo.dependencyCount = subpassDependencies.size();
        createInfo.pDependencies = subpassDependencies.data();

        createInfo.subpassCount = static_cast<uint32_t>(subpassDescriptions.size());
        createInfo.pSubpasses = subpassDescriptions.data();

        createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;


        VkResult result = vkCreateRenderPass(VulkanLogicalDevice::getLogicalDevice(), &createInfo, nullptr, &mRenderPass);
        if(result != VK_SUCCESS) {

            CHEV_MESSAGE_ERROR("Failed to create Render Pass!")

        }

        sRenderPassManager = this;


    }

    return mRenderPass;
}

void ShadowMappingPass::RecordRenderPass(VkCommandBuffer buffer, uint32_t imageIndex)
{

    // Variables

    glm::mat4 lightViewTransform;
    glm::mat4 inverseLightViewTransform;



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

    // Draw Geometry to Albedo and normal textures
    GeometryManager* sGeometryManager = GeometryManager::getGeometryManager();
    sGeometryManager->PerformGeometryPass(buffer, imageIndex);



    vkCmdNextSubpass(buffer, VK_SUBPASS_CONTENTS_INLINE);

    const std::vector<LightComponent*> lightSourceComponents = LightingManager::GetLightingManager()->GetLightComponents();

    // For each light source:
    for(LightComponent* lightSource : lightSourceComponents) {

        bool bIsLastLightSource = lightSourceComponents[lightSourceComponents.size()-1] == lightSource;

        // Build subpass Requirements based on light type
        if(lightSource->mLightInfo.lightType == 0){
            // Point Light

            // Template for light transformation
            //lightViewTransform = glm::rotate(lightSource->mLightInfo.lightTransform, 90.f, glm::vec3(1.f, 0.f, 0.f));


            // Forwards
            lightViewTransform = lightSource->mLightInfo.lightTransform;

            mShadowMapMaterial->BindMaterial(&buffer, imageIndex);

            vkCmdPushConstants(buffer, mShadowMapMaterial->getMaterialLayout(), VK_SHADER_STAGE_ALL_GRAPHICS, 0, 64, &lightViewTransform);

            sGeometryManager->PerformGeometryPass(buffer, imageIndex, false);
            vkCmdNextSubpass(buffer, VK_SUBPASS_CONTENTS_INLINE);



            // Apply Pass

            inverseLightViewTransform = glm::inverse(lightViewTransform);

            mApplyShadowMaterial->BindMaterial(&buffer, imageIndex);

            vkCmdPushConstants(buffer, mApplyShadowMaterial->getMaterialLayout(), VK_SHADER_STAGE_ALL_GRAPHICS, 0, 64, &inverseLightViewTransform);

            // Draw full screen shape
            vkCmdDraw(buffer, 3, 1, 0, 0);
            vkCmdNextSubpass(buffer, VK_SUBPASS_CONTENTS_INLINE);


            // Backwards
            lightViewTransform = glm::rotate(lightSource->mLightInfo.lightTransform, 90.f, glm::vec3(1.f, 0.f, 0.f));

            mShadowMapMaterial->BindMaterial(&buffer, imageIndex);

            vkCmdPushConstants(buffer, mShadowMapMaterial->getMaterialLayout(), VK_SHADER_STAGE_ALL_GRAPHICS, 0, 64, &lightViewTransform);

            sGeometryManager->PerformGeometryPass(buffer, imageIndex, false);
            vkCmdNextSubpass(buffer, VK_SUBPASS_CONTENTS_INLINE);


            // Apply Pass

            inverseLightViewTransform = glm::inverse(lightViewTransform);

            mApplyShadowMaterial->BindMaterial(&buffer, imageIndex);

            vkCmdPushConstants(buffer, mApplyShadowMaterial->getMaterialLayout(), VK_SHADER_STAGE_ALL_GRAPHICS, 0, 64, &inverseLightViewTransform);

            // Draw full screen shape
            vkCmdDraw(buffer, 3, 1, 0, 0);
            vkCmdNextSubpass(buffer, VK_SUBPASS_CONTENTS_INLINE);


            // Up
            lightViewTransform = glm::rotate(lightSource->mLightInfo.lightTransform, 180.f, glm::vec3(1.f, 0.f, 0.f));

            mShadowMapMaterial->BindMaterial(&buffer, imageIndex);

            vkCmdPushConstants(buffer, mShadowMapMaterial->getMaterialLayout(), VK_SHADER_STAGE_ALL_GRAPHICS, 0, 64, &lightViewTransform);

            sGeometryManager->PerformGeometryPass(buffer, imageIndex, false);
            vkCmdNextSubpass(buffer, VK_SUBPASS_CONTENTS_INLINE);


            // Apply Pass

            inverseLightViewTransform = glm::inverse(lightViewTransform);

            mApplyShadowMaterial->BindMaterial(&buffer, imageIndex);

            vkCmdPushConstants(buffer, mApplyShadowMaterial->getMaterialLayout(), VK_SHADER_STAGE_ALL_GRAPHICS, 0, 64, &inverseLightViewTransform);

            // Draw full screen shape
            vkCmdDraw(buffer, 3, 1, 0, 0);
            vkCmdNextSubpass(buffer, VK_SUBPASS_CONTENTS_INLINE);


            // Down
            lightViewTransform = glm::rotate(lightSource->mLightInfo.lightTransform, 270.f, glm::vec3(1.f, 0.f, 0.f));

            mShadowMapMaterial->BindMaterial(&buffer, imageIndex);

            vkCmdPushConstants(buffer, mShadowMapMaterial->getMaterialLayout(), VK_SHADER_STAGE_ALL_GRAPHICS, 0, 64, &lightViewTransform);

            sGeometryManager->PerformGeometryPass(buffer, imageIndex, false);
            vkCmdNextSubpass(buffer, VK_SUBPASS_CONTENTS_INLINE);


            // Apply Pass

            inverseLightViewTransform = glm::inverse(lightViewTransform);

            mApplyShadowMaterial->BindMaterial(&buffer, imageIndex);

            vkCmdPushConstants(buffer, mApplyShadowMaterial->getMaterialLayout(), VK_SHADER_STAGE_ALL_GRAPHICS, 0, 64, &inverseLightViewTransform);

            // Draw full screen shape
            vkCmdDraw(buffer, 3, 1, 0, 0);
            vkCmdNextSubpass(buffer, VK_SUBPASS_CONTENTS_INLINE);


            // Left
            lightViewTransform = glm::rotate(lightSource->mLightInfo.lightTransform, 90.f, glm::vec3(0.f, 1.f, 0.f));

            mShadowMapMaterial->BindMaterial(&buffer, imageIndex);

            vkCmdPushConstants(buffer, mShadowMapMaterial->getMaterialLayout(), VK_SHADER_STAGE_ALL_GRAPHICS, 0, 64, &lightViewTransform);

            sGeometryManager->PerformGeometryPass(buffer, imageIndex, false);
            vkCmdNextSubpass(buffer, VK_SUBPASS_CONTENTS_INLINE);

            // Apply Pass

            inverseLightViewTransform = glm::inverse(lightViewTransform);

            mApplyShadowMaterial->BindMaterial(&buffer, imageIndex);

            vkCmdPushConstants(buffer, mApplyShadowMaterial->getMaterialLayout(), VK_SHADER_STAGE_ALL_GRAPHICS, 0, 64, &inverseLightViewTransform);

            // Draw full screen shape
            vkCmdDraw(buffer, 3, 1, 0, 0);
            vkCmdNextSubpass(buffer, VK_SUBPASS_CONTENTS_INLINE);

            // Right
            lightViewTransform = glm::rotate(lightSource->mLightInfo.lightTransform, 270.f, glm::vec3(0.f, 1.f, 0.f));

            mShadowMapMaterial->BindMaterial(&buffer, imageIndex);

            vkCmdPushConstants(buffer, mShadowMapMaterial->getMaterialLayout(), VK_SHADER_STAGE_ALL_GRAPHICS, 0, 64, &lightViewTransform);

            sGeometryManager->PerformGeometryPass(buffer, imageIndex, false);
            vkCmdNextSubpass(buffer, VK_SUBPASS_CONTENTS_INLINE);


            // Apply Pass

            inverseLightViewTransform = glm::inverse(lightViewTransform);

            mApplyShadowMaterial->BindMaterial(&buffer, imageIndex);

            vkCmdPushConstants(buffer, mApplyShadowMaterial->getMaterialLayout(), VK_SHADER_STAGE_ALL_GRAPHICS, 0, 64, &inverseLightViewTransform);

            // Draw full screen shape
            vkCmdDraw(buffer, 3, 1, 0, 0);


            //ASK IF TO NEXT SUBPASS
            if(!bIsLastLightSource){
                vkCmdNextSubpass(buffer, VK_SUBPASS_CONTENTS_INLINE);
            }


        } else if (lightSource->mLightInfo.lightType == 1) {
            // Spot Light

            // Depth Pass
            // Use special material

            lightViewTransform = lightSource->mLightInfo.lightTransform;

            mShadowMapMaterial->BindMaterial(&buffer, imageIndex);

            vkCmdPushConstants(buffer, mShadowMapMaterial->getMaterialLayout(), VK_SHADER_STAGE_ALL_GRAPHICS, 0, 64, &lightViewTransform);

            sGeometryManager->PerformGeometryPass(buffer, imageIndex, false);
            vkCmdNextSubpass(buffer, VK_SUBPASS_CONTENTS_INLINE);


            // Apply Pass

            inverseLightViewTransform = glm::inverse(lightViewTransform);

            mApplyShadowMaterial->BindMaterial(&buffer, imageIndex);

            vkCmdPushConstants(buffer, mApplyShadowMaterial->getMaterialLayout(), VK_SHADER_STAGE_ALL_GRAPHICS, 0, 64, &inverseLightViewTransform);

            // Draw full screen shape
            vkCmdDraw(buffer, 3, 1, 0, 0);

            //ASK IF TO NEXT SUBPASS
            if(!bIsLastLightSource){
                vkCmdNextSubpass(buffer, VK_SUBPASS_CONTENTS_INLINE);
            }


        } else if (lightSource->mLightInfo.lightType == 2) {
            // Directional Light

            // Depth Pass
            // Use special material

            lightViewTransform = lightSource->mLightInfo.lightTransform;

            mShadowMapMaterial->BindMaterial(&buffer, imageIndex);

            vkCmdPushConstants(buffer, mShadowMapMaterial->getMaterialLayout(), VK_SHADER_STAGE_ALL_GRAPHICS, 0, 64, &lightViewTransform);

            sGeometryManager->PerformGeometryPass(buffer, imageIndex, false);
            vkCmdNextSubpass(buffer, VK_SUBPASS_CONTENTS_INLINE);


            // Apply Pass

            inverseLightViewTransform = glm::inverse(lightViewTransform);

            mApplyShadowMaterial->BindMaterial(&buffer, imageIndex);

            vkCmdPushConstants(buffer, mApplyShadowMaterial->getMaterialLayout(), VK_SHADER_STAGE_ALL_GRAPHICS, 0, 64, &inverseLightViewTransform);

            // Draw full screen shape
            vkCmdDraw(buffer, 3, 1, 0, 0);

            //ASK IF TO NEXT SUBPASS
            if(!bIsLastLightSource){
                vkCmdNextSubpass(buffer, VK_SUBPASS_CONTENTS_INLINE);
            }

        }

    }

    // End Render Pass
    vkCmdEndRenderPass(buffer);

}

void ShadowMappingPass::InitializeRP() {
    // Lighting Material

    //TODO: Update paths
    VkShaderModule shadowGeometryVertModule = ChevalierMaterial::createShaderModule(FileReader::readFile("content/shaders/ChevalierII/LightVert.spv"));
    VkShaderModule shadowGeometryFragModule = ChevalierMaterial::createShaderModule(FileReader::readFile("content/shaders/ChevalierII/LightFrag.spv"));

    VkShaderModule shadowLightingVertModule = ChevalierMaterial::createShaderModule(FileReader::readFile("content/shaders/ChevalierII/LightVert.spv"));
    VkShaderModule shadowLightingFragModule = ChevalierMaterial::createShaderModule(FileReader::readFile("content/shaders/ChevalierII/LightFrag.spv"));

    InitRenderPipelineResources();

    mShadowMapMaterial = new ShadowGeometryMaterial();
    mShadowMapMaterial->init_pipeline(this, shadowGeometryVertModule, shadowGeometryFragModule, 0);

    mApplyShadowMaterial = new ShadowLightingMaterial();
    mApplyShadowMaterial->init_pipeline(this, shadowLightingVertModule, shadowLightingFragModule, 1);
}
