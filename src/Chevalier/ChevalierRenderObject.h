#pragma once
#include "WyrmCore.h"

struct RenderObjectCreateInfo {

	std::string modelFilepath;
	glm::mat4 initialTransform;

};

//Data Objects for the Renderer
class WyrmRenderObject
{
	static uint16_t count;
public:

	//Create a basic render object - no assigned model
	WyrmRenderObject();

	//Create a basic render object with a path to a model
	WyrmRenderObject(const std::string& modelFilepath);

	//Create a render object with the create info
	WyrmRenderObject(RenderObjectCreateInfo* createInfo);

	//Destuctor
	~WyrmRenderObject();

	//Initalizing
	void InitObject();

	virtual void OnRegisteredToRenderer();
	virtual void OnUnregisteredToRenderer();

	//Draws
	void RecordObjectDraw(VkCommandBuffer commandBuffer);

	glm::mat4 getModelMatrix() { return objectTransform; }

protected:

	//Setup
	void LoadObject();

	//Returns true if there is a custom vertex data to use
	//This should also populate the vertex and index data
	virtual bool customLoad();

	void createVertexBuffer();
	void createIndexBuffer();

	

	//Cleanup

	void cleanupRenderObject();

protected:

	std::string modelFilepath;

	std::vector<Vertex> verts;
	std::unordered_map<Vertex, uint32_t> uniqueVertices;
	std::vector<uint32_t> indices;

	VkBuffer mVertexBuffer;
	VkDeviceMemory mVertexBufferMemory;

	VkBuffer mIndexBuffer;
	VkDeviceMemory mIndexBufferMemory;

	//Transform
	glm::mat4 objectTransform = glm::identity<glm::mat4>();



};

