#include "MVPRenderBlock.h"

quickAccessRenderBlock::quickAccessRenderBlock(std::vector<quickAccessRenderBlock*>& renderBlockListIn) : 
    cameraRot(1, 3),
    cameraLoc(1, 3)
{
    idxVt = 0;
    idxVr = 1;
    idxVs = 2;
    idxP = 3;
    addMatrix(MathMatRMaj<float>(4, 4, 1)); //Vt
    addMatrix(MathMatRMaj<float>(4, 4, 1)); //Vr
    addMatrix(MathMatRMaj<float>(4, 4, 1)); //Vs
    addMatrix(MathMatRMaj<float>(4, 4, 1)); //P

    renderBlockID = renderBlockListIn.size();
    renderBlockListIn.push_back(this);
}

//quickAccessRenderBlock::quickAccessRenderBlock(std::vector<quickAccessRenderBlock*>& renderBlockListIn, const std::list<GraphicObj*>& RendObjsListIn) : 
//    quickAccessRenderBlock(renderBlockListIn)
//{
//    for (auto iter = RendObjsListIn.begin(); iter != RendObjsListIn.end(); ++iter)
//    {
//        RendObjsList.push_back(*iter);
//    }
//}

quickAccessRenderBlock::quickAccessRenderBlock(std::vector<quickAccessRenderBlock*>& renderBlockListIn, const quickAccessRenderBlock& src) :
    quickAccessRenderBlock(renderBlockListIn)
{
    for (auto iter = src.RendObjsList.begin(); iter != src.RendObjsList.end(); ++iter)
    {
        RendObjsList.insert(*iter);
    }
}
static unsigned int debugCounter = 0;
void quickAccessRenderBlock::drawAll(const Renderer& Rend)
{
    //TODO: improve dirty bit implementation
    //first case is unique since it uses mat's in CM
    bool postDirtyBit = false;
    if (dirtyBits[CM.size() - 2] == true || dirtyBits[CM.size() - 1] == true)
    {
        RM[RM.size() - 1].multDirect(CM[CM.size() - 1], CM[CM.size() - 2]);
        postDirtyBit = true;
    }
    for (unsigned int resultIdx = RM.size() - 2; resultIdx < RM.size(); --resultIdx)
    {
        if (postDirtyBit == true)
        {
            RM[resultIdx].multDirect(RM[resultIdx + 1], CM[resultIdx]);
        }
        else if (dirtyBits[resultIdx] == true)
        {
            RM[resultIdx].multDirect(RM[resultIdx + 1], CM[resultIdx]);
            postDirtyBit = true;
        }
    }

    //reset dirty bits
    if (postDirtyBit)
    {
        for (unsigned short dirtyIdx = 0; dirtyIdx < dirtyBits.size(); ++dirtyIdx)
        {
            dirtyBits[dirtyIdx] = 0; //TODO: check if it is more efficent to check if dirtyBit is true first (depends on speed of read vs write)
        }
    }

    //render objects
    for (auto iter = RendObjsList.begin(); iter != RendObjsList.end(); ++iter)
    {
        //if()//TODO: check for model dirty bit & postDirtyBit to see if MVP needs to be regenerated
        iter->second.multDirect(RM[0], iter->first->viewModelMat());
        iter->first->getpSP()->bind();
        iter->first->getpSP()->setUniformMatrix4f("MVP", iter->second.getContent(), GL_TRUE);
        if (iter->first->getpTX() != nullptr)
        {
            //iter->first->getpSP()->setUniform1i("u_Texture", iter->first->getpTX()->getSlot());
            iter->first->getpTX()->bind();
        }
        if (debugCounter % 144 == 0)
        {
            if (iter->first->getName() == "Box")
            {
                MathMatRMaj<float> temp(1, 4);
                temp[0][0] = 1;
                temp[0][1] = 1;
                temp[0][2] = 1;
                temp[0][3] = 1;
                temp.multAssignReverse(iter->second);
                temp = temp * (1 / temp[0][3]);
                std::cout << "Box MVP: \n" << iter->second << "\n <1,1,1> translation: \n" << temp << std::endl;


            }
            if (iter->first->getpTX() != nullptr)
                std::cout << iter->first->getName() << ": " << iter->first->getpTX()->getSlot() << std::endl;
        }
        //if (iter->first->getName() != "Box")
        iter->first->draw(Rend);
    }
    debugCounter++;
}

void quickAccessRenderBlock::addMatrix(MathMatRMaj<float>&& _Val)
{
    CM.push_back(_Val);
    dirtyBits.push_back(true);
    if (CM.size() > RM.size() + 1)
    {
        RM.push_back(MathMatRMaj<float>(4, 4, 1));
    }
}

const MathMatRMaj<float>& quickAccessRenderBlock::viewMat(unsigned short compIndex) const
{
    return CM[compIndex];
}

const float& quickAccessRenderBlock::viewMat(unsigned short compIndex, unsigned char rowIdx, unsigned char colIdx) const
{
    return CM[compIndex].viewContent(rowIdx, colIdx);
}

MathMatRMaj<float>& quickAccessRenderBlock::editMat(unsigned short compIndex)
{
    dirtyBits[compIndex] = true;
    return CM[compIndex];
}

const MathMatRMaj<float>& quickAccessRenderBlock::viewRes(unsigned short resIndex) const
{
    return RM[resIndex];
}

const float& quickAccessRenderBlock::viewRes(unsigned short resIndex, unsigned char rowIdx, unsigned char colIdx) const
{
    return RM[resIndex].viewContent(rowIdx, colIdx);
}

void quickAccessRenderBlock::setTran(unsigned short matIndex, const float transX, const float transY, const float transZ)
{
    dirtyBits[matIndex] = true;
    float* content = CM[matIndex].getContent();

    content[0] = 1;
    content[1] = 0;
    content[2] = 0;
    content[3] = transX;

    content[4] = 0;
    content[5] = 1;
    content[6] = 0;
    content[7] = transY;

    content[8] = 0;
    content[9] = 0;
    content[10] = 1;
    content[11] = transZ;

    content[12] = 0;
    content[13] = 0;
    content[14] = 0;
    content[15] = 1;
}

void quickAccessRenderBlock::setScale(unsigned short matIndex, const float scaleX, const float scaleY, const float scaleZ, const float scaleW)
{
    dirtyBits[matIndex] = true;
    float* content = CM[matIndex].getContent();

    content[0] = scaleX;
    content[1] = 0;
    content[2] = 0;
    content[3] = 0;

    content[4] = 0;
    content[5] = scaleY;
    content[6] = 0;
    content[7] = 0;

    content[8] = 0;
    content[9] = 0;
    content[10] = scaleZ;
    content[11] = 0;

    content[12] = 0;
    content[13] = 0;
    content[14] = 0;
    content[15] = scaleW;
}

void quickAccessRenderBlock::setScaleRatio2(unsigned short matIndex, const float scale)
{
    dirtyBits[matIndex] = true;
    float* content = CM[matIndex].getContent();

    content[0] = scale;
    content[1] = 0;
    content[2] = 0;
    content[3] = 0;

    content[4] = 0;
    content[5] = scale;
    content[6] = 0;
    content[7] = 0;

    content[8] = 0;
    content[9] = 0;
    content[10] = 1;
    content[11] = 0;

    content[12] = 0;
    content[13] = 0;
    content[14] = 0;
    content[15] = 1;
}

void quickAccessRenderBlock::setScaleRatio3(unsigned short matIndex, const float scale)
{
    dirtyBits[matIndex] = true;
    float* content = CM[matIndex].getContent();

    content[0] = scale;
    content[1] = 0;
    content[2] = 0;
    content[3] = 0;

    content[4] = 0;
    content[5] = scale;
    content[6] = 0;
    content[7] = 0;

    content[8] = 0;
    content[9] = 0;
    content[10] = scale;
    content[11] = 0;

    content[12] = 0;
    content[13] = 0;
    content[14] = 0;
    content[15] = 1;

}

void quickAccessRenderBlock::setRotCard(unsigned short matIndex, unsigned char dir, const float angle)
{
    dirtyBits[matIndex] = true;

    float compCos = std::cosf(angle);
    float compSin = std::sinf(angle);

    float* content = CM[matIndex].getContent();

    content[0] = 1;
    content[1] = 0;
    content[2] = 0;
    content[3] = 0;

    content[4] = 0;
    content[5] = 1;
    content[6] = 0;
    content[7] = 0;

    content[8] = 0;
    content[9] = 0;
    content[10] = 1;
    content[11] = 0;

    content[12] = 0;
    content[13] = 0;
    content[14] = 0;
    content[15] = 1;

    CM[matIndex][0 + dir][0 + dir] = compCos;
    CM[matIndex][0 + dir][(1 + dir)%3] = -compSin;
    CM[matIndex][(1 + dir)%3][0 + dir] = compSin;
    CM[matIndex][(1 + dir)%3][(1 + dir)%3] = compCos;
}

void quickAccessRenderBlock::applyRotCard(unsigned short matIndex, unsigned char dir, const float angle)
{
    dirtyBits[matIndex] = true;
    MathMatRMaj<float> NewMat(4,4,1);

    float compCos = std::cosf(angle);
    float compSin = std::sinf(angle);

    NewMat[0 + dir][0 + dir] = compCos;
    NewMat[0 + dir][(1 + dir) % 3] = -compSin;
    NewMat[(1 + dir) % 3][0 + dir] = compSin;
    NewMat[(1 + dir) % 3][(1 + dir) % 3] = compCos;

    CM[matIndex] *= NewMat;
}

/// <summary>
/// Creates a orthogonal projection at for the matrix index specified. Expects the target matrix to be initialized as an identity
/// </summary>
/// <param name="matIndex">Matrix index</param>
/// <param name="screenSpace">A matrix with screen space specifications: <<Lower Bound x, Lower Bound y>, <Upper Bound x, Upper Bound y>, <resolution x, resolution y>></param>
/// <param name="scaleType">Type of scaling to use. Resolution of the bounded area must be included in screen space matrix for all cases</param>
/// <param name="minDepth">World space depth (mapped to -1)</param>
/// <param name="maxDepth">World space depth (mapped to 1)</param>
void quickAccessRenderBlock::setOrthoProj(unsigned short matIndex, const float screenSpace[][2], const OrthoScalePreset& scaleType, const float minDepth, const float maxDepth)
{
    dirtyBits[matIndex] = true;

    //set consts
    CM[matIndex][0][1] = 0;
    CM[matIndex][0][2] = 0;
    CM[matIndex][1][0] = 0;
    CM[matIndex][1][2] = 0;
    CM[matIndex][2][0] = 0;
    CM[matIndex][2][1] = 0;
    CM[matIndex][3][0] = 0;
    CM[matIndex][3][1] = 0;
    CM[matIndex][3][2] = 0;
    CM[matIndex][3][3] = 1;

    switch (scaleType)
    {
    case OrthoScalePreset::PerPixel:
        //For multiplying constant
        CM[matIndex][0][0] = (screenSpace[1][0] - screenSpace[0][0]) / screenSpace[2][0];
        CM[matIndex][1][1] = (screenSpace[1][1] - screenSpace[0][1]) / screenSpace[2][1];
        break;
    case OrthoScalePreset::VerticalIs1:
        //For multiplying constant
        CM[matIndex][0][0] = (screenSpace[1][0] - screenSpace[0][0]) * screenSpace[2][1] / screenSpace[2][0] / 2.0f;
        CM[matIndex][1][1] = (screenSpace[1][1] - screenSpace[0][1]) / 2.0f;
        break;
    case OrthoScalePreset::HorizontalIs1:
        //For multiplying constant
        CM[matIndex][0][0] = (screenSpace[1][0] - screenSpace[0][0]) / 2.0f;
        CM[matIndex][1][1] = (screenSpace[1][1] - screenSpace[0][1]) * screenSpace[2][0] / screenSpace[2][1] / 2.0f;
        break;
    }
    //For adding constant
    CM[matIndex][0][3] = (screenSpace[1][0] + screenSpace[0][0]) / 2.0f;
    CM[matIndex][1][3] = (screenSpace[1][1] + screenSpace[0][1]) / 2.0f;

    //for z mapping from [minDepth, maxDepth] -> [-1, 1]
    CM[matIndex][2][2] = 2.0f / (maxDepth - minDepth);
    CM[matIndex][2][3] = (minDepth + maxDepth) / (minDepth - maxDepth);
}

void quickAccessRenderBlock::setOrthoProj(unsigned short matIndex, const float screenSpace[][2], const MathMatRMaj<float>& worldSpace)
{
    //Implement later lol I'm too lazy
}

void quickAccessRenderBlock::setPerspProj(unsigned short matIndex, const float screenSpace[][2], const PerspScalePreset& scaleType, const float minDepth, float maxDepth, const float FOV)
{
    //set consts
    CM[matIndex][0][1] = 0;
    CM[matIndex][0][3] = 0;
    CM[matIndex][1][0] = 0;
    CM[matIndex][1][3] = 0;
    CM[matIndex][2][0] = 0;
    CM[matIndex][2][1] = 0;
    CM[matIndex][3][0] = 0;
    CM[matIndex][3][1] = 0;
    CM[matIndex][3][2] = 1;
    CM[matIndex][3][3] = 0;
    switch (scaleType)
    {
    case PerspScalePreset::VerticalFOV:
        //For multiplying constant AND perspective FOV mapping
        CM[matIndex][0][0] = (screenSpace[1][0] - screenSpace[0][0]) * screenSpace[2][1] / screenSpace[2][0] / 2.0f / std::tanf(FOV/2.0f);
        CM[matIndex][1][1] = (screenSpace[1][1] - screenSpace[0][1]) / 2.0f / std::tanf(FOV / 2.0f);
        
        break;
    case PerspScalePreset::HorizontalFOV:
        //For multiplying constant AND perspective FOV mapping
        CM[matIndex][0][0] = (screenSpace[1][0] - screenSpace[0][0]) / 2.0f / std::tanf(FOV / 2.0f);
        CM[matIndex][1][1] = (screenSpace[1][1] - screenSpace[0][1]) * screenSpace[2][0] / screenSpace[2][1] / 2.0 / std::tanf(FOV / 2.0f);
        break;
    }
    //For adding constant (corrects screen space offset)
    CM[matIndex][0][2] = (screenSpace[1][0] + screenSpace[0][0]) / 2.0f;
    CM[matIndex][1][2] = (screenSpace[1][1] + screenSpace[0][1]) / 2.0f;

    //for z mapping from [minDepth, maxDepth] -> [-1, 1] AND perspective AND z-order correction
    CM[matIndex][2][2] = (minDepth + maxDepth) / (maxDepth - minDepth);
    CM[matIndex][2][3] = -2.0f * minDepth * maxDepth / (maxDepth - minDepth);
}