#include "ChevalierRenderObject.h"
#include "ChevalierEngineStatics.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

uint16_t ChevalierRenderObject::count = 0;

ChevalierRenderObject::ChevalierRenderObject()
{
    

    if (ChevalierRenderObject::count % 2 == 0) {
        modelFilepath = "content/models/VikingRoom/viking_room.obj";
        objectTransform = glm::translate(objectTransform, glm::vec3(0.0f, 0, 2.0f * ChevalierRenderObject::count));
    }
    else {
        modelFilepath = "content/models/monkey.obj";
        objectTransform = glm::translate(objectTransform, glm::vec3(0.0f, 0, 2.0f * ChevalierRenderObject::count));
    }
    
    count++;
    std::cout << "Object #" << count << std::endl;
}

ChevalierRenderObject::ChevalierRenderObject(const std::string& modelFilepath)
{
}
ChevalierRenderObject::ChevalierRenderObject(RenderObjectCreateInfo* createInfo)

{
}

ChevalierRenderObject::~ChevalierRenderObject()
{
    cleanupRenderObject();
}

void ChevalierRenderObject::InitObject()
{
    //First we must populate the verts data
    LoadObject();

    //Next create our object buffers
    createVertexBuffer();
    createIndexBuffer();

}

void ChevalierRenderObject::OnRegisteredToRenderer()
{
    InitObject();
}

void ChevalierRenderObject::OnUnregisteredToRenderer()
{
    delete this;
}

void ChevalierRenderObject::LoadObject()
{

    //Check if we have a custom render shape
    if (customLoad()) {
        return;
    }

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (modelFilepath == "") {
        modelFilepath = "content/models/VikingRoom/viking_room.obj";
    }


    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelFilepath.c_str())) {
        throw std::runtime_error(warn + err);
    }

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {

            Vertex vertex{};

            vertex.pos = { attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2], };

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.color = { 1.0f, 1.0f, 1.0f };


            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(verts.size());
                verts.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);


        }
    }
}

bool ChevalierRenderObject::customLoad()
{
    //By default, use obj unpacking
    return false;
}

void ChevalierRenderObject::createVertexBuffer()
{
    VkDeviceSize bufferSize = sizeof(verts[0]) * verts.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    ChevalierEngineStatics::createBuffer(
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer,
        stagingBufferMemory);

    void* data;
    vkMapMemory(ChevalierEngineStatics::getLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, verts.data(), (size_t)bufferSize);
    vkUnmapMemory(ChevalierEngineStatics::getLogicalDevice(), stagingBufferMemory);

    ChevalierEngineStatics::createBuffer(
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        mVertexBuffer,
        mVertexBufferMemory
    );

    ChevalierEngineStatics::copyBuffer(stagingBuffer, mVertexBuffer, bufferSize);

    vkDestroyBuffer(ChevalierEngineStatics::getLogicalDevice(), stagingBuffer, nullptr);
    vkFreeMemory(ChevalierEngineStatics::getLogicalDevice(), stagingBufferMemory, nullptr);
}

void ChevalierRenderObject::createIndexBuffer()
{
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    ChevalierEngineStatics::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(ChevalierEngineStatics::getLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(ChevalierEngineStatics::getLogicalDevice(), stagingBufferMemory);

    ChevalierEngineStatics::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mIndexBuffer, mIndexBufferMemory);

    ChevalierEngineStatics::copyBuffer(stagingBuffer, mIndexBuffer, bufferSize);

    vkDestroyBuffer(ChevalierEngineStatics::getLogicalDevice(), stagingBuffer, nullptr);
    vkFreeMemory(ChevalierEngineStatics::getLogicalDevice(), stagingBufferMemory, nullptr);
}


void ChevalierRenderObject::RecordObjectDraw(VkCommandBuffer commandBuffer)
{
    VkBuffer vertexBuffers[] = { mVertexBuffer };
    VkDeviceSize offsets[] = { 0 };

    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(commandBuffer, mIndexBuffer, 0, VK_INDEX_TYPE_UINT32);

    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

}

void ChevalierRenderObject::cleanupRenderObject()
{
    VkDevice device = ChevalierEngineStatics::getLogicalDevice();
    vkDestroyBuffer(device, mIndexBuffer, nullptr);
    vkFreeMemory(device, mIndexBufferMemory, nullptr);

    vkDestroyBuffer(device, mVertexBuffer, nullptr);
    vkFreeMemory(device, mVertexBufferMemory, nullptr);
}
