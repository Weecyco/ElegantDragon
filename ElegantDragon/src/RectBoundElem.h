#pragma once
#include "DragonDB.h"
#include "GraphicObj.h"


//ideal: Each level with 2 items (more if we expect more misses), located based on k Means algo, depths are balances
class RectBoundElem : public GraphicObj
{
protected:
    friend class SceneContainer;
    GraphicObj** childObj = nullptr;
    unsigned int childCount;

    MathVec3f cornerPt0;
    MathVec3f cornerPt1;
    //for balancing purposes if there is movement (above a certatin threshold) remove and replace in closest COM. 
    //if all far, consider creating a new child 
    MathVec3f centerOfMass; 
    float weight;
    //float scale; scale = avgOfi(childObj(i)_COM - childObj(i+1)_COM) Store for quicker access?
    
public:
    RectBoundElem();
    ~RectBoundElem();
    RectBoundElem(DragonDB& DrgnDB, unsigned int sizeIn);
    void setChild(GraphicObj* ChildObjIn, unsigned int loc); //perhaps add pushback option?
    GraphicObj* getChild(unsigned int loc) const; // not considering child to be part of "itself"
    const unsigned int getChildCount() const;

    virtual bool intersectionPt(const MathVec3f& point);

    virtual MathVec3f boundsRangeMin(const MathVec3f& dir) const;
    virtual MathVec3f boundsRangeMax(const MathVec3f& dir) const;

    virtual MathVec3f boundsRangeCardinalMin(EDrgn::Direction3 dir) const;
    virtual MathVec3f boundsRangeCardinalMax(EDrgn::Direction3 dir) const;

    virtual MathVec3f boundsRangeCardinalMin() const;
    virtual MathVec3f boundsRangeCardinalMax() const;
};