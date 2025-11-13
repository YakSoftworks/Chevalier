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



public:

	void LoadMeshComponent();

	// Material reference
	ChevalierMaterialInterface* pMaterial;

protected:

	// Puts the Vertex and Index Data into the Vectors
	virtual void LoadModelData();

	void createVertexBuffer();
	void createIndexBuffer();

    //TODO: References to mesh and material
	// Here, we own what it means to be our shape
	std::vector<Vertex> verts;
	std::vector<uint32_t> indices;

	VkBuffer mVertexBuffer;
	VkDeviceMemory mVertexBufferMemory;

	VkBuffer mIndexBuffer;
	VkDeviceMemory mIndexBufferMemory;


public:
	//Transform
	ModelTransform componentTransform;
        


};

class CubeComponent : public MeshComponent {

public:

protected:
	virtual void LoadModelData() override;


};

class PlaneComponent : public MeshComponent {


protected:
	virtual void LoadModelData() override;

};

class CylinderComponent : public MeshComponent {

	uint32_t numDivisions = 40;

	virtual void LoadModelData() override;

	//Helpers for Verts And Indicies
private:
	void GenerateVerts();

	void GenerateIndicies();

};

class SphereComponent : public MeshComponent {

	uint32_t numDivisions = 10;

	virtual void LoadModelData() override;

	//Helpers for Verts And Indicies
private:
	void GenerateVerts();

	void GenerateIndicies();

};

class NPlaneComponent : public MeshComponent {

	uint32_t numDivisions = 10;

	virtual void LoadModelData() override;

	//Helpers for Verts And Indicies
private:
	void GenerateVerts();

	void GenerateIndicies();


};