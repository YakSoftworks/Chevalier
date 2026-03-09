#pragma once
#include "Objects/Materials/ChevalierMaterial.h"

class GeometryMaterial : public ChevalierMaterial
{
	virtual void createPipeline(RenderPassManager* renderPass, VkShaderModule vertShaderModule, VkShaderModule fragShaderModule, uint32_t subpass) override;



};

