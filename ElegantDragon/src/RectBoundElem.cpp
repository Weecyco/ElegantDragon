#include "RectBoundElem.h"

RectBoundElem::RectBoundElem() :
    GraphicObj("Bounding Block Element", GOTypeList::BoundElem)
{
    //default to size 2;
    childObj.resize(defaultSize);
    //set to null
    memset(childObj.data(), 0, defaultSize * sizeof(GraphicObj*));
    weight = 0;
    childCount = 0;
}

RectBoundElem::~RectBoundElem()
{
    for (auto childIter = childObj.begin(); childIter != childObj.end(); childIter++)
    {
        if ((*childIter) != nullptr && (*childIter)->getType() == GOTypeList::BoundElem)
        {
            //BoundingElement are not generally kept with the main database. memory management needs to be done on the fly
            delete (*childIter);
        }
    }
}

void RectBoundElem::setChild(GraphicObj* ChildObjIn, unsigned int loc)
{
    PROJ_ASSERT_W_MSG(childObj.size() > loc, "Location does not exist yet");
    PROJ_ASSERT_W_MSG(ChildObjIn, "Child list not initialized");

    childObj[loc] = ChildObjIn;
}

GraphicObj* RectBoundElem::getChild(unsigned int loc) const
{
    return childObj[loc];
}

void RectBoundElem::pushBack(GraphicObj* ChildObjIn)
{

    MathVec3f objBoundMin = ChildObjIn->boundsRangeCardinalMin();
    MathVec3f objBoundMax = ChildObjIn->boundsRangeCardinalMax();
    if (objBoundMin.x < cornerPt0.x)
    {
        cornerPt0.x = objBoundMin.x;
    }
    if (objBoundMin.y < cornerPt0.y)
    {
        cornerPt0.y = objBoundMin.y;
    }
    if (objBoundMin.z < cornerPt0.z)
    {
        cornerPt0.z = objBoundMin.z;
    }
    if (objBoundMax.x > cornerPt1.x)
    {
        cornerPt1.x = objBoundMax.x;
    }
    if (objBoundMax.y > cornerPt1.y)
    {
        cornerPt1.y = objBoundMax.y;
    }
    if (objBoundMax.z > cornerPt1.z)
    {
        cornerPt1.z = objBoundMax.z;
    }

    if (++childCount > childObj.size())
    {
        childObj.push_back(ChildObjIn);
    }
    else
    {
        childObj[childCount - 1] = ChildObjIn;
    }
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

//float RectBoundElem::getCOM() const
//{
//
//}