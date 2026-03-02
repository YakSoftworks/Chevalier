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
        // 4 - Shadow Depth Map


	virtual VkRenderPass GetRenderPassRef() override;

	virtual void RecordRenderPass(VkCommandBuffer buffer, uint32_t imageIndex) override;

    void InitializeRP() override;


protected:

    void CreateFrameBuffers() override;

    void InitRenderPipelineResources() override;

public:
	ColorResources mAlbedoResources;
	NormalResources mNormalResources;
	DepthResources mDepthResources;
    DepthResources mShadowDepthResources;

	ChevalierMaterial* mShadowMapMaterial;
	ChevalierMaterial* mApplyShadowMaterial;

     void getRenderPassImageViews(std::vector<VkImageView>& imageViewResources) override {
        imageViewResources.resize(4);
        imageViewResources[0] = mDepthResources.depthImageView;
        imageViewResources[1] = mAlbedoResources.colorImageView;
        imageViewResources[2] = mNormalResources.colorImageView;
        imageViewResources[3] = mShadowDepthResources.depthImageView;
    }


};