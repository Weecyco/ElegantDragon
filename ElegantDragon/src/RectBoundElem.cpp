#include "RectBoundElem.h"

RectBoundElem::RectBoundElem() :
    GraphicObj("Bounding Block Element", GOTypeList::BoundElem)
{
    childCount = 0;
    weight = 0;
}

RectBoundElem::RectBoundElem(DragonDB& DrgnDB, unsigned int sizeIn = 0) :
    GraphicObj(DrgnDB.GObjs, "Bounding Block Element")
{
    childCount = sizeIn;
    if (sizeIn > 0)
    {
        childObj = new (GraphicObj*[sizeIn]);
        //initialize base
        for (unsigned int childIdx = 0; childIdx < childCount; ++childIdx)
        {
            childObj[childIdx] = nullptr;
        }
    }
    else
        childObj = nullptr;

}

RectBoundElem::~RectBoundElem()
{
    delete[] childObj;
}

void RectBoundElem::setChild(GraphicObj* ChildObjIn, unsigned int loc)
{
    PROJ_ASSERT_W_MSG(childCount > 0, "Bounding Block is empty");
    PROJ_ASSERT_W_MSG(ChildObjIn, "Child list not initialized");

    childObj[loc] = ChildObjIn;
}

GraphicObj* RectBoundElem::getChild(unsigned int loc) const
{
    return childObj[loc];
}

const unsigned int RectBoundElem::getChildCount() const
{
    return childCount;
}

bool RectBoundElem::intersectionPt(const MathVec3f& point)
{
    return false;
}

MathVec3f RectBoundElem::boundsRangeMin(const MathVec3f& dir) const
{
    return MathVec3f();
}

MathVec3f RectBoundElem::boundsRangeMax(const MathVec3f& dir) const
{
    return MathVec3f();
}

MathVec3f RectBoundElem::boundsRangeCardinalMin(EDrgn::Direction3 dir) const
{
    return MathVec3f();
}

MathVec3f RectBoundElem::boundsRangeCardinalMax(EDrgn::Direction3 dir) const
{
    return MathVec3f();
}

MathVec3f RectBoundElem::boundsRangeCardinalMin() const
{
    //should always be world axis aligned, just use cornerPt
    return cornerPt0;
}

MathVec3f RectBoundElem::boundsRangeCardinalMax() const
{
    return cornerPt1;
}
