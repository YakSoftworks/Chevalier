#pragma once
#include "ChevalierGraphicsCore.h"


#pragma region Cube

#define SHAPE_CUBE_VERTS \
    { \
        /*{ {Pos}, {Color}, {TexCoord} }*/ \
    { { -0.5f, -0.5f,  0.5f},  {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f} },\
    { {  0.5f, -0.5f,  0.5f},  {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} },\
    { {  0.5f,  0.5f,  0.5f},  {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f} },\
    { { -0.5f,  0.5f,  0.5f},  {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f} },\
    { { -0.5f, -0.5f, -0.5f},  {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} },\
    { {  0.5f, -0.5f, -0.5f},  {0.0f, 1.0f, 1.0f}, {1.0f, 0.0f} },\
    { {  0.5f,  0.5f, -0.5f},  {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} },\
    { { -0.5f,  0.5f, -0.5f},  {1.0f, 0.0f, 1.0f}, {0.0f, 1.0f} }\
    };\

    // RH - Triangle List
#define SHAPE_CUBE_INDICES \
    {\
        0,1,2,2,3,0,\
        4,7,6,6,5,4,\
        0,3,7,7,4,0,\
        2,1,5,5,6,2,\
        3,2,6,6,7,3,\
        1,0,4,4,5,1,\
    };\
    




#pragma endregion

#pragma region Sphere
#pragma endregion


#pragma region cylinder
#pragma endregion


