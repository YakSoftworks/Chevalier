#pragma once

#include "Core.h"
#include "Objects/ObjectTypes.h"


class Component{

    

protected:

    virtual void PreInitializeComponent();
    virtual void InitializeComponent(); // Register component
    virtual void PostInitializeComponent();

    //Core Realtime Functions - Override for usage
public:
    virtual void BeginPlay() {}; // Called whenever object is spawned
    virtual void Tick(float delta) {}; // Called every tick
    virtual void EndPlay() {}; // Called when an object is destroyed


};



// Component with a transform
class SceneComponent : public Component {

protected:
    //Owns a transform
    ModelTransform componentTransform;


};


