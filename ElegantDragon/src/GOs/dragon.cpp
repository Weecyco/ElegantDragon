#include "dragon.h"
#include "DragonDB.h"

#include "projectDefines.h"
#include "Renderer.h"
#include "MathDrgn.h"

#include <iostream>

GraphicObj Drgn::defaultGO;
Drgn::Drgn(DragonDB& DrgnDB, unsigned int lenIn, const DrgnType typeIn) :
    GraphicObj(defaultGO, DrgnDB.GObjs), //write a proper copy constructor if necessary
    location(MathVec<float>(2))
{
    //make kind of a copy GO constructor for this later
    //DrgnDB.GObjs.push_back(this);
    //insertion into the render layer data structures should be separate

    len = lenIn;
    curLen = len;
    turns = new char[static_cast<unsigned>(len / 8) + (len % 8 > 0 ? 1 : 0)];
    for (int i = 0; i < static_cast<unsigned>(len / 8) + (len % 8 > 0 ? 1 : 0); i++)
        turns[i] = 0;

    unsigned int index = 1;
    PROJ_ASSERT_W_MSG(!dragonRecur(index), "Faked dragon!!");


    //NOTE: Vertex count for this obj is just the cpu VB vertex count (single turn)

    //depending on the dragon type
    //create new unique VB and VAO for itself probably do IB if needed
    //create new ShProg for itself
    drgnType = typeIn;
    VertexBufferLayout Layout;
    float* drgnVert = nullptr;
    unsigned int* drgnIdx = nullptr;
    switch (typeIn)
    {
    case DrgnType::SolidCpuRend:
        name = "Solid_Cpu_Turn_Dragon";
        DrgnDB.VBs.push_back(new VertexBuffer(defaultGO.pcpuVB->data(), defaultGO.pcpuVB->size() * sizeof(float)));
        DrgnDB.VAOs.push_back(new VertexArray());
        
        //set layouts
        Layout.push<float>(2);
        
        DrgnDB.SPs.push_back(new Shader("res/shaders/testSolid.shader"));
        DrgnDB.SPs.back()->bind();
        DrgnDB.SPs.back()->setUniform4f("u_Color", 1, 1, 1, 1); //just as default
        break;
    case DrgnType::SolidShaderTransform:
        name = "Solid_ShaderTransform_Dragon";
        //generates the full list of vert and idx buffers based on the len and the predefined values in the cpuVB & cpuIB
        //IMPORTANT cpu VB and IB MUST be already set to what you want at this point
        drgnVert = generateDragonVert(len);
        drgnIdx = generateDragonIdx(len); //set to 0 if moving the generation to be built into length resizer

        DrgnDB.VBs.push_back(new VertexBuffer(drgnVert, len * defaultGO.pcpuVB->size() * sizeof(float)));
        DrgnDB.VAOs.push_back(new VertexArray());

        DrgnDB.IBs.push_back(new IndexBuffer(drgnIdx, len * defaultGO.pcpuIB->size(), GL_DYNAMIC_DRAW)); //loads new IB to the Data base
        setIB(DrgnDB.IBs); // assigns the new IB to this object

        //set layouts
        Layout.push<float>(2);

        DrgnDB.SPs.push_back(new Shader("res/shaders/worldSolGPUTrans.shader"));
        DrgnDB.SPs.back()->bind();
        DrgnDB.SPs.back()->setUniform2f("u_camLocation", DrgnDB.camLoc[0], DrgnDB.camLoc[1]);
        DrgnDB.SPs.back()->setUniform1f("u_camScale", DrgnDB.camScaling);
        DrgnDB.SPs.back()->setUniform2f("u_screenScale", DrgnDB.xScale, DrgnDB.yScale);
        DrgnDB.SPs.back()->setUniform4f("u_Color", 1, 1, 1, 1); //just as default

        break;
    case DrgnType::Style0ShaderTransform:
    {
        name = "Style0_ShaderTransform_Dragon";
        /*DrgnDB.cpuVBs.push_back(std::vector<float>{
                1.0 / 3.0,                          0.0      ,                      0.0,                                    0.0,
                1.0 / 3.0,                          1.0 / 3.0,                      0.0,                                    0.5,
                2.0 / 3.0,                          2.0 / 3.0,                      0.5,                                    1,
                1.0,                                2.0 / 3.0,                      1,                                      1,
                1.0,                                1.0 / 3.0,                      1,                                      0.5,
                1.0 - INNER_VERTEX_DISTANCE_THIRD,  1.0 / 3.0,                      1 - (INNER_VERTEX_DISTANCE_THIRD*3/2),  0.5,
                2.0 / 3.0,                          INNER_VERTEX_DISTANCE_THIRD,    0.5,                                    INNER_VERTEX_DISTANCE_THIRD * 3 / 2,
                2.0 / 3.0,                          0.0,                            0.5,                                    0.0
        });
        setCpuVB(DrgnDB.cpuVBs);*/
        float textureMap[] = {
                0.0,                                    0.0,
                0.0,                                    0.5,
                0.5,                                    1,
                1,                                      1,
                1,                                      0.5,
                1 - (INNER_VERTEX_DISTANCE_THIRD * 3 / 2),  0.5,
                0.5,                                    INNER_VERTEX_DISTANCE_THIRD * 3 / 2,
                0.5,                                    0.0
        };

        drgnVert = generateDragonVert(len);
        drgnVert = appendTextureMap(drgnVert, textureMap, len);
        /*std::cout << "\n\npost function" << drgnVert << "\n";
        for (int i = 0; i < 100; i++)
        {
            std::cout << drgnVert[i] << " ";
        }*/

        drgnIdx = generateDragonIdx(len); //set to 0 if moving the generation to be built into length resizer

        std::cout << "VB total size: " << len * vertexCount * 4 << std::endl;
        /*std::cout << "\n\n";
        for (int i = 0; i < 100; i++)
        {
            std::cout << drgnVert[i] << " ";
        }
        std::cout << "\n\n";
        for (int i = 0; i < 100; i++)
        {
            std::cout << drgnIdx[i] << " ";
        }
        std::cout << "\n\n";*/
        DrgnDB.VBs.push_back(new VertexBuffer(drgnVert, len * vertexCount * 4 * sizeof(float)));
        DrgnDB.VAOs.push_back(new VertexArray());

        DrgnDB.IBs.push_back(new IndexBuffer(drgnIdx, len * defaultGO.pcpuIB->size(), GL_DYNAMIC_DRAW)); //loads new IB to the Data base
        setIB(DrgnDB.IBs); // assigns the new IB to this object

        //set layouts
        Layout.push<float>(2); //location 0
        Layout.push<float>(2); //location 1

        DrgnDB.SPs.push_back(new Shader("res/shaders/worldTexGPUTrans.shader"));
        DrgnDB.SPs.back()->bind();
        DrgnDB.SPs.back()->setUniform2f("u_camLocation", DrgnDB.camLoc[0], DrgnDB.camLoc[1]); //hard code location offest for now
        DrgnDB.SPs.back()->setUniform1f("u_camScale", DrgnDB.camScaling);
        DrgnDB.SPs.back()->setUniform2f("u_screenScale", DrgnDB.xScale, DrgnDB.yScale);
        DrgnDB.SPs.back()->setUniform1i("u_Texture", 0);//hard code to 0 temporarily
    }
        break;
    case DrgnType::SolidMVP:
        name = "Solid_MVP_Dragon";
        //generates the full list of vert and idx buffers based on the len and the predefined values in the cpuVB & cpuIB
        //IMPORTANT cpu VB and IB MUST be already set to what you want at this point
        drgnVert = generateDragonVert(len);
        drgnIdx = generateDragonIdx(len); //set to 0 if moving the generation to be built into length resizer

        DrgnDB.VBs.push_back(new VertexBuffer(drgnVert, len * defaultGO.pcpuVB->size() * sizeof(float)));
        DrgnDB.VAOs.push_back(new VertexArray());

        DrgnDB.IBs.push_back(new IndexBuffer(drgnIdx, len * defaultGO.pcpuIB->size(), GL_DYNAMIC_DRAW)); //loads new IB to the Data base
        setIB(DrgnDB.IBs); // assigns the new IB to this object

        //set layouts
        Layout.push<float>(2);

        DrgnDB.SPs.push_back(new Shader("res/shaders/worldSolMVP.shader"));
        DrgnDB.SPs.back()->bind();
        DrgnDB.SPs.back()->setUniformMatrix4f("MVP", MathMatRMaj<float>(4, 4, 1).getContent(), GL_TRUE); //Not very efficient way of creating an identity matrix, but meh
        DrgnDB.SPs.back()->setUniform4f("u_Color", 1, 1, 1, 1); //just as default

        break;
    case DrgnType::Style0MVP:
    {
        name = "Style0_MVP_Dragon";
        float textureMap[] = {
                0.0,                                    0.0,
                0.0,                                    0.5,
                0.5,                                    1,
                1,                                      1,
                1,                                      0.5,
                1 - (INNER_VERTEX_DISTANCE_THIRD * 3 / 2),  0.5,
                0.5,                                    INNER_VERTEX_DISTANCE_THIRD * 3 / 2,
                0.5,                                    0.0
        };

        drgnVert = generateDragonVert(len);
        drgnVert = appendTextureMap(drgnVert, textureMap, len);

        drgnIdx = generateDragonIdx(len); //set to 0 if moving the generation to be built into length resizer

        DrgnDB.VBs.push_back(new VertexBuffer(drgnVert, len * vertexCount * 4 * sizeof(float)));
        DrgnDB.VAOs.push_back(new VertexArray());

        DrgnDB.IBs.push_back(new IndexBuffer(drgnIdx, len * defaultGO.pcpuIB->size(), GL_DYNAMIC_DRAW)); //loads new IB to the Data base
        setIB(DrgnDB.IBs); // assigns the new IB to this object

        //set layouts
        Layout.push<float>(2); //location 0
        Layout.push<float>(2); //location 1

        DrgnDB.SPs.push_back(new Shader("res/shaders/worldTexMVP.shader"));
        DrgnDB.SPs.back()->bind();
        DrgnDB.SPs.back()->setUniformMatrix4f("MVP", MathMatRMaj<float>(4, 4, 1).getContent(), GL_TRUE);
        DrgnDB.SPs.back()->setUniform1i("u_Texture", 0);//hard code to 0 temporarily
    }
        break;

    default:
        PROJ_ASSERT_W_MSG(1, "Unknown dragon class type")
    }
    //apply layout and buffer to VAO
    DrgnDB.VAOs.back()->addBuffer(*(DrgnDB.VBs.back()), Layout);
    
    //Set internal references
    setVB(DrgnDB.VBs);
    setVAO(DrgnDB.VAOs);
    setSP(DrgnDB.SPs);
    /*pVB = DrgnDB.VBs.back();
    VBLoc = DrgnDB.VBs.size() - 1;
    pVAO = DrgnDB.VAOs.back();
    VAOLoc = DrgnDB.VAOs.size() - 1;
    pSP = DrgnDB.SPs.back();
    SPLoc = DrgnDB.SPs.size() - 1;*/

    //std::cout << defaultGO.name;
    //clean up temp arrays
    if (drgnVert)
        delete[] drgnVert;
    if (drgnIdx)
        delete[] drgnIdx;
}

Drgn::Drgn(const Drgn& src) :
    GraphicObj(src), location(src.location)
{
    drgnType = src.drgnType;
    len = src.len;
    curLen = src.curLen;
    
    unsigned int maxMajorIdx = static_cast<unsigned int>((len - 1) / 8);
    turns = new char[maxMajorIdx + 1];
    for (unsigned int majorIdx = 0; majorIdx < maxMajorIdx + 1; ++majorIdx)
        turns[majorIdx] = src.turns[majorIdx];
        
}

Drgn::~Drgn()
{
    delete[] turns;
}

unsigned Drgn::dragonRecur(unsigned& index)
{
    if (index >= len)
        return 0;
    if (index < 1) return 1;
    for (index; index < len; index++)
    {
        unsigned shift = index;
        while ((shift & 1) == 0)
            shift >>= 1;
        //std::cout << "shift = " << shift << " turn = " << ((shift & 2) >> 1) << std::endl;
        turns[static_cast<unsigned>(index / 8)] |= (((shift & 2) >> 1) << (index % 8));
    }
    return 0;
}

float* Drgn::generateDragonVert(const unsigned int& lenIn)
{
    PROJ_ASSERT_W_MSG(lenIn <= len, "Cannot generate dragon vertex length greater than the dragon length")
    MathVec<float> Dir(2);
    Dir[1] = 1;
    MathVec<float> Loc(2);
    Loc[0] = location[0];//initialize base location to the desired location
    Loc[1] = location[1];

    bool lastTurn = 0; //let 0=right 1=left (initialize to default image mapping)
    //quite inefficient to recalculate the vertices when there are only 4 possibilities (or 8 if not dependent on direction)
    //but who cases lol. well it is more efficent on memory at least err by a bit

    // initialize vertex locations to the desired
    float* vertexBuffMap = new float[pcpuVB->size()]; //turn specific
    float* vertexBuffMapFull = new float[lenIn * pcpuVB->size()]; //full image
    for (unsigned int i = 0; i < pcpuVB->size(); ++i)
    {
        vertexBuffMap[i] = (*pcpuVB)[i] + (i % 2 ? location[1] : location[0]);
    }
    for (unsigned int i = 0; i < lenIn * pcpuVB->size(); ++i)
    {
        vertexBuffMapFull[i] = 5.0;
    }

    for (unsigned int drgnIdx = 0; drgnIdx < lenIn; ++drgnIdx)
    {
        //if (drgnIdx == 114) __debugbreak();
        //only check turn. it is prerotated and transposed
        if ((*this)[drgnIdx] != lastTurn)
        {
            //use generalized mirror off the direction Dir once available
            for (unsigned int i = 0; i < pcpuVB->size(); i += 2)
            {
                if (std::abs(Dir[0]) < 0.5)
                    vertexBuffMap[i] = (vertexBuffMap[i] - (Loc[0] + 0.5)) * -1 + (Loc[0] + 0.5);
                else if (std::abs(Dir[1]) < 0.5)
                    vertexBuffMap[i + 1] = (vertexBuffMap[i + 1] - (Loc[1] + 0.5)) * -1 + (Loc[1] + 0.5);
            }
        }

        //************************************************
        // 
        //update locations and draw
        /*float* bufferData = DrgnDB.worldToScreen(vertexBuffMap, pcpuVB->size());
        pVB->setDataBind(bufferData, sizeof(float) * pcpuVB->size());
        DrgnDB.rend.draw(*pVAO, *pIB, *pSP);
        delete[] bufferData;*/

        //Update full dragon vertex array with the turn specific buffer after its transpose and mirrors
        for (unsigned int turnBufferIdx = 0; turnBufferIdx < pcpuVB->size(); ++turnBufferIdx)
        {
            vertexBuffMapFull[(drgnIdx * pcpuVB->size()) + turnBufferIdx] = vertexBuffMap[turnBufferIdx];
        }

        //rotate since we're now facing a different direction
        float rotation = 0;
        switch ((*this)[drgnIdx])
        {
        case 0:
            rotation = -PI / 2;
            break;
        case 1:
            rotation = PI / 2;
            break;
        default:
            PROJ_ASSERT_W_MSG(0, "Invalid rotation value return");
        }
        for (unsigned int i = 0; i + 1 < pcpuVB->size(); i += 2)
        {
            MathVec<float>::rotate2(rotation, vertexBuffMap[i], vertexBuffMap[i + 1], Loc[0] + 0.5, Loc[1] + 0.5);
        }
        //adjust pretence vectors to prepare for next one
        Dir.rotate(rotation);

        //have vertexBuffMap reference Loc later it will make it cleaner. 
        //probably have a special function to generate the final locations with one transpose too
        for (unsigned int i = 0; i < pcpuVB->size(); ++i)
        {
            vertexBuffMap[i] += Dir[i % 2];
        }
        Loc[0] = Loc[0] + Dir[0];
        Loc[1] = Loc[1] + Dir[1];


        lastTurn = (*this)[drgnIdx];
    }
    /*std::cout << "[VB VALUES LIST]" << std::endl;
    for (unsigned int turnIdx = 0; turnIdx < lenIn; ++turnIdx)
    {
        std::cout << "Turn " << turnIdx << ": ";
        for (unsigned int elemIdx = 0; elemIdx < pcpuVB->size(); ++elemIdx)
        {
            std::cout << "#" << elemIdx << "=" << vertexBuffMapFull[turnIdx * pcpuVB->size() + elemIdx] << " ";
        }
        std::cout << std::endl;
    }*/

    delete[] vertexBuffMap;
    return vertexBuffMapFull;
}

float* Drgn::appendTextureMap(float* drgnVert, const float* textureMap, const unsigned int& lenIn)
{
    float* rawDrgnVert = drgnVert;
    drgnVert = new float[lenIn * vertexCount * 4];
    for (unsigned int vertIdx = 0; vertIdx < lenIn * vertexCount * 4; vertIdx += 4)
    {
        drgnVert[vertIdx] = rawDrgnVert[vertIdx / 2];
        drgnVert[vertIdx + 1] = rawDrgnVert[vertIdx / 2 + 1];
        drgnVert[vertIdx + 2] = textureMap[(vertIdx / 2) % (2 * vertexCount)];
        drgnVert[vertIdx + 3] = textureMap[(vertIdx / 2 + 1) % (2 * vertexCount)];
    }
    //std::cout << "\n\npre function" << drgnVert << "\n";
    //for (unsigned int vertIdx = 0; vertIdx < 100; vertIdx++)
    //{
    //    std::cout << drgnVert[vertIdx] << " ";
    //}
    //std::cout << std::endl;
    delete[] rawDrgnVert;
    return drgnVert;
}

unsigned int* Drgn::generateDragonIdx(const unsigned int& lenIn)
{
    PROJ_ASSERT_W_MSG(!(pcpuVB->size() % 2), "Non-even VB buffer. Cannot generate Dragon Indices")
    unsigned int* indexBuffMapFull = new unsigned int[lenIn * pcpuIB->size()];
    for (unsigned int turnIdx = 0; turnIdx < lenIn; ++turnIdx)
    {
        for (unsigned int elemIdx = 0; elemIdx < pcpuIB->size(); ++elemIdx)
        {
            indexBuffMapFull[turnIdx * pcpuIB->size() + elemIdx] = (*pcpuIB)[elemIdx] + (vertexCount * turnIdx);
        }
    }
    /*std::cout << "[IB VALUES LIST]" << std::endl;
    for (unsigned int turnIdx = 0; turnIdx < lenIn; ++turnIdx)
    {
        std::cout << "Turn " << turnIdx << ": ";
        for (unsigned int elemIdx = 0; elemIdx < pcpuIB->size(); ++elemIdx)
        {
            std::cout << "#" << elemIdx << "=" << indexBuffMapFull[turnIdx * pcpuIB->size() + elemIdx] << " ";
        }
        std::cout << std::endl;
    }*/
    return indexBuffMapFull;
}

void Drgn::printTurns()
{
    unsigned indexMax = static_cast<unsigned>(len / 8) + (len % 8 > 0 ? 1 : 0);

    for (int i = 0; i < indexMax; ++i)
    {
        char innerMax = 8;
        if (i == indexMax - 1)
        {
            innerMax = len % 8;
            if (innerMax == 0) innerMax = 8;
        }

        for (int inner = 0; inner < innerMax; ++inner)
            std::cout << ((turns[i] >> inner) & 1);
        std::cout << "\n";
    }
}

int Drgn::drawDragon(DragonDB& DrgnDB)
{
    MathVec<float> Dir(2);
    Dir[1] = 1;
    MathVec<float> Loc(2);
    Loc[0] = location[0];//initialize base location to the desired location
    Loc[1] = location[1];
    
    bool lastTurn = 0; //let 0=right 1=left (initialize to default image mapping)
    //quite inefficient to recalculate the vertices when there are only 4 possibilities (or 8 if not dependent on direction)
    //but who cases lol. well it is more efficent on memory at least err by a bit

    // initialize vertex locations to the desired
    float* vertexBuffMap = new float[pcpuVB->size()];
    for (unsigned int i = 0; i < pcpuVB->size(); ++i)
    {
        vertexBuffMap[i] = (*pcpuVB)[i] + (i % 2 ? location[1] : location[0]);
    }

    for (unsigned int drgnIdx = 0; drgnIdx < curLen; ++drgnIdx)
    {
        //only check turn. it is prerotated and transposed
        if ((*this)[drgnIdx] != lastTurn)
        {
            //use generalized mirror off the direction Dir once available
            for (unsigned int i = 0; i < pcpuVB->size(); i += 2)
            {
                if (std::abs(Dir[0]) < 0.5)
                    vertexBuffMap[i] = (vertexBuffMap[i] - (Loc[0] + 0.5)) * -1 + (Loc[0] + 0.5);
                else if (std::abs(Dir[1]) < 0.5)
                    vertexBuffMap[i + 1] = (vertexBuffMap[i + 1] - (Loc[1] + 0.5)) * -1 + (Loc[1] + 0.5);
            }
        }

        //update locations and draw
        float* bufferData = DrgnDB.worldToScreen(vertexBuffMap, pcpuVB->size());
        pVB->setDataB(bufferData, sizeof(float) * pcpuVB->size());
        DrgnDB.rend.draw(*pVAO, *pIB, *pSP);
        delete[] bufferData;

        //rotate since we're now facing a different direction
        float rotation = 0;
        /*if ((*this)[drgnIdx] == 0)
        {
            rotation = -PI / 2;
        }
        else
        {
            rotation = PI / 2;
        }*/
        switch ((*this)[drgnIdx])
        {
        case 0:
            rotation = -PI / 2;
            break;
        case 1:
            rotation = PI / 2;
            break;
        default:
            PROJ_ASSERT_W_MSG(0, "Invalid rotation value return");
        }
        for (unsigned int i = 0; i + 1 < pcpuVB->size(); i += 2)
        {
            MathVec<float>::rotate2(rotation, vertexBuffMap[i], vertexBuffMap[i + 1], Loc[0] + 0.5, Loc[1] + 0.5);
        }
        //adjust pretence vectors to prepare for next one
        Dir.rotate(rotation);

        //have vertexBuffMap reference Loc later it will make it cleaner. 
        //probably have a special function to generate the final locations with one transpose too
        for (unsigned int i = 0; i < pcpuVB->size(); ++i)
        {
            vertexBuffMap[i] += Dir[i % 2];
        }
        Loc[0] = Loc[0] + Dir[0];
        Loc[1] = Loc[1] + Dir[1];
        

        lastTurn = (*this)[drgnIdx];
    }

    delete[] vertexBuffMap;
    return 0;
}

void Drgn::setCurLen(const unsigned int curLenIn) {
    PROJ_ASSERT(curLenIn <= len)

    const unsigned int preLen = curLen;
    if (curLen == curLenIn) return;
    curLen = curLenIn;

    switch (drgnType)
    {
    case DrgnType::SolidShaderTransform:
    case DrgnType::Style0ShaderTransform: //update if necessary for Style 0
    case DrgnType::SolidMVP:
    case DrgnType::Style0MVP:
    {
         //DONE --TODO--: \
        Make run once with full length and then reference. \
        I'm checking the diff now to either copy over or set to 0 so not having the full one don't matter \
        Saves a bit of time to not have to regenerate the index \
        PREFERED ALTERNATIVE: \
        Remove "generateDragonIdx" and have the indices generated here (not too much overhead anyways)


        //------------for lazer show-----------------------//
        // unsigned int* drgnIdx = generateDragonIdx(curLen);
        //pIB->setData(drgnIdx, curLen * pcpuIB->size(), GL_STREAM_DRAW, true);
        //delete[] drgnIdx;
        // 
        //------------for normal show-----------------------//
        unsigned int* diffDrgnIdx;
        if (curLen > preLen)
        { 
            diffDrgnIdx = new unsigned int[(curLen - preLen) * pcpuIB->size()];
            //get the indices from preLen (preLen*pcpuIB->size()) to curLen (curLen*pcpuIB->size()) via drgnIdx
            for (unsigned int index = 0; index < (curLen - preLen) * pcpuIB->size(); ++index)
                diffDrgnIdx[index] = (*pcpuIB)[index % pcpuIB->size()] + vertexCount * (preLen + static_cast<unsigned int>(index / pcpuIB->size()));

            pIB->editDataB(diffDrgnIdx, preLen * pcpuIB->size(), (curLen - preLen) * pcpuIB->size());
        }
        else
        {
            diffDrgnIdx = new unsigned int[(preLen - curLen) * pcpuIB->size()];
            //get the indices from preLen (preLen*pcpuIB->size()) to curLen (curLen*pcpuIB->size()) via drgnIdx
            for (unsigned int index = 0; index < (preLen - curLen) * pcpuIB->size(); ++index)
                diffDrgnIdx[index] = 0;

            pIB->editDataB(diffDrgnIdx, curLen * pcpuIB->size(), (preLen - curLen) * pcpuIB->size());
        }

        
        //std::cout << "test print to slow down" << "mmooo" << std::endl;
        delete[] diffDrgnIdx;
    }
    break;
    default:
        break;
    }
}

int Drgn::setLocation(float x, float y)
{
    location[0] = x;
    location[1] = y;
    return 0;
}

int Drgn::setLocation(MathVec<float> src)
{
    if (src.getSize() != 2) {
        std::cout << "[WARNING] Incorrect location dimention provided" << std::endl;
        PROJ_ASSERT(false);
        return -1;
    }
    location = src;

    return 0;
}

bool Drgn::operator[](const unsigned int drgnIdx) const
{
    //make sure it doesn't index out-of-bounds
    PROJ_ASSERT_W_MSG(drgnIdx < len, "Drgn index access out of bounds");

    //finds the major indexs it needs to access the char's location and gets the remainder to access the bit
    unsigned int majorIndex = static_cast<unsigned int>(drgnIdx / 8);
    
    return ((turns[majorIndex] >> drgnIdx % 8) & 1);
}

