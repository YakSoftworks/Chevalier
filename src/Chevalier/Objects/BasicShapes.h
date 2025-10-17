#include "ChevalierGraphicsCore.h"


#pragma region Cube

#define SHAPE_CUBE_VERTS \
    { \
        /*{ {Pos}, {Color}, {TexCoord} }*/ \
        {{.5, .5, .5}, {1, 1, 1}, {0, 0}}, \
        {{-.5, .5, .5}, {0, 1, 1}, {0, 0}}, \
        {{-.5, -.5, .5}, {0, 0, 1}, {0, 0}}, \
        {{.5, -.5, .5}, {1, 0, 1}, {0, 0}}, \
        {{.5, .5, -.5}, {1, 1, 0}, {0, 0}}, \
        {{-.5, .5, -.5}, {0, 1, 0}, {0, 0}}, \
        {{-.5, -.5, -.5}, {0, 0, 0}, {0, 0}}, \
        {{.5, -.5, -.5}, {1, 0, 0}, {0, 0}} \
    };\

    // RH - Triangle List
#define SHAPE_CUBE_INDICES \
    {\
        0, 1, 2, 2, 3, 0, /* +Z Face */ \
        4, 5, 6, 6, 7, 4, /* -Z Face */ \
        0, 3, 4, 4, 7, 0, /* +X Face */ \
        1, 2, 5, 5, 6, 1, /* -X Face */ \
        0, 1, 4, 4, 5, 0, /* +Y Face */ \
        2, 3, 6, 6, 7, 2  /* -Y Face */ \
    };\
    
#pragma endregion

#pragma region Sphere
#pragma endregion


#pragma cylinder
#pragma enderegion


