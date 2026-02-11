#pragma once

#include "RenderPassManager.h"

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


	ColorResources mAlbedoResources;
	ColorResources mNormalResources;
	DepthResources mDepthResources;

	ChevalierMaterial* lightingMaterial;

};

