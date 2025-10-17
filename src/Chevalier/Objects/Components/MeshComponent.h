#include "Core.h"
#include "Component.h"
#include "ChevalierInterfaces.h"



class MeshComponent : public SceneComponent, public ChevalierRenderObjectInterface {


    //ChevalierRenderObjectInterface Implementations
public:
    
    	// Function to bind object data to the given command buffer
	virtual void DrawObject(VkCommandBuffer buffer) override;

	// Returns a bool of whether or not an object is drawable
	virtual bool IsObjectDrawable() override;

	// Gets the resource to get information of the object's material
	virtual ChevalierMaterialInterface* GetObjectMaterial() override;

private:

    //TODO: References to mesh and material
        


};