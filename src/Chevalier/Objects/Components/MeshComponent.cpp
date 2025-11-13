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
    //ObjectDataThisFrame.modelMat = glm::translate(glm::mat4(1.0f), TranslationVector) * glm::rotate(glm::mat4(1.0f), time * glm::radians(15.0f), glm::vec3(0.f, 1.f, 1.f));
        
        //glm::rotate(glm::mat4(1.0f), renderObjectID * time * glm::radians(15.0f), RotationVector);

    ObjectDataThisFrame.modelMat = componentTransform.getTransform();

    objectDataArray[renderObjectID] = ObjectDataThisFrame;

    vkCmdBindVertexBuffers(buffer, 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(buffer, mIndexBuffer, 0, VK_INDEX_TYPE_UINT32);

    vkCmdPushConstants(buffer, pMaterial->getMaterialLayout(), VK_SHADER_STAGE_ALL_GRAPHICS, 0, 4, &renderObjectID);

    vkCmdDrawIndexed(buffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

    //CHEV_MESSAGE_LOG("Drawing Cube");

}

bool MeshComponent::IsObjectDrawable()
{
    return true;
}

ChevalierMaterialInterface* MeshComponent::GetObjectMaterial()
{
    return pMaterial;
}


void CubeComponent::LoadModelData(){

    verts = SHAPE_CUBE_VERTS;
    indices = SHAPE_CUBE_INDICES;

}

void PlaneComponent::LoadModelData(){

    verts = SHAPE_PLANE_VERTS;
    indices = SHAPE_PLANE_INDICES;

}

void CylinderComponent::LoadModelData(){
    GenerateVerts();
    GenerateIndicies();
};

void CylinderComponent::GenerateVerts(){

    verts.clear();
    verts.reserve(2*numDivisions);

    // Find our Angular Jump Distance in Radians
    // 2*pi / n
    double radialJump = glm::two_pi<double>() / numDivisions;

    double currentRadians = 0.f;

    for(uint32_t i = 0; i < numDivisions; i++){

        // Points at h1 and h2

        Vertex p1{};
        Vertex p2{};
        // Position
        p1.pos.x = static_cast<float>(sin(currentRadians)); //static_cast<float>(glm::sin(currentRadians));
        p1.pos.y = static_cast<float>(cos(currentRadians)); //static_cast<float>(glm::cos(currentRadians));
        p1.pos.z = 1.f;

        p2.pos.x = static_cast<float>(glm::sin(currentRadians));
        p2.pos.y = static_cast<float>(glm::cos(currentRadians));
        
        p2.pos.z = -1.f;

        // Tex-Coord
        p1.texCoord.x = static_cast<float>(currentRadians / glm::two_pi<double>());
        p1.texCoord.y = 1.0f;

        p2.texCoord.x = static_cast<float>(currentRadians / glm::two_pi<double>());
        p2.texCoord.y = 0.0f;

        // Colors
        p1.color.x = static_cast<float>(currentRadians / glm::two_pi<double>());
        p1.color.y = 1.0f;

        p2.color.x = static_cast<float>(currentRadians / glm::two_pi<double>());
        p2.color.y = 0.0f;

        //TODO: NORMALS

        //Submit Verts
        verts.push_back(p2);
        verts.push_back(p1);

        //Increment our radians
        currentRadians += radialJump;
    }

}

void CylinderComponent::GenerateIndicies(){

    indices.clear();
    indices.reserve(6*numDivisions + 2*(3*(numDivisions-2)));

    // MUST BE CALLED AFTER GenerateVerts()!
    uint32_t numVerts = static_cast<uint32_t>(verts.size());

    for(uint32_t i = 0; i < numDivisions; i++){ // Panels

        // For Each Division

        //Winding 1
        /*indices.push_back(((2 * i))% numVerts);
        indices.push_back(((2 * i) + 2)% numVerts);
        indices.push_back(((2 * i) + 3)% numVerts);
        indices.push_back(((2 * i) + 3)% numVerts);
        indices.push_back(((2 * i) + 1)% numVerts);
        indices.push_back(((2 * i))% numVerts);*/

        //Winding 2
        indices.push_back(((2 * i) + 2) % numVerts);
        indices.push_back(((2 * i)) % numVerts);
        indices.push_back(((2 * i) + 3) % numVerts);
        
        indices.push_back(((2 * i)) % numVerts);
        indices.push_back(((2 * i) + 1) % numVerts);
        indices.push_back(((2 * i) + 3) % numVerts);
        
        

    }

    // TODO: Top
    // Odds


    // TODO: Bottom
    // Evens
    uint32_t leftIndex = static_cast<uint32_t>(verts.size()-2);
    uint32_t rightIndex = 0;

    //Starting Triangles

    //Red
    indices.push_back(rightIndex);
    indices.push_back(rightIndex + 2);
    indices.push_back(leftIndex);
    

    //Green
    indices.push_back(rightIndex+1);
    indices.push_back(leftIndex+1);
    indices.push_back(rightIndex + 3);

    rightIndex += 2;


    while(leftIndex-rightIndex > 2){

        //Note: 1 - 2 - Increment - 3

        //Red Side
        indices.push_back(rightIndex);
        indices.push_back(rightIndex + 2);
        indices.push_back(leftIndex);

        //Green Side
        indices.push_back(rightIndex + 1);
        indices.push_back(leftIndex + 1);
        indices.push_back(rightIndex + 3);

        //Increment R
        rightIndex += 2;


        //Red
        indices.push_back(leftIndex);
        indices.push_back(rightIndex);
        indices.push_back(leftIndex - 2);
        
        //Green
        indices.push_back(leftIndex+1);
        indices.push_back(leftIndex - 1);
        indices.push_back(rightIndex + 1);

        
        leftIndex -= 2;

    }


}


void SphereComponent::LoadModelData()
{
    GenerateVerts();
    GenerateIndicies();
}

void SphereComponent::GenerateVerts() {

    //UV SPHERE

    //GENERATE FANCY CYLINDERS FOR EACH ROW

    verts.clear();
    verts.reserve(2 * numDivisions * (numDivisions+1));

    // Find our Angular Jump Distance in Radians
    // 2*pi / n
    double radialJump = glm::two_pi<double>() / numDivisions;

    double currentRadians = 0.f;
    double currentHeightRadians = 0.f;

    for(uint32_t height = 0; height < numDivisions+1; height++) {

        for (uint32_t i = 0; i < numDivisions; i++) {

            // Points at h1 and h2

            Vertex p1{};
            // Position
            p1.pos.x = static_cast<float>(sin(currentRadians) * sin(currentHeightRadians)); //static_cast<float>(glm::sin(currentRadians));
            p1.pos.y = static_cast<float>(cos(currentRadians) * sin(currentHeightRadians)); //static_cast<float>(glm::cos(currentRadians));
            p1.pos.z = static_cast<float>(cos(currentHeightRadians));

            // Tex-Coord
            p1.texCoord.x = static_cast<float>(currentRadians / glm::two_pi<double>());
            p1.texCoord.y = static_cast<float>(sin(currentHeightRadians));

            // Colors
            p1.color.x = static_cast<float>(currentRadians / glm::two_pi<double>());
            p1.color.y = static_cast<float>(sin(currentHeightRadians));
            p1.color.z = static_cast<float>(cos(currentHeightRadians));

            //TODO: NORMALS

            //Submit Vert
            verts.push_back(p1);

            //Increment our radians
            currentRadians += radialJump;
        }


        currentHeightRadians += radialJump / 2;
    }

   
}

void SphereComponent::GenerateIndicies() {

    // MUST BE CALLED AFTER GenerateVerts()!

    indices.clear();
    indices.reserve(6 * numDivisions * numDivisions);

    uint32_t numVerts = static_cast<uint32_t>(verts.size());

    for (uint32_t height = 0; height < numDivisions; height++) {

        for (uint32_t i = 0; i < numDivisions; i++) { // Panels

            // For Each Division

            //Winding RH
            /*indices.push_back(((2 * i) + 2) % numVerts);
            indices.push_back(((2 * i)) % numVerts);
            indices.push_back(((2 * i) + 3) % numVerts);

            indices.push_back(((2 * i)) % numVerts);
            indices.push_back(((2 * i) + 1) % numVerts);
            indices.push_back(((2 * i) + 3) % numVerts);*/

            indices.push_back((height * numDivisions) + (i) % numDivisions);
            indices.push_back((height * numDivisions) + (i + 1)%numDivisions);
            indices.push_back(((height + 1) * numDivisions) + (i + 1) % numDivisions);

            indices.push_back(((height + 1) * numDivisions) + (i) % numDivisions);
            indices.push_back((height * numDivisions) + (i) % numDivisions);
            indices.push_back(((height + 1) * numDivisions) + (i + 1) % numDivisions);

        }

    }

}



void NPlaneComponent::LoadModelData() {
    GenerateVerts();
    GenerateIndicies();

}

void NPlaneComponent::GenerateVerts() {
    // Creates an nxn square of vertices


    // Allocate array
    verts.resize(numDivisions*numDivisions);
    verts.clear();


    float intervalSize = 1 / (numDivisions-1);

    // For each row
    for(uint32_t i = 0; i < numDivisions; i++){

        // For each column
        for(uint32_t j = 0; j < numDivisions; j++){

            Vertex newVertex{};

            newVertex.pos.x = i * intervalSize;
            newVertex.pos.z = j * intervalSize;

            newVertex.color.x = i * intervalSize;
            newVertex.color.z = j * intervalSize;

            newVertex.texCoord.x = i * intervalSize;
            newVertex.texCoord.y = j * intervalSize;

            verts.push_back(newVertex);

        }
    }

}

void NPlaneComponent::GenerateIndicies() {

    indices.clear();
    indices.resize((numDivisions-1)*(numDivisions-1)*12);

    for(uint32_t i = 0; i < numDivisions-1; i++){


        for(uint32_t j = 0; j < numDivisions-1; j++){

            //Relevant Verts:

            /*
            
               i,j - - - i+1, j
                 |        |
                 |        |
                 |        |
              i, j+1 - - - i+1, j+1

            */


            //Top Triangles

            indices.push_back( (numDivisions * i) + j);
            indices.push_back( (numDivisions * i) + j+1);
            indices.push_back( (numDivisions * i+1) + j);

            indices.push_back( (numDivisions * i+1) + j);
            indices.push_back( (numDivisions * i+1) + j+1);
            indices.push_back( (numDivisions * i) + j);


            //Bottom Triangles

            indices.push_back( (numDivisions * i) + j+1);
            indices.push_back( (numDivisions * i) + j);
            indices.push_back( (numDivisions * i+1) + j);

            indices.push_back( (numDivisions * i+1) + j+1);
            indices.push_back( (numDivisions * i+1) + j);
            indices.push_back( (numDivisions * i) + j);

        }

    }

}


