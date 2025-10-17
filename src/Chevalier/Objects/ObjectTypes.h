#include "Core.h"


struct Transform{

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;


    glm::mat4x4 getTransform() {

        //TODO: Return Transform's Model Matrix

    }

};


struct RenderAttributes {
    
//Struct containing attributes for rendering an object
    bool bRender = true;

};