#include "MeshComponent.h"

void MeshComponent::LoadMeshComponent(){
    
    // Populate our vertex and index data
    LoadModelData();

    // Create Buffers
    createVertexBuffer();
    createIndexBuffer();
}

void MeshComponent::LoadModelData(){
    /* Trigger Error */
    CHEV_MESSAGE_ERROR("USING INVALID MESH COMPONENT LOAD");
}

void MeshComponent::createVertexBuffer(){
    VkDeviceSize bufferSize = sizeof(verts[0]) * verts.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VulkanBuffer::createBuffer(
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer,
        stagingBufferMemory);

    void* data;
    vkMapMemory(VulkanLogicalDevice::getLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, verts.data(), (size_t)bufferSize);
    vkUnmapMemory(VulkanLogicalDevice::getLogicalDevice(), stagingBufferMemory);

    VulkanBuffer::createBuffer(
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        mVertexBuffer,
        mVertexBufferMemory
    );

    VulkanBuffer::copyBuffer(stagingBuffer, mVertexBuffer, bufferSize);

    vkDestroyBuffer(VulkanLogicalDevice::getLogicalDevice(), stagingBuffer, nullptr);
    vkFreeMemory(VulkanLogicalDevice::getLogicalDevice(), stagingBufferMemory, nullptr);
}

void MeshComponent::createIndexBuffer(){
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VulkanBuffer::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(VulkanLogicalDevice::getLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(VulkanLogicalDevice::getLogicalDevice(), stagingBufferMemory);

    VulkanBuffer::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mIndexBuffer, mIndexBufferMemory);

    VulkanBuffer::copyBuffer(stagingBuffer, mIndexBuffer, bufferSize);

    vkDestroyBuffer(VulkanLogicalDevice::getLogicalDevice(), stagingBuffer, nullptr);
    vkFreeMemory(VulkanLogicalDevice::getLogicalDevice(), stagingBufferMemory, nullptr);
}

void MeshComponent::DrawObject(VkCommandBuffer buffer, ObjectShaderData* objectDataArray)
{
    //Debug
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    //


    VkBuffer vertexBuffers[] = { mVertexBuffer };
    VkDeviceSize offsets[] = { 0 };


    //Update Object Stored Data
    ObjectShaderData ObjectDataThisFrame{};



    //ObjectDataThisFrame.modelMat = componentTransform.getTransform();

    glm::vec3 TranslationVector{ 0.0, 0.0, 0.0 };

    if (renderObjectID < 1) {
        TranslationVector = { 0.0, 1.0, 0.0 };
    }
    else {
        TranslationVector = { 0.0, -1.0, 0.0 };
    }

    //debug Model Mat
    ObjectDataThisFrame.modelMat = glm::translate(glm::mat4(1.0f), TranslationVector);
        
        //glm::rotate(glm::mat4(1.0f), renderObjectID * time * glm::radians(15.0f), RotationVector);

    objectDataArray[renderObjectID] = ObjectDataThisFrame;

    vkCmdBindVertexBuffers(buffer, 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(buffer, mIndexBuffer, 0, VK_INDEX_TYPE_UINT32);

    vkCmdPushConstants(buffer, pMaterial->getMaterialLayout(), VK_SHADER_STAGE_ALL_GRAPHICS, 0, 4, &renderObjectID);

    vkCmdDrawIndexed(buffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

    //CHEV_MESSAGE_LOG("Drawing Cube");

}

void CubeComponent::LoadModelData(){

    verts = SHAPE_CUBE_VERTS;
    indices = SHAPE_CUBE_INDICES;
    
}


void PlaneComponent::LoadModelData(){

    verts = SHAPE_PLANE_VERTS;
    indices = SHAPE_PLANE_INDICES;

}
