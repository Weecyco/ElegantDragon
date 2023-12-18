#include "DragonDB.h"
#include "GOs/dragon.h"

char DragonDB::graphicsSetup()
{
    //RenderBlock setup
    new quickAccessRenderBlock(RenderBlocks);//For GUI
    new quickAccessRenderBlock(RenderBlocks);//For World
    std::cout << RenderBlocks[1]->viewMat(3) << std::endl;
    //set default matricies
    {
        //float screenSpace[][2] = { -1.0f, -0.8f, 0.0f, 0.8f, static_cast<float>(resolution.first) / 2, static_cast<float>(resolution.second) * 0.6 };
        float screenSpace[][2] = { -1.0f, -1.0f, 1.0f, 1.0f, static_cast<float>(resolution.first), static_cast<float>(resolution.second)};
        RenderBlocks[1]->setOrthoProj(3, screenSpace, quickAccessRenderBlock::OrthoScalePreset::VerticalIs1, 0, 10);
        RenderBlocks[0]->setOrthoProj(3, screenSpace, quickAccessRenderBlock::OrthoScalePreset::VerticalIs1, -1, 1);
        RenderBlocks[1]->setPerspProj(3, screenSpace, quickAccessRenderBlock::PerspScalePreset::VerticalFOV, 0.2, 100, 3.0f * PI / 2.0f);
    }
    /*RenderBlocks[1]->editMat(1)[1][1] = std::cosf(PI / 3.0f);
    RenderBlocks[1]->editMat(1)[1][2] = -std::sinf(PI / 3.0f);
    RenderBlocks[1]->editMat(1)[2][1] = std::sinf(PI / 3.0f);
    RenderBlocks[1]->editMat(1)[2][2] = std::cosf(PI / 3.0f);*/
    std::cout << RenderBlocks[1]->viewMat(3) << std::endl;
    RenderBlocks[1]->setScaleRatio2(2, 1.0f);
    std::cout << RenderBlocks[1]->viewMat(2) << std::endl;
    RenderBlocks[1]->setTran(0, 0.0f, 0.0f, 0.0f);
    std::cout << RenderBlocks[1]->viewMat(0) << std::endl;
    //add more config stuff later


    //Load textures:
    //idx 0
    TXs.push_back(new Texture("res/textures/dragon_textures/Dragon_Corner_Texture-Small_Dark_NoBack.png", 0));//Qinni-Final_WIP_redo_Sketch_Colour_Unfinished.png
    //idx 1
    TXs.push_back(new  Texture("res/textures/test_arena_textures/shaded_frame.png", 1));
    //TXs.back()->bind(1);

    //define all GO's
    
    //manual definitions
    //May want to add unload functionality later to save memory when unused.

    //TODO: re-factor this stuff into into more usable functions to reduce redundancy
    //TODO: separate manual definition from automatic ones once we (I) start implementing imports
    //default invalid texture
    {
        float VB[] = {
            0.0,       0.0,
            0.0,       1.0 / 2.0,
            1.0 / 2.0, 0.0,
            1.0 / 2.0, 1.0 / 2.0,
            1.0,       1.0 / 2.0,
            1.0 / 2.0, 1.0,
            1.0,       1.0
        };
        unsigned int IB[] = {
            0, 1, 2,
            1, 2, 3,
            3, 4, 5,
            4, 5, 6,
        };
        VBs.push_back(new VertexBuffer(VB, 14 * sizeof(unsigned int)));
        VAOs.push_back(new VertexArray());
        VertexBufferLayout Layout;
        Layout.push<float>(2);
        VAOs.back()->addBuffer(*(VBs.back()), Layout);

        IBs.push_back(new IndexBuffer(IB, 12));
        SPs.push_back(new Shader("res/shaders/testSolid.shader"));
        SPs.back()->bind();
        //set using Uniform to set colour
        SPs.back()->setUniform4f("u_Color", 0.5, 0.5, 0.5, 0.5);

        //auto increments GObjs in the constructor
        new GraphicObj(GObjs, "Invalid_GO");
        //GObjs.back().setVB(VBs.size() - 1, VBs.back()); //if you don't want to use "set__LastOf" shortcut 
        
        //defaults to using last item in the vector
        GObjs.back()->setVertexCount(7);
        GObjs.back()->setVB(VBs);
        GObjs.back()->setIB(IBs);
        GObjs.back()->setVAO(VAOs);
        GObjs.back()->setSP(SPs);
        GObjs.back()->setTX(TXs, 0);

        //old way of initializing
        /*new GraphicObj(
            GObjs,
            VBs.size() - 1,
            IBs.size() - 1,
            VAOs.size() - 1,
            ShProgs.size() - 1,
            "Invalid_GO");*/
    }

    //Dragon turn
    cpuVBs.push_back(std::vector<float>{
        1.0 / 3.0, 0.0      ,
        1.0 / 3.0, 1.0 / 3.0,
        2.0 / 3.0, 2.0 / 3.0,
        1.0      , 2.0 / 3.0,
        1.0      , 1.0 / 3.0,

        1.0 - INNER_VERTEX_DISTANCE_THIRD, 1.0 / 3.0,

        2.0 / 3.0, INNER_VERTEX_DISTANCE_THIRD,
        2.0 / 3.0, 0.0
    });

    cpuIBs.push_back(std::vector<unsigned int>{
        0, 1, 7,
        1, 6, 7,
        1, 2, 6,
        2, 5, 6,
        2, 3, 5,
        3, 4, 5
        });

    //Initiallize the GPU side VB and IB to the default values
    std::cout << cpuVBs.back().size() << std::endl;
    VBs.push_back(new VertexBuffer(cpuVBs.back().data(), cpuVBs.back().size() * sizeof(float)));
    VAOs.push_back(new VertexArray());
    VertexBufferLayout Layout;
    Layout.push<float>(2);
    VAOs.back()->addBuffer(*(VBs.back()), Layout);

    IBs.push_back(new IndexBuffer(cpuIBs.back().data(), cpuIBs.back().size(), GL_DYNAMIC_DRAW));
    SPs.push_back(new Shader("res/shaders/testSolid.shader"));
    SPs.back()->bind();
    //set using Uniform to set colour
    SPs.back()->setUniform4f("u_Color", 1, 1, 1, 1);

    //load GO's into object's default
    Drgn::defaultGO = GraphicObj("Dragon_Turn"); // made a boo boo... the generated GO is copied over to Drgn::defaultGO before being deallocated so the one in GObjs is now invalid... ignore for now...
    Drgn::defaultGO.setVertexCount(cpuVBs.back().size()/2);
    Drgn::defaultGO.setCpuVB(cpuVBs);
    Drgn::defaultGO.setCpuIB(cpuIBs);
    Drgn::defaultGO.setVB(VBs);
    Drgn::defaultGO.setIB(IBs);
    Drgn::defaultGO.setVAO(VAOs);
    Drgn::defaultGO.setSP(SPs);
    /*Drgn::defaultGO = GraphicObj(
        GObjs,
        VBs.size() - 1,
        IBs.size() - 1,
        VAOs.size() - 1,
        ShProgs.size() - 1,
        "Dragon_Turn");
    Drgn::defaultGO.cpuVBLoc = cpuVBs.size() - 1;
    Drgn::defaultGO.pcpuVB = &cpuVBs.back();
    Drgn::defaultGO.cpuIBLoc = cpuIBs.size() - 1;
    Drgn::defaultGO.pcpuIB = &cpuIBs.back();
    Drgn::defaultGO.pVB = VBs.back();
    Drgn::defaultGO.pIB = IBs.back();
    Drgn::defaultGO.pVAO = VAOs.back();
    Drgn::defaultGO.pSP = ShProgs.back();*/

    //Simple unit box object
    /*{
        float VB[] = {
            0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
        };
        float IB[] = {
            0, 1, 6,
            0, 6, 3,
            2, 0, 3,
            2, 3, 5,
            3, 6, 7,
            3, 7, 5,
            0, 2, 4,
            0, 4, 1,
            1, 4, 7,
            1, 7, 6,
            2, 5, 7,
            2, 7, 4
        };

    }*/

    return 0;
}