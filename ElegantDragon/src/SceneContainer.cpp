#include "SceneContainer.h"
#include "RectBoundElem.h"
#include <queue>

// Currently  only works for rectangular axis aligned cases
// Algo is to minimize bounding volume size of its immediate child (greedy heuristic).
// Ideal would be to organize structure based on probability of future query. To bring objects as high as possible for regions of interest.
// Another optimization is to create a complex multi-threaded way where it distributes checks for each children to different threads and 
// make use of either multiple children per layer or checking parallel layers during overlap.
bool SceneContainer::insertTree(GraphicObj& objIn)
{
    MathVec3f objBoundMin = objIn.boundsRangeCardinalMin();
    MathVec3f objBoundMax = objIn.boundsRangeCardinalMax();

    if (TreeObjects == nullptr)
    {
        //Create!
        TreeObjects = new RectBoundElem();
        TreeObjects->pushBack(&objIn);
    }

    RectBoundElem* curBVol = TreeObjects;
    RectBoundElem** ParentPointerReference = &TreeObjects;
    MathVec3f* curBVolMin = &curBVol->cornerPt0;
    MathVec3f* curBVolMax = &curBVol->cornerPt1;
    MathVec3f selChildMin;
    MathVec3f selChildMax;
    if (objBoundMin.x < curBVolMin->x)
    {
        selChildMin.x = objBoundMin.x;
    }
    if (objBoundMin.y < curBVolMin->y)
    {
        selChildMin.y = objBoundMin.y;
    }
    if (objBoundMin.z < curBVolMin->z)
    {
        selChildMin.z = objBoundMin.z;
    }
    if (objBoundMax.x > curBVolMax->x)
    {
        selChildMax.x = objBoundMax.x;
    }
    if (objBoundMax.y > curBVolMax->y)
    {
        selChildMax.y = objBoundMax.y;
    }
    if (objBoundMax.z > curBVolMax->z)
    {
        selChildMax.z = objBoundMax.z;
    }
    *curBVolMin = selChildMin;
    *curBVolMax = selChildMax;
    while (true)
    {
        unsigned int childCount = curBVol->childCount;

        //Step 2 which child to place in
        
        //Directly place
        if (curBVol->childCount == 0)
        {
            curBVol->pushBack(&objIn);
            return true;
        }
        //Only one other that is 
        if (childCount == 1)
        {
            if (curBVol->childObj[0]->getType() == GOTypeList::BoundElem)
            {
                RectBoundElem* tempObj = curBVol;
                *ParentPointerReference = reinterpret_cast<RectBoundElem*>(curBVol->childObj[0]);
                curBVol = *ParentPointerReference;
                tempObj->childObj[0] = nullptr;
                delete tempObj;
                continue;
            }
            else
            {
                curBVol->pushBack(&objIn);
                return true;
            }
        }
        //Pass down to a child (TODO: or add a 3+ child for multithread cases)
        {
            MathVec3f tempChildMin;
            MathVec3f tempChildMax;
            MathVec3f origChildMin;
            MathVec3f origChildMax;
            double volScore = INFINITY;
            double selScore = INFINITY;
            unsigned int selChildIdx = 0;
            bool changeFlag;
            selChildMin.x = -INFINITY;
            selChildMin.y = -INFINITY;
            selChildMin.z = -INFINITY;
            selChildMax.x = INFINITY;
            selChildMax.y = INFINITY;
            selChildMax.z = INFINITY;
            
            //calculate new bound and decided which child this should be in
            for (unsigned int childIdx = 0; childIdx < curBVol->childObj.size(); ++childIdx)
            {
                origChildMin = curBVol->childObj[childIdx]->boundsRangeCardinalMin();
                origChildMax = curBVol->childObj[childIdx]->boundsRangeCardinalMax();


                changeFlag = false;
                //TODO add bounding cost func + bounding limit comparison generalization -> enables generalization to non-rectangular in the future!
                if (objBoundMin.x < origChildMin.x)
                {
                    tempChildMin.x = objBoundMin.x;
                    changeFlag = true;
                }
                if (objBoundMin.y < origChildMin.y)
                {
                    tempChildMin.y = objBoundMin.y;
                    changeFlag = true;
                }
                if (objBoundMin.z < origChildMin.z)
                {
                    tempChildMin.z = objBoundMin.z;
                    changeFlag = true;
                }
                if (objBoundMax.x > origChildMax.x)
                {
                    tempChildMax.x = objBoundMax.x;
                    changeFlag = true;
                }
                if (objBoundMax.y > origChildMax.y)
                {
                    tempChildMax.y = objBoundMax.y;
                    changeFlag = true;
                }
                if (objBoundMax.z > origChildMax.z)
                {
                    tempChildMax.z = objBoundMax.z;
                    changeFlag = true;
                }

                //Change in child size if placed
                volScore = (tempChildMax.x - tempChildMin.x) * (tempChildMax.y - tempChildMax.y) * (tempChildMax.z - tempChildMax.z) - 
                    (origChildMax.x - origChildMin.x) * (origChildMax.y - origChildMin.y) * (origChildMax.z - origChildMin.z);
                if (selScore > volScore)
                {
                    selScore = volScore;
                    selChildIdx = childIdx;
                    selChildMin = tempChildMin;
                    selChildMax = tempChildMax;
                }
            }

            //TODO: Add more complex analysis of score and possibly other metrics (ie size balancing between children and probablistic analysis) to better decide
            if (curBVol->childObj[selChildIdx]->getType() == GOTypeList::BoundElem)
            {
                curBVol = reinterpret_cast<RectBoundElem*>(curBVol->childObj[selChildIdx]);
                curBVolMin = &curBVol->cornerPt0;
                curBVolMax = &curBVol->cornerPt1;
            }
            else
            {
                RectBoundElem* tempBound = new RectBoundElem();
                tempBound->pushBack(curBVol->childObj[selChildIdx]);
                curBVol->childObj[selChildIdx] = tempBound;
            }

            *curBVolMin = selChildMin;
            *curBVolMax = selChildMax;
            curBVol = reinterpret_cast<RectBoundElem*>(curBVol->childObj[selChildIdx]);
        }
    }



    return false;
}

std::vector<GraphicObj*> SceneContainer::pointQuery(const MathVec3f queryPoint)
{
    std::vector<GraphicObj*> returnObjList;
    std::queue<GraphicObj*> traveralQ;
    traveralQ.push(TreeObjects);

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
    return returnObjList;
}

