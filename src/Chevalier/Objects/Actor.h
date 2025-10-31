#pragma once
#include "Core.h"

#include "Components/Component.h"
#include "ObjectTypes.h"

class Actor{

    //Has a set of components it manages - Calls their management functions during its own
    std::vector<Component*> actorComponents;

    //Realtime functions - Make sure to call these if overrided
public:
    virtual void BeginPlay() {
        for(Component* component : actorComponents){
            component->BeginPlay();
        }
    };

    virtual void Tick(float deltaTime) {
        for(Component* component : actorComponents){
            component->Tick(deltaTime);
        }
    };

    virtual void EndPlay() {
        for(Component* component : actorComponents){
            component->EndPlay();
        }
    };

    //Component Management
public:

    void AddComponentToActor(Component* component) { actorComponents.push_back(component); }


};