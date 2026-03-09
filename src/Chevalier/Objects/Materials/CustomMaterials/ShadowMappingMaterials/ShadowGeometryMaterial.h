//
// Created by Jack on 3/2/26.
//

#pragma once

#include "ChevalierMaterial.h"

struct LightSourcePushConstantStruct{
    glm::mat4 transform;
    uint32_t light_id;

};

class ShadowGeometryMaterial : public ChevalierMaterial {


    void createPipelineLayout() override;

    void createPipeline(RenderPassManager *renderPass, VkShaderModule vertShaderModule, VkShaderModule fragShaderModule, uint32_t subpass) override;


};


