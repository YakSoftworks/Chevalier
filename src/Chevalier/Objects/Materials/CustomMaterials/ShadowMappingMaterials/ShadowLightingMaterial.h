//
// Created by Jack on 3/2/26.
//
#pragma once

#include "ChevalierMaterial.h"

struct ShadowLightDescriptor : public MaterialDescriptorBase {

    // Create Descriptor Set Layout
    virtual void CreateDescriptorSetLayout() override;

    // Create Descriptor Pool
    virtual void CreateDescriptorPool() override;

    // Create Descriptor Sets
    virtual void CreateDescriptorSets() override;

    VkSampler depthImageSampler;

};


class ShadowLightingMaterial : public ChevalierMaterial {

    ShadowLightDescriptor materialInputAttachmentDescriptorSet;



    virtual void createPipelineLayout() override;

public:

    virtual void BindMaterial(VkCommandBuffer* buffer, uint32_t currentFrame) override;


};

