#pragma once
#include "Core.h"
#include "Component.h"
#include "ChevalierInterfaces.h"

#include "Objects/BasicShapes.h"



class MeshComponent : public SceneComponent, public ChevalierRenderObjectInterface {


    //ChevalierRenderObjectInterface Implementations
public:
    
    	// Function to bind object data to the given command buffer
	virtual void DrawObject(VkCommandBuffer buffer, ObjectShaderData* objectDataArray) override;

	// Returns a bool of whether or not an object is drawable
	virtual bool IsObjectDrawable() override;

	// Gets the resource to get information of the object's material
	virtual ChevalierMaterialInterface* GetObjectMaterial() override;

private:



    //TODO: References to mesh and material
        


};

class CubeComponent : public SceneComponent, public ChevalierRenderObjectInterface {


	//ChevalierRenderObjectInterface Implementations
public:

	// Function to bind object data to the given command buffer
	virtual void DrawObject(VkCommandBuffer buffer, ObjectShaderData* objectDataArray) override;

	// Returns a bool of whether or not an object is drawable
	virtual bool IsObjectDrawable() override { return true; }

	// Gets the resource to get information of the object's material
	virtual ChevalierMaterialInterface* GetObjectMaterial() override { return pMaterial; }

public:

	//References to mesh and material
	ChevalierMaterialInterface* pMaterial;




	//DEBUG:

public:

	//Setup
	void LoadObject();

protected:

	void createVertexBuffer();
	void createIndexBuffer();


	// Here, we own what it means to be a cube
	std::vector<Vertex> verts = SHAPE_CUBE_VERTS;
	std::vector<uint32_t> indices = SHAPE_CUBE_INDICES;

	VkBuffer mVertexBuffer;
	VkDeviceMemory mVertexBufferMemory;

	VkBuffer mIndexBuffer;
	VkDeviceMemory mIndexBufferMemory;


	//Transform
	glm::mat4 objectTransform = glm::identity<glm::mat4>();

};