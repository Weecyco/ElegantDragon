#include "GraphicObj.h"
#include "DragonDB.h"

GraphicObj::GraphicObj(
    std::vector<GraphicObj*>& GObjs, 
    const unsigned int& VBLocIn, 
    const unsigned int& IBLocIn, 
    const unsigned int& VAOLocIn, 
    const unsigned int& ShProgLocIn, 
    const std::string& nameIn)
    :GraphicObj(GObjs.size(), VBLocIn, IBLocIn, VAOLocIn, ShProgLocIn, nameIn)
{
    GObjs.push_back(this);
}

GraphicObj::GraphicObj(const unsigned int& VBLocIn, const unsigned int& IBLocIn, const unsigned int& VAOLocIn, const unsigned int& ShProgLocIn, const std::string& nameIn)
    :GraphicObj(0, VBLocIn, IBLocIn, VAOLocIn, ShProgLocIn, nameIn)
{
}

GraphicObj::GraphicObj(
    const unsigned int& DDBIdxIn,
    const unsigned int& VBLocIn, 
    const unsigned int& IBLocIn, 
    const unsigned int& VAOLocIn, 
    const unsigned int& ShProgLocIn, 
    const std::string& nameIn) : 
    ModelMat(4, 4)
{
    //default to identity matrix (depreciated constructors but meh updating it anyways)
    ModelMat[0][0] = 1;
    ModelMat[1][1] = 1;
    ModelMat[2][2] = 1;
    ModelMat[3][3] = 1;

    DDBIdx = DDBIdxIn;
    //EXCLUDING direct access temporarily due to circular dependency between GraphicObj & DragonDB

    cpuVBLoc = -1;
    VBLoc = VBLocIn;
    //pVB = DDBIn.VBs[VBLocIn];

    cpuIBLoc = -1;
    IBLoc = IBLocIn;
    //pIB = DDBIn.IBs[IBLocIn];

    VAOLoc = VAOLocIn;
    //pVAO = DDBIn.VAOs[VAOLocIn];

    SPLoc = ShProgLocIn;
    //pShProg = DDBIn.ShProgs[ShProgLocIn];

    name = nameIn;
}

void GraphicObj::setCpuVB(const unsigned int& cpuVBLocIn, std::vector<float>& cpuVB)
{
    cpuVBLoc = cpuVBLocIn;
    pcpuVB = &cpuVB;
}

void GraphicObj::setCpuIB(const unsigned int& cpuIBLocIn, std::vector<unsigned int>& cpuIB)
{
    cpuIBLoc = cpuIBLocIn;
    pcpuIB = &cpuIB;
}

void GraphicObj::setCpuVB(std::vector<std::vector<float>>& cpuVBsIn)
{
    PROJ_ASSERT_W_MSG(cpuVBsIn.size() > 0, "Attempted to set A Graphic Object's cpu's Vertex Buffer with an empty cpuVB vector");
    cpuVBLoc = cpuVBsIn.size() - 1;
    pcpuVB = &cpuVBsIn.back();
}

void GraphicObj::setCpuIB(std::vector<std::vector<unsigned int>>& cpuIBsIn)
{
    PROJ_ASSERT_W_MSG(cpuIBsIn.size() > 0, "Attempted to set A Graphic Object's cpu's Index Buffer with an empty cpuIB vector");
    cpuIBLoc = cpuIBsIn.size() - 1;
    pcpuIB = &cpuIBsIn.back();
    
}

void GraphicObj::setCpuVB(std::vector<std::vector<float>>& cpuVBsIn, const unsigned int indexIn)
{
    PROJ_ASSERT_W_MSG(cpuVBsIn.size() > indexIn, "Attempted to set A Graphic Object's cpu's Vertex Buffer with an empty cpuVB vector");
    cpuVBLoc = indexIn;
    pcpuVB = &cpuVBsIn[indexIn];
}

void GraphicObj::setCpuIB(std::vector<std::vector<unsigned int>>& cpuIBsIn, const unsigned int indexIn)
{
    PROJ_ASSERT_W_MSG(cpuIBsIn.size() > indexIn, "Attempted to set A Graphic Object's cpu's Index Buffer with an empty cpuIB vector");
    cpuIBLoc = indexIn;
    pcpuIB = &cpuIBsIn[indexIn];
}

void GraphicObj::setVB(const unsigned int& VBLocIn, VertexBuffer* pVBIn)
{
    VBLoc = VBLocIn;
    pVB = pVBIn;
}

void GraphicObj::setIB(const unsigned int& IBLocIn, IndexBuffer* pIBIn)
{
    IBLoc = IBLocIn;
    pIB = pIBIn;
}

void GraphicObj::setVAO(const unsigned int& VAOLocIn, VertexArray* pVAOIn)
{
    VAOLoc = VAOLocIn;
    pVAO = pVAOIn;
}

void GraphicObj::setSP(const unsigned int& SPLocIn, Shader* pSPIn)
{
    SPLoc = SPLocIn;
    pSP = pSPIn;
}

void GraphicObj::setVB(const std::vector<VertexBuffer*>& VBsIn)
{
    PROJ_ASSERT_W_MSG(VBsIn.size() > 0, "Attempted to set A Graphic Object's Vertex Buffer with an empty VB vector");
    VBLoc = VBsIn.size() - 1;
    pVB = VBsIn.back();
}

void GraphicObj::setIB(const std::vector<IndexBuffer*>& IBsIn)
{
    PROJ_ASSERT_W_MSG(IBsIn.size() > 0, "Attempted to set A Graphic Object's Index Buffer with an empty IB vector");
    IBLoc = IBsIn.size() - 1;
    pIB = IBsIn.back();
}

void GraphicObj::setVAO(const std::vector<VertexArray*>& VAOsIn)
{
    PROJ_ASSERT_W_MSG(VAOsIn.size() > 0, "Attempted to set A Graphic Object's Vertex Array with an empty VAO vector");
    VAOLoc = VAOsIn.size() - 1;
    pVAO = VAOsIn.back();
}

void GraphicObj::setSP(const std::vector<Shader*>& SPsIn)
{
    PROJ_ASSERT_W_MSG(SPsIn.size() > 0, "Attempted to set A Graphic Object's Shader Program with an empty SP vector");
    SPLoc = SPsIn.size() - 1;
    pSP = SPsIn.back();
}

void GraphicObj::setVB(const std::vector<VertexBuffer*>& VBsIn, const unsigned int indexIn)
{
    PROJ_ASSERT_W_MSG(VBsIn.size() > indexIn, "Attempted to set A Graphic Object's Vertex Buffer with an invalid VB vector location");
    VBLoc = indexIn;
    pVB = VBsIn[indexIn];
}

void GraphicObj::setIB(const std::vector<IndexBuffer*>& IBsIn, const unsigned int indexIn)
{
    PROJ_ASSERT_W_MSG(IBsIn.size() > indexIn, "Attempted to set A Graphic Object's Index Buffer with an invalid IB vector location");
    IBLoc = indexIn;
    pIB = IBsIn[indexIn];
}

void GraphicObj::setVAO(const std::vector<VertexArray*>& VAOsIn, const unsigned int indexIn)
{
    PROJ_ASSERT_W_MSG(VAOsIn.size() > indexIn, "Attempted to set A Graphic Object's Vertex Array with an invalid VAO vector location");
    VAOLoc = indexIn;
    pVAO = VAOsIn[indexIn];
}

void GraphicObj::setSP(const std::vector<Shader*>& SPsIn, const unsigned int indexIn)
{
    PROJ_ASSERT_W_MSG(SPsIn.size() > indexIn, "Attempted to set A Graphic Object's Shader Program with an invalid SP vector location");
    SPLoc = indexIn;
    pSP = SPsIn[indexIn];
}

VertexBuffer* GraphicObj::getpVB()
{
    // TODO: insert return statement here
    return pVB;
}

IndexBuffer* GraphicObj::getpIB()
{
    // TODO: insert return statement here
    return pIB;
}

VertexArray* GraphicObj::getpVAO()
{
    // TODO: insert return statement here
    return pVAO;
}

Shader* GraphicObj::getpSP()
{
    // TODO: insert return statement here
    return pSP;
}

Texture* GraphicObj::getpTX()
{
    // TODO: insert return statement here
    return pTX;
}

const MathMatRMaj<float>& GraphicObj::viewModelMat() const
{
    return ModelMat;
}

MathMatRMaj<float>& GraphicObj::editModelMat()
{
    modelMatDirtyBit = true;
    return ModelMat;
}
