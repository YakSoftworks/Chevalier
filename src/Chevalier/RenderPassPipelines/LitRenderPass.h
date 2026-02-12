#pragma once

#include "RenderPassManager.h"

#include "Objects/Materials/CustomMaterials/LightingShader.h"

class ChevalierMaterial;

class LitRenderPass : public RenderPassManager
{

	// Setup for lights without shadows

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

	ChevalierMaterial* lightingMaterial;

	virtual std::vector<VkImageView> getRenderPassImageViews() override;


};

