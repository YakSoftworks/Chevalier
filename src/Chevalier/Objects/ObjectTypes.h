#pragma once

#include "Core.h"

#include "glm/gtc/matrix_transform.hpp"


struct ModelTransform{

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;


    glm::mat4 getTransform() {

        //TODO: Return Transform's Model Matrix
        glm::mat4 transform(1.0f);


        transform = glm::scale(glm::translate(transform, position), scale);


        return transform;
    }

};


struct RenderAttributes {
    
//Struct containing attributes for rendering an object
    bool bRender = true;

};


struct GlobalDataObject {

    

    glm::mat4 perspectiveMat;
    glm::mat4 viewMat;
    glm::mat4 projViewMat;
    glm::mat4 debugModelMat;

};

struct ObjectShaderData {

    glm::mat4 modelMat = glm::mat4();

};

struct LightShaderData {

    float intensity = 2.f;

};