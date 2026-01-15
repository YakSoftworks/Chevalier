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

    switch (LightType) {

    case(SpotLight):

        if (numSpotLights >= CHEVALIER_CONSTANTS_MAX_SPOT_LIGHTS) {
            
            CHEV_MESSAGE_ERROR("UNABLE TO ADD ADDITIONAL SPOTLIGHT - MAX LIMIT REACHED");
            return -1;
        }

        numSpotLights++;
        return numSpotLights - 1;


        break;
    case(PointLight):

        

        if (numPointLights >= CHEVALIER_CONSTANTS_MAX_POINT_LIGHTS) {

            CHEV_MESSAGE_ERROR("UNABLE TO ADD ADDITIONAL POINTLIGHT - MAX LIMIT REACHED");
            return -1;
        }

        numPointLights++;

        return numPointLights - 1;


        break;
    case(DirectionalLight):

        if (numDirectionalLights >= CHEVALIER_CONSTANTS_MAX_DIRECTIONAL_LIGHTS) {

            CHEV_MESSAGE_ERROR("UNABLE TO ADD ADDITIONAL DIRECTIONAL LIGHT - MAX LIMIT REACHED");
            return -1;
        }

        
        numPointLights++;
        return numDirectionalLights - 1;

        break;


    }

    // We broke the switch statement, so return an error
    CHEV_MESSAGE_ERROR("FAILED TO CATCH AND RETURN INDEX - LIGHTING MANAGER");
    return -1;
}
