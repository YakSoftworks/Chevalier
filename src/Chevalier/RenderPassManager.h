#pragma once

#include "ChevalierGraphicsCore.h"

#include "Objects/Lighting/\LightingManager.h"
#include "Objects/GeometryManager.h"


class RenderPassManager
{
	// Describes how to make the render pass including:
	// Subpasses,
	// Material Parameters
	// 
	// 
	// Public Getters

protected:

	static RenderPassManager* sRenderPassManager;

public:

	static RenderPassManager* getRenderPassManager() { return sRenderPassManager; }

	virtual void getRenderPassImageViews(std::vector<VkImageView>& imageViewResources) { /* Do Nothing */ }

protected:

	VkRenderPass mRenderPass;

public:

	virtual VkShaderModule GetVertShaderOverride() {
		// Default return invalid reference
		return 0;
	}

	virtual VkShaderModule GetFragShaderOverride() {
		// Default return invalid reference
		return 0;
	}

	virtual void GetPassRasterizationStateInfo(VkPipelineRasterizationStateCreateInfo& rasterizer) {

		// Default rasterizer is for unlit

		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;

	}


	virtual VkRenderPass GetRenderPassRef() {
		// Return invalid
		return 0;
	}


	virtual void InitializeRP() {}

	virtual void RecordRenderPass(VkCommandBuffer buffer, uint32_t imageIndex)
	{
		// Default - Do nothing
	}

	void InitFramebuffer() { CreateFrameBuffers(); }

protected:

	std::array<VkFramebuffer, CHEVALIER_MAX_FRAMES_IN_FLIGHT> mFramebuffers;

	virtual void CreateFrameBuffers() {};


	virtual void InitRenderPipelineResources() {};

	

};

