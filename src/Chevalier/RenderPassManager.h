#pragma once

#include "ChevalierGraphicsCore.h"



class RenderPassManager
{
	// Describes how to make the render pass including:
	// Subpasses,
	// Material Parameters
	// 
	// 
	// Public Getters

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

	virtual void RecordRenderPass(VkCommandBuffer buffer) 
	{
		// Default - Do nothing
	}

};

