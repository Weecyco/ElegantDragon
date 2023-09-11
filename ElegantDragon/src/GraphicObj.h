#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <list>

#include "graphic_api_interface/IndexBuffer.h"
#include "graphic_api_interface/VertexBuffer.h"
#include "graphic_api_interface/VertexArray.h"
#include "graphic_api_interface/Shader.h"
#include "graphic_api_interface/Textures.h"
#include "graphic_api_interface/Renderer.h"
#include "MathDrgn.h"
#include "GOTypeList.h"

/*
* Base class for graphical objects. 
* Gurenteed to have a VB, IB, VAO, and Shader defined
* May include CPU accessable VB and IB if defined 
* If so, it may be used to manipulate buffers on the cpu side
* 
* Pointers to each item are included for ease of access
*/

//implement something for bounds detection later?
//struct GraphicBound
//{
//    bool highbound = false; //lowbound = false highbound = true
//
//    GraphicObj* pGO;
//};

class GraphicObj
{
protected: //Switch to protected later
    unsigned int DDBIdx = 0; //location of this object in the database. Have 0 be invalid later (the no texture texture)

    GOTypeList type = GOTypeList::Unknown;
    std::string name = "uninitialized";
    unsigned int vertexCount = 0; //may or maynot be used depending on the obj.
    std::vector<std::pair<unsigned char, std::list<GraphicObj*>::iterator>> renderBlockIters;
    MathMatRMaj<float> ModelMat;
    bool modelMatDirtyBit = 0;

    int cpuVBLoc = -1;//set -1 if unset
    std::vector<float>* pcpuVB = nullptr;
    unsigned int VBLoc = 0;
    VertexBuffer* pVB = nullptr;

    int cpuIBLoc = -1;//set -1 if unset
    std::vector<unsigned int>* pcpuIB = nullptr;
    unsigned int IBLoc = 0;
    IndexBuffer* pIB = nullptr;

    //maybe add layout?
    unsigned int VAOLoc = 0;
    VertexArray* pVAO = nullptr;

    unsigned int SPLoc = 0;
    Shader* pSP = nullptr;

    unsigned int TXLoc = 0;
    Texture* pTX = nullptr;

public:
    /// <summary>
    /// Empty initialization
    /// </summary>
    GraphicObj() {
        //create a macro for handling warnings later
        PROJ_WARNING("Empty GraphicObj created (ignore if at start)");
    };

    /// <summary>
    /// Name / type  only initialization
    /// </summary>
    /// <param name="nameIn">Sets the name of this graphic object</param>
    GraphicObj(
        const std::string& nameIn,
        const GOTypeList typeIn = GOTypeList::Unknown
    ) :
        ModelMat(4, 4, 1)
    {
        name = nameIn;
        type = typeIn;
    };

    /// <summary>
    /// Copy and append to GObjs list
    /// </summary>
    /// <param name="GOIn">Input GO</param>
    /// <param name="GObjs">List to appended</param>
    GraphicObj(
        const GraphicObj& GOIn,
        std::vector<GraphicObj*>& GObjs
    ) : GraphicObj(GOIn) {
        GObjs.push_back(this);
    }

    /// <summary>
    /// Standard initialization
    /// </summary>
    /// <param name="GObjs">The main vector of graphic object that will be appended to</param>
    /// <param name="nameIn">Sets the name of this graphic object</param>
    GraphicObj(
        std::vector<GraphicObj*>& GObjs,
        const std::string& nameIn,
        const GOTypeList typeIn = GOTypeList::Unknown
    ) :
        ModelMat(4, 4, 1)
    {
        name = nameIn;
        type = typeIn;
        GObjs.push_back(this);
    };

    //Old way of initializing
    GraphicObj(
        std::vector<GraphicObj*>& GObjs,
        const unsigned int& VBLocIn,
        const unsigned int& IBLocIn,
        const unsigned int& VAOLocIn,
        const unsigned int& ShProgLocIn,
        const std::string& nameIn
    );

    //For "loose" GO's (GO's that aren't defined in the DB) 
    //All of them will have DDB of 0 but will have a address that doesn't match
    GraphicObj(
        const unsigned int& VBLocIn,
        const unsigned int& IBLocIn,
        const unsigned int& VAOLocIn,
        const unsigned int& ShProgLocIn,
        const std::string& nameIn
    );
    /*GraphicObj(const GraphicObj& srcGO) {

    }*/

private:
    GraphicObj(
        const unsigned int& DDBIdxIn,
        const unsigned int& VBLocIn,
        const unsigned int& IBLocIn,
        const unsigned int& VAOLocIn,
        const unsigned int& ShProgLocIn,
        const std::string& nameIn
    );
    //Add proper way of changing attributes later then set these things to private

public:
    inline void setVertexCount(const unsigned int& vertexCountIn) { vertexCount = vertexCountIn; };
    //Set Location to 0 if if does not refer to the DB verter indices
    void setCpuVB(const unsigned int& cpuVBLocIn, std::vector<float>& cpuVB);
    void setCpuIB(const unsigned int& cpuIBLocIn, std::vector<unsigned int>& cpuIB);

    void setCpuVB(std::vector<std::vector<float>>& cpuVBsIn);
    void setCpuIB(std::vector<std::vector<unsigned int>>& cpuIBsIn);

    void setCpuVB(std::vector<std::vector<float>>& cpuVBsIn, const unsigned int indexIn);
    void setCpuIB(std::vector<std::vector<unsigned int>>& cpuIBsIn, const unsigned int indexIn);


    //Set Location to 0 if it does not refer to the DB vector indices
    void setVB(const unsigned int& VBLocIn, VertexBuffer* pVBIn);
    void setIB(const unsigned int& IBLocIn, IndexBuffer* pIBIn);
    void setVAO(const unsigned int& VAOLocIn, VertexArray* pVAOIn);
    void setSP(const unsigned int& SPLocIn, Shader* pSPIn);
    //void setTX(const unsigned int& TXLocIn, Texture* pTXIn);

    //Auto Sets to the FINAL ITEM in the Vector
    void setVB(const std::vector<VertexBuffer*>& VBsIn);
    void setIB(const std::vector<IndexBuffer*>& IBsIn);
    void setVAO(const std::vector<VertexArray*>& VAOsIn);
    void setSP(const std::vector<Shader*>& SPsIn);
    //void setTX(const std::vector<Texture*>& TXsIn);

    //Auto Sets the component givent the Vector and location in that vector;
    void setVB(const std::vector<VertexBuffer*>& VBsIn, const unsigned int indexIn);
    void setIB(const std::vector<IndexBuffer*>& IBsIn, const unsigned int indexIn);
    void setVAO(const std::vector<VertexArray*>& VAOsIn, const unsigned int indexIn);
    void setSP(const std::vector<Shader*>& SPsIn, const unsigned int indexIn);
    //void setTX(const std::vector<Texture*>& TXsIn, const unsigned int indexIn);

    //returns pointer to Buffers
    inline std::vector<float>* getpcpuVB() { return pcpuVB; };
    inline std::vector<unsigned int>* getpcpuIB() { return pcpuIB; };
    VertexBuffer* getpVB(); //maybe change these to inline and define here if i don't need anything too complecated
    IndexBuffer* getpIB();
    VertexArray* getpVAO();
    Shader* getpSP();
    Texture* getpTX();

    const MathMatRMaj<float>& viewModelMat() const;
    MathMatRMaj<float>& editModelMat();

    //Default to draw
    virtual void draw(const Renderer& Rend) const { Rend.draw(*(pVAO), *(pIB), *(pSP)); };

    //Type check
    const GOTypeList getType() const;

    //Default to invisible / never intersect To be implemented per object
    virtual bool intersectPt(const MathVec3f& point) const {return false;};
    virtual bool intersectLine() const { return false;};
    virtual bool intersectFace() const { return false;};
    virtual bool intersectObj() const { return false;};

    /// <summary>
    /// Fins Min/Max point of its intersecting volume in a specific direction in world space
    /// </summary>
    /// <param name="dir">A Direction </param>
    /// <returns>Min/Max point of its intersecting volume</returns>
    virtual MathVec3f boundsRangeMin(const MathVec3f& dir) const;
    virtual MathVec3f boundsRangeMax(const MathVec3f& dir) const;

    /// <summary>
    /// Fins Min/Max point of its intersecting volume in a specific cardinal direction in world space
    /// </summary>
    /// <param name="dir">A Cardinal Direction </param>
    /// <returns>Min/Max point of its intersecting volume</returns>
    virtual MathVec3f boundsRangeCardinalMin(EDrgn::Direction3 dir) const;
    virtual MathVec3f boundsRangeCardinalMax(EDrgn::Direction3 dir) const;

    /// <summary>
    /// Returns Min/Max corner points of a rectangular bounding volume in world space
    /// Should contain all intersectable points
    /// </summary>
    /// <returns>Vec3 representing the corners of he volume </returns>
    virtual MathVec3f boundsRangeCardinalMin() const;
    virtual MathVec3f boundsRangeCardinalMax() const;

};