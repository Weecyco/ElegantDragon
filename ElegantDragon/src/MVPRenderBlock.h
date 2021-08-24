#pragma once
#include <forward_list>
#include <map>

#include "graphicObj.h"
#include "MathDrgn.h"

//May soon be deprecated (not enough reason to hard wire an ordered data structure if the gpu handles it)
//may be useful for mouse selection, but not for rendering
//Handles the layering for a render space
//provides O(n) all item traversal
//provides O(log(n)) complexity insert, deletion, and search when no duplicate layer numbers O(n) when all layer numbers are equal
class quickOrderRenderBlock
{
private:
    std::list<GraphicObj*> RendObjsList;
    std::multimap<unsigned int, std::list<GraphicObj*>::iterator> RendObjsMap;
public:
    void drawAll(const Renderer& Rend) const;

    inline std::list<GraphicObj*>::iterator begin() { return RendObjsList.begin(); };
    inline std::list<GraphicObj*>::iterator end() { return RendObjsList.end(); };

    //Add a O(1) version that for push back / front later that gives the O(log(n)) part to a separate thread
    std::list<GraphicObj*>::iterator insert(GraphicObj* GOIn);
    bool del(GraphicObj* GOIn);
    std::list<GraphicObj*>::iterator find(GraphicObj* GOIn);
};


//faster at push back / front and lower overall overhead (when object ordering is not needed)
class quickAccessRenderBlock {
protected:
    std::list<GraphicObj*> RendObjsList; //list of objects in this render space (must have MVP uniform) TODO: make into a pair<>
    unsigned char renderBlockID = 0;

    //TODO: option to switch to using a tree pattern if we want log(n) for all singular edits but nlog(n) space complexity instead of n
    // (only useful if we want a lot more layers to the transform)
    //Lower indexes Require fewer calculations to generate result
    //Organize based on use case
    //By default, Index: Vt = 0, Vr = 1, Vs = 2, P = 3
    std::vector<MathMatRMaj<float>> CM; //Component Matricies
    std::vector<MathMatRMaj<float>> RM; //Result Matricies TODO: Consider changing this to = size of CM and copy first item before use
    unsigned short idxVt; //First operation to bring camera to center
    unsigned short idxVr;
    unsigned short idxVs;
    unsigned short idxP;

    //could use bit wise accessing instead of wasting 16.5% of the space but meh
    //dirty bits for which ones need to be multiplied
    std::vector<bool> dirtyBits; //maybe used a number of bools? might be faster tho less spece efficient 

    //TODO: Make derivative camera class??
    //camera is with no rotation / translation is looking towards "z" at 0, 0, 0
    MathMatRMaj<float> cameraRot;
    MathMatRMaj<float> cameraLoc;

public:
    quickAccessRenderBlock(std::vector<quickAccessRenderBlock*>& renderBlockListIn);
    quickAccessRenderBlock(std::vector<quickAccessRenderBlock*>& renderBlockListIn, const std::list<GraphicObj*>& RendObjsListIn);
    quickAccessRenderBlock(std::vector<quickAccessRenderBlock*>& renderBlockListIn, const quickAccessRenderBlock& src);
    quickAccessRenderBlock(const quickAccessRenderBlock& src) {PROJ_ASSERT_W_MSG(0, "Copy constructor is not enabled for quickAccessRenderBlock. Please use 'quickAccessRenderBlock(std::vector<quickAccessRenderBlock>& renderBlockListIn, const quickAccessRenderBlock& src)'"); };\
                                                               //temporarily disable copy con. maybe implement later but we have the renderBlockList dependent one
                                                               //privating was causing compilation issues. 
    
                                                               //draw function
    void drawAll(const Renderer& Rend);

    //Object list control
    inline std::list<GraphicObj*>::iterator begin() { return RendObjsList.begin(); };
    inline std::list<GraphicObj*>::iterator end() { return RendObjsList.end(); };

    inline void push_back(GraphicObj* pGOIn) { RendObjsList.push_back(pGOIn); };
    inline void push_front(GraphicObj* pGOIn) { RendObjsList.push_front(pGOIn); };
    inline void remove(GraphicObj* GOIn) { RendObjsList.remove(GOIn); };
    inline void erase(std::list<GraphicObj*>::iterator iter) { RendObjsList.erase(iter); };
    inline std::list<GraphicObj*>::iterator find(GraphicObj* pGOIn) {};

    //Matrix vector control
    void addMatrix(MathMatRMaj<float>&& _Val);

    //directly access the matrix
    //Component Matrices
    const MathMatRMaj<float>& viewMat(unsigned short compIndex) const;
    const float& viewMat(unsigned short compIndex, unsigned char rowIdx, unsigned char colIdx) const;
    MathMatRMaj<float>& editMat(unsigned short compIndex);
    //Result Matrices
    const MathMatRMaj<float>& viewRes(unsigned short resIndex) const;
    const float& viewRes(unsigned short resIndex, unsigned char rowIdx, unsigned char colIdx) const;

    //Camera operations
    void setTran(unsigned short matIndex, const float transX, const float transY, const float transZ);
    
    void setScale(unsigned short matIndex, const float scaleX, const float scaleY, const float scaleZ, const float scaleW);
    void setScaleRatio2(unsigned short matIndex, const float scale);
    void addScaleRatio2(unsigned short matIndex, const float scale); //TODO: make create this function
    void setScaleRatio3(unsigned short matIndex, const float scale);

    /// <summary>
    /// Sets Rotation along a cardinal axis along(0=z (xy plane), 1=x (yz plane), 2=y (zx plane))
    /// </summary>
    void setRotCard(unsigned short matIndex, unsigned char dir, const float angle);
    
    /// <summary>
    /// Rotates along a cardinal axis along(0=z (xy plane), 1=x (yz plane), 2=y (zx plane))
    /// </summary>
    void applyRotCard(unsigned short matIndex, unsigned char dir, const float angle);

    //PerPixel: 1 unit = 1 pixel
    //VerticalIs1: full vertical range = 1
    //HorizontalIs1: full horizontal range = 1
    enum class OrthoScalePreset {
        PerPixel      = 0,
        VerticalIs1   = 1,
        HorizontalIs1 = 2
    };

    void setOrthoProj(unsigned short matIndex, const float screenSpace[][2], const OrthoScalePreset& scaleType, const float minDepth, const float maxDepth);
    void setOrthoProj(unsigned short matIndex, const float screenSpace[][2], const MathMatRMaj<float>& worldSpace);
    
    enum class PerspScalePreset {
        VerticalFOV = 0,
        HorizontalFOV = 1
    };

    void setPerspProj(unsigned short matIndex, const float screenSpace[][2], const PerspScalePreset& scaleType, const float minDepth, const float maxDepth, const float FOV);
};