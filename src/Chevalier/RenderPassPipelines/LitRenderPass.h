#pragma once

#include "RenderPassManager.h"

class LitRenderPass : public RenderPassManager
{

	// Setup for lights without shadows

	virtual VkRenderPass GetRenderPassRef() override;

};

