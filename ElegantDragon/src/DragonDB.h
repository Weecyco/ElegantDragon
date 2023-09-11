#pragma once
#include <utility>
#include <chrono>
#include <vector>

#include "GraphicObj.h"
#include "graphic_api_interface/Renderer.h"
#include "MathDrgn.h"
#include "MVPRenderBlock.h"

#define INNER_VERTEX_DISTANCE 0.5857864376 //1-(1/tan(3PI/8))
#define INNER_VERTEX_DISTANCE_THIRD 0.1952621459 //(1-(1/tan(3PI/8)))/3

#define GUI_BLOCK_INDEX 0
#define WORLD_BLOCK_INDEX 1


enum class CameraTranType
{
    Jump = 0,
    Linear = 1,
    Smoothed = 2
};

/*
* Data base for the Dragon application. Holds various settings and components
*/
class DragonDB
{
private:
    
    const unsigned int contextIndex; //for future use when multiple graphical contexts are necessary

    /*
    * Loads graphical components into structures and prepares them for rendering 
    * 
    * Input:
    *   TODO: future graphic package or folder
    * Return: 
    *   0 for no errors
    *   1 for error
    * expand if needed
    */
    char graphicsSetup();

public:
    //Holds the API specific calls for draw polls
    Renderer rend;

    //Holds all views to be rendered (View & Projection matrix) and their assiciated GO's (containing Model Matrix);
    //sample use case: one for UI, one for 3D world with computer, one for 3D game on that computer
    std::vector<quickAccessRenderBlock*> RenderBlocks;
    
    //all graphical objects will reference these graphic elements to let themselves be drawn.
    std::vector<GraphicObj*> GObjs; //holds an array of graphic Obj which will hold the vertex, index buffers, and textures 

    //std::multimap<float, > implement something for accelerating bounds detection later

    /*
    * cpuIB and cpuVB are for refencing IB/VB on cpu side if needed. 
    * less efficient, but doesn't rely on shader to manipulate
    * 
    * Note for better performance maybe change to an array on the inside or pair or maybe custom struct?
    */
    std::vector<std::vector<unsigned int>> cpuIBs;
    std::vector<std::vector<float>> cpuVBs;

    //might be better to use a linked list depending on the use case ie. if we constantly need to load in or remove things
    std::vector<IndexBuffer*> IBs;
    std::vector<VertexBuffer*> VBs;
    std::vector<VertexArray*> VAOs;//the vao should reference the VB and layout
    //maybe make layout array?
    std::vector<Shader*> SPs;
    std::vector<Texture*> TXs;

    const std::pair<unsigned int, unsigned int> resolution;
    const double homeScaling;

    DragonDB(const unsigned int DDBIdx, const float homeScaling, const std::pair<unsigned int, unsigned int> &resolutionIn);
    ~DragonDB();
};

