#include "LightingManager.h"

LightingManager* LightingManager::singletonManager = nullptr;

LightingManager::LightingManager() {

    if (singletonManager) {
        delete(this);
    }

    singletonManager = this;

}

uint32_t LightingManager::RegisterLightObject(LightSourceType LightType, LightComponent* component)
{

    LightSourceComponents.push_back(component);
    numLightSourceComponents++;
    return numLightSourceComponents-1;

}

void LightingManager::UpdateLightBuffer(LightShaderInfo* ShaderInfoArray)
{

    for (LightComponent* Light : LightSourceComponents) {

        Light->UpdateLightBufferInfo(ShaderInfoArray);
    }
    
}