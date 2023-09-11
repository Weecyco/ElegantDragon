#include "SceneContainer.h"
#include "RectBoundElem.h"
#include <queue>

bool SceneContainer::insertTree(GraphicObj& objIn)
{
    MathVec3f objBoundMin = objIn.boundsRangeCardinalMin();
    MathVec3f objBoundMax = objIn.boundsRangeCardinalMax();
    RectBoundElem* curBVol = &TreeObjects;
    while (true)
    {
        MathVec3f& curBVolMin = curBVol->cornerPt0;
        MathVec3f& curBVolMax = curBVol->cornerPt1;

        //individually access for slight speed improvment. measure later against large datasets...
        //Step 1 adjust bounds of bounding volume if needed
        if (objBoundMin.x < curBVolMin.x)
        {
            curBVolMin.x = objBoundMin.x;
        }
        if (objBoundMin.y < curBVolMin.y)
        {
            curBVolMin.y = objBoundMin.y;
        }
        if (objBoundMin.z < curBVolMin.z)
        {
            curBVolMin.z = objBoundMin.z;
        }
        if (objBoundMax.x > curBVolMax.x)
        {
            curBVolMax.x = objBoundMax.x;
        }
        if (objBoundMax.y > curBVolMax.y)
        {
            curBVolMax.y = objBoundMax.y;
        }
        if (objBoundMax.z > curBVolMax.z)
        {
            curBVolMax.z = objBoundMax.z;
        }

        //Step 2 which child to place in
        
        //Directly place
        if (curBVol->childCount == 0)
        {

        }


        for (unsigned int childIdx = 0; childIdx < curBVol->childCount; ++childIdx)
        {
            if (curBVol->childObj[childIdx]->getType() == GOTypeList::BoundElem)
            {

            }
            else
            {

            }
        }
    }



    return false;
}

std::vector<GraphicObj*> SceneContainer::pointQuery(const MathVec3f queryPoint)
{
    std::vector<GraphicObj*> returnObjList;
    std::queue<GraphicObj*> traveralQ;
    traveralQ.push(&TreeObjects);

    GraphicObj* curObj;
    const RectBoundElem* curObjBBlock;
    GraphicObj* curObjChild;
    while (traveralQ.empty())
    {
        curObj = traveralQ.front();
        traveralQ.pop();
        if (curObj->getType() == GOTypeList::BoundElem) //Pray type is set correctly if casting issues are too common consider adding a hash for memory corruption check
        {
            curObjBBlock = reinterpret_cast<const RectBoundElem*>(curObj);
            for (unsigned int curObjChildIdx = 0; curObjChildIdx < curObjBBlock->getChildCount(); ++curObjChildIdx)
            {
                curObjChild = curObjBBlock->getChild(curObjChildIdx);
                if (curObjChild->intersectPt(queryPoint))
                {
                    traveralQ.push(curObjChild);
                }
            }
            
        }
        else 
        {
            returnObjList.push_back(curObj);
        }
    }
}

