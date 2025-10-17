#include "Core.h"

#include "ObjectTypes.h"

#include "Actor.h"

class Map{

    // Container of Objects in the Map
    std::vector<Actor*> MapActors;

    // Map management

    //Runtime functions
public:
    virtual void BeginPlay() {
        for(Actor* actor : MapActors){
            actor->BeginPlay();
        }
    }
    virtual void Tick(float deltaTime){
        for(Actor* actor : MapActors){
            actor->Tick(deltaTime);
        }
    }
    virtual void EndPlay(){
        for(Actor* actor : MapActors){
            actor->EndPlay();
        }
    }



};
