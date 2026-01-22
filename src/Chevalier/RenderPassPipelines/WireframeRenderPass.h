#pragma once

#include "RenderPassManager.h"

class WireframeRenderPass : public RenderPassManager
{

	virtual void GetPassRasterizationStateInfo(VkPipelineRasterizationStateCreateInfo& rasterizer) override{

		// Default rasterizer is for unlit
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_LINE;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_NONE;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;

	}

	virtual void RecordRenderPass(VkCommandBuffer buffer) override;

	virtual VkRenderPass GetRenderPassRef() override;

};

