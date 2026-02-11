#include "WireframeRenderPass.h"

void WireframeRenderPass::RecordRenderPass(VkCommandBuffer buffer, uint32_t imageIndex)
{
	// Do a single geometry pass



}

VkRenderPass WireframeRenderPass::GetRenderPassRef()
{
    if (!mRenderPass) {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = SwapChainManager::getSwapchainImageFormat();
        colorAttachment.samples = MSAAResources::getMSAASampleCount();
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
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


        VkAttachmentReference colorAttachmentResolveRef{};
        colorAttachmentResolveRef.attachment = 2;
        colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;



        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = DepthResources::findDepthFormat();
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.samples = MSAAResources::getMSAASampleCount();
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentRef{};
        depthAttachmentRef.attachment = 1;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        std::array attachments = { colorAttachment, depthAttachment, colorAttachmentResolve };


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
        geometryDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
        geometryDependency.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        geometryDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        geometryDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;



        VkSubpassDescription geometrySubpass{};
        geometrySubpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        geometrySubpass.colorAttachmentCount = 1;
        geometrySubpass.pColorAttachments = &colorAttachmentRef;
        geometrySubpass.pDepthStencilAttachment = &depthAttachmentRef;
        geometrySubpass.pResolveAttachments = &colorAttachmentResolveRef;

        VkSubpassDescription lightingSubpass{};
        lightingSubpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        lightingSubpass.colorAttachmentCount = 1;
        lightingSubpass.pColorAttachments = &colorAttachmentRef;
        lightingSubpass.pDepthStencilAttachment = &depthAttachmentRef;
        lightingSubpass.pResolveAttachments = &colorAttachmentResolveRef;


        std::array<const VkSubpassDescription, 2> subpasses = { geometrySubpass, lightingSubpass };
        std::array<const VkSubpassDependency, 2> subpassDependencies = { geometryDependency, lightingDependency };


        VkRenderPassCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

        createInfo.dependencyCount = static_cast<uint32_t>(subpasses.size());
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
