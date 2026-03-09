#pragma once
#include "../ChevalierMaterial.h"

struct LightingShaderDescriptor : public MaterialDescriptorBase {

	// Create Descriptor Set Layout
	virtual void CreateDescriptorSetLayout() override;

	// Create Descriptor Pool
	virtual void CreateDescriptorPool() override;

	// Create Descriptor Sets
	virtual void CreateDescriptorSets() override;

};

class LightingShader : public ChevalierMaterial
{

	LightingShaderDescriptor materialInputAttachmentDescriptorSet;

	virtual void createPipelineLayout() override;

public:

	virtual void BindMaterial(VkCommandBuffer* buffer, uint32_t currentFrame) override;
	

};

