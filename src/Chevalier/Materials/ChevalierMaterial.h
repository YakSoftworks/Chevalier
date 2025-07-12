#pragma once
#include "ChevalierCore.h"

namespace PipelineInitializers {
	VkPipelineShaderStageCreateInfo pipeline_shader_stage_create_info(VkShaderStageFlagBits stage, VkShaderModule shaderModule);
	VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info();
	VkPipelineInputAssemblyStateCreateInfo input_assembly_create_info(VkPrimitiveTopology topology);
	VkPipelineRasterizationStateCreateInfo rasterization_state_create_info(VkPolygonMode polygonMode);
	VkPipelineMultisampleStateCreateInfo multisampling_state_create_info();
	VkPipelineColorBlendAttachmentState color_blend_attachment_state();

	VkPipelineLayoutCreateInfo pipeline_layout_create_info();

}



struct PipelineInitializer {

	std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
	VkPipelineVertexInputStateCreateInfo vertexInputInfo;
	VkPipelineInputAssemblyStateCreateInfo inputAssembly;
	VkViewport viewport;
	VkRect2D scissor;
	VkPipelineRasterizationStateCreateInfo rasterizer;
	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	VkPipelineMultisampleStateCreateInfo multisampling;
	VkPipelineLayout pipelineLayout;


	

	

};

struct MaterialCreateInfo {

	std::string vertexShaderPath;
	std::string fragmentShaderPath;


};

//Exists for multiple meshes
class ChevalierMaterial
{

public:

	ChevalierMaterial();

	//Pipelines

	VkPipelineLayout materialPipelineLayout;
	VkPipeline materialPipeline;

	void bindMaterialProperties(VkCommandBuffer commandBuffer);

	void cleanupMaterial();


	~ChevalierMaterial();

protected:
	void create_pipeline(VkDevice device, VkRenderPass renderPass);

	static VkShaderModule createShaderModule(const std::vector<char>& code);

};

