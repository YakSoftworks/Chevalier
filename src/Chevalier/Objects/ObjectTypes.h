#pragma once

#include "Core.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"


struct ModelTransform{

    glm::vec3 position;
    //As degrees
    glm::vec3 rotation;
    glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f);

    // Return Transform's Model Matrix
    glm::mat4 getTransform() {

        glm::mat4 transform(1.0f);

        glm::mat4 scaleMatrix = glm::scale(transform, scale);
        glm::mat4 rotationMatrix = 
            glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f)) *
            glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f)) *
            glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
        
        transform = glm::translate(transform, position) * (rotationMatrix * scaleMatrix);

        //transform = glm::translate(glm::scale(transform, scale), position);

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

    float timeSinceStart = 0;

    int numLights;

};

struct ObjectShaderData {

    glm::mat4 modelMat = glm::mat4();

};

struct LightShaderData {

    float intensity = 2.f;

};