#pragma once

#include "ChevalierGraphicsCore.h"

// Forward Declarations
class ChevalierRenderObjectInterface;
class ChevalierMaterialInterface;



class GeometryManager
{
public:

	GeometryManager();

	// Singleton

protected:

	static GeometryManager* pSingletonReference;

public:

	static GeometryManager* getGeometryManager() { return pSingletonReference; }


	// Access to all geometry


protected:

	// Current Drawobjects
	std::vector<ChevalierRenderObjectInterface*> RenderObjects;

	// Current Materials
	std::vector<ChevalierMaterialInterface*> Materials;

	uint32_t numRegisteredObjects;
	uint32_t numRegisteredMaterials;


public:

	// Register Geometry Object

	// Inputs:
	// Geometry Reference
	// Material Reference
	// Outputs: 
	// RenderObjectID

	uint32_t RegisterGeometryObject(ChevalierRenderObjectInterface* newGeometryReference, ChevalierMaterialInterface* newMaterialReference);

	void PerformGeometryPass(VkCommandBuffer buffer, uint32_t currentFrame);


};

