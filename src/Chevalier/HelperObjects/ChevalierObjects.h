#pragma once

#include "ChevalierGraphicsCore.h"

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

struct DepthResources {

protected:

	// Contains objects for depth buffering
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;

public:

	VkImageView depthImageView;

public:

	void CreateDepthResources(uint32_t width, uint32_t height);
	void cleanup();

	static VkFormat findDepthFormat();

};

struct ColorResources {

	// Contains objects for color buffering
	VkImage colorImage;
	VkDeviceMemory colorImageMemory;
	VkImageView colorImageView;

public:

	void CreateColorResources(uint32_t width, uint32_t height);
	void cleanup();

};

struct MSAAResources {
	
protected:

	static VkSampleCountFlagBits msaaSample;
	
public:

	friend struct VulkanPhysicalDevice;

	static VkSampleCountFlagBits getMSAASampleCount() { return msaaSample; }

};

struct SyncObjects {

	//Vectors containing an object for each frame in flight
	static std::vector<VkSemaphore> imageAvailableSemaphore;
	static std::vector<VkSemaphore> renderFinishedSemaphore;
	static std::vector<VkFence> inFlightFences;
	static uint32_t currentFrameIndex;


public:

	static void createSyncObjects();

};

struct ChevFramebuffer {

	static void InitFramebuffers(VkImageView colorImageView, VkImageView depthImageView, VkRenderPass renderPass);

    static void Cleanup(VkFramebuffer framebuffer);

};

//Vertex Data
struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec3 normal;
    glm::vec2 texCoord;


    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, normal);

        attributeDescriptions[3].binding = 0;
        attributeDescriptions[3].location = 3;
        attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[3].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }

    bool operator==(const Vertex& other) const {
        return pos == other.pos && color == other.color && texCoord == other.texCoord && normal == other.normal;
    }


};

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^
                (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
                (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}

struct FileReader {

    static std::vector<char> readFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error("failed to open file!");
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }
};

struct ChevalierSubpassInfo {

    ChevalierSubpassInfo();

    VkSubpassDependency subpassDependency;
    VkSubpassDescription subpass;


};

struct ChevalierRenderPassInfo {


};


struct ChevalierRenderPass {

public:

    virtual void CreateRenderPass() {};

protected:




};