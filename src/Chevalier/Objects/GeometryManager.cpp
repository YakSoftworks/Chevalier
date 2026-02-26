#include "GeometryManager.h"

#include "Materials/ChevalierMaterial.h"
#include "ChevalierInterfaces.h"


GeometryManager* GeometryManager::pSingletonReference = nullptr;

GeometryManager::GeometryManager() {

    if (pSingletonReference) {
        delete(this);
        return;
    }
    
    
    pSingletonReference = this;

    RenderObjects.reserve(CHEVALIER_CONSTANTS_INITIAL_MODEL_COUNT);
    Materials.reserve(CHEVALIER_CONSTANTS_INITIAL_MODEL_COUNT);

}

uint32_t GeometryManager::RegisterGeometryObject(ChevalierRenderObjectInterface* newGeometryReference, ChevalierMaterialInterface* newMaterialReference)
{
    // Save Reference   
    RenderObjects.push_back(newGeometryReference);
    
    if (newMaterialReference) {

        Materials.push_back(newMaterialReference);
        numRegisteredMaterials++;

    }

    // Increment our index
    numRegisteredObjects++;

    return numRegisteredObjects - 1;

}

void GeometryManager::PerformGeometryPass(VkCommandBuffer buffer, uint32_t currentFrame, bool useObjectMaterial)
{

    ObjectShaderData* PerObjectDataThisFrame = static_cast<ObjectShaderData*>(ChevalierMaterial::sGlobalDataManager.modelMatrixBuffersMapped[currentFrame]);

    for (ChevalierRenderObjectInterface* object : RenderObjects) {

        if (object->IsObjectDrawable()) {
            //ChevalierMaterialInterface* material = object->GetObjectMaterial();

            if(useObjectMaterial) {
                // Bind Material
                object->GetObjectMaterial()->BindMaterial(&buffer, currentFrame);
            }
            //draw Object

            object->DrawObject(buffer, PerObjectDataThisFrame);

        }
    }
}
