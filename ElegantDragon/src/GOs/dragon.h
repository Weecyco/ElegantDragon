#pragma once
#include "DragonDB.h"
#include "GraphicObj.h"
#include "Renderer.h"
#include "MathDrgn.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

enum class DrgnType {
    SolidCpuRend = 0,
    SolidShaderTransform = 1,
    Style0ShaderTransform = 2,
    SolidMVP = 3,
    Style0MVP = 4
};

class Drgn : public GraphicObj
{
public:
    static GraphicObj defaultGO; //some derived classes my not have a default
private:
    DrgnType drgnType;
    char* turns;
    unsigned int len; //how many turns are stored
    unsigned int curLen; //how many turns to draw
    MathVec<float> location;

public:
    Drgn(DragonDB& DrgnDB, unsigned int lenIn, const DrgnType shaderType = DrgnType::SolidShaderTransform);
    Drgn(const Drgn& src);
    ~Drgn();

    void printTurns();

    void setCurLen(const unsigned int curLenIn);

    int setLocation(float x, float y);
    int setLocation(MathVec<float> src);
    int drawDragon(DragonDB& DrgnDB);

    bool operator[](const unsigned int) const;

private:
    unsigned dragonRecur(unsigned& index);

    /// <summary>
    /// Generates Dragon Verticies with the given length
    /// </summary>
    /// <param name="len">Turn length that is to be generated</param>
    /// <returns>dynamically allocated array of vertex locations ({x0, y0, x1, y1, ...})</returns>
    float* generateDragonVert(const unsigned int& len);

    /// <summary>
    /// Appends texture map to all verticies
    /// </summary>
    /// <param name="drgnVert">Input dynamically allocated array with raw vertex data</param>
    /// <param name="textureMap">The mapping that will be repeatedly appended per dragon turn (should be size of vertexCount x 2)</param>
    float* appendTextureMap(float* drgnVert, const float* textureMap, const unsigned int& lenIn);

    /// <summary>
    /// Generates Dragon Indices with the given len
    /// Should be used to adjust Dragon size after generation
    /// </summary>
    /// <param name="len">Turn length that is to be generated</param>
    /// <returns>dynamically allocated array of indicies</returns>
    unsigned int* generateDragonIdx(const unsigned int& len);
};

