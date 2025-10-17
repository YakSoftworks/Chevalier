#pragma once

#include "Core.h"

#include "glm/gtc/matrix_transform.hpp"


struct ModelTransform{

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;


    glm::mat4x4 getTransform() {

        //TODO: Return Transform's Model Matrix

        /*glm::mat4 transform(1.0f);

        glm::translate(transform, position);


        glm::scale(transform, scale);*/

    }

};


struct RenderAttributes {
    
//Struct containing attributes for rendering an object
    bool bRender = true;

};


struct GlobalDataObject {



};

struct ObjectShaderData {



};

struct LightShaderData {



};