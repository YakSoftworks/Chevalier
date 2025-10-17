#include "Core.h"
#include "ObjectTypes.h"


class Component{

    
    //Core Realtime Functions - Override for usage
public:
    virtual void BeginPlay() {}; // Called whenever object is spawned
    virtual void Tick(float delta) {}; // Called every tick
    virtual void EndPlay() {}; // Called when an object is destroyed


};



// Component with a transform
class SceneComponent : public Component {

    //Owns a transform
    ModelTransform componentTransform;


};


