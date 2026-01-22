#pragma once

#include "RenderPassManager.h"

class UnlitRenderPass : public RenderPassManager
{

	// We only need to create a custom render-pass
	virtual VkRenderPass GetRenderPassRef() override;

};

