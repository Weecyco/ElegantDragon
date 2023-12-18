#pragma once
#include "DragonDB.h"
#include "GraphicObj.h"
#include "MathDrgn.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

//TODO: Create Outline shader
//for now just use texture
class Box : public GraphicObj
{
private:
    MathVec3f cornerPt0;
    MathVec3f cornerPt1;

public:
    Box(DragonDB& DrgnDB);
    ~Box();
    
};

