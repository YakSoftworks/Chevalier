#pragma once

#include "RenderPassManager.h"

class ChevalierMaterial;


class ShadowMappingPass : public RenderPassManager
{

    // Attachments:
        // 0 - Swap Chain Image
        // 1 - Depth Image
        // 2 - Albedo Image
        // 3 - Normal Image
        // 4 - Light Depth Map


	virtual VkRenderPass GetRenderPassRef() override;

	virtual void RecordRenderPass(VkCommandBuffer buffer, uint32_t imageIndex) override;

	virtual void InitializeRP() override;


protected:

	virtual void CreateFrameBuffers() override;

	virtual void InitRenderPipelineResources() override;

public:
	ColorResources mAlbedoResources;
	NormalResources mNormalResources;
	DepthResources mDepthResources;
    DepthResources mShadowDepthResources;

	ChevalierMaterial* mShadowMapMaterial;
	ChevalierMaterial* mApplyShadowMaterial;

	virtual void getRenderPassImageViews(std::vector<VkImageView>& imageViewResources) override;


};