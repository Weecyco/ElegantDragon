#include "Box.h"

Box::Box(DragonDB& DrgnDB) :
GraphicObj(DrgnDB.GObjs, "Box")
{
    //float VB[] = {
    //        0.0f, 0.0f, 0.0f,
    //        1.0f, 0.0f, 0.0f,
    //        0.0f, 1.0f, 0.0f,
    //        0.0f, 0.0f, 1.0f,
    //        1.0f, 1.0f, 0.0f,
    //        0.0f, 1.0f, 1.0f,
    //        1.0f, 0.0f, 1.0f,
    //        1.0f, 1.0f, 1.0f,
    //};
    //unsigned int IB[] = {
    //    0, 1, 6,
    //    0, 6, 3,
    //    2, 0, 3,
    //    2, 3, 5,
    //    3, 6, 7,
    //    3, 7, 5,
    //    0, 2, 4,
    //    0, 4, 1,
    //    1, 4, 7,
    //    1, 7, 6,
    //    2, 5, 7,
    //    2, 7, 4
    //};
    ////No duplicate  vertex needed  since  the  we havea symetric map
    //float textureMap[] = {
    //    0.0f, 0.0f,
    //    1.0f, 0.0f,
    //    0.0f, 1.0f,
    //    0.0f, 1.0f,
    //    1.0f, 1.0f,
    //    0.0f, 0.0f,
    //    1.0f, 1.0f,
    //    1.0f, 0.0f
    //}; 
    float VB[] = {
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f,

        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,

        0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,

        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f,

        1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
    };
    unsigned int IBbase[] = {
        0, 1, 2,
        2, 3, 0,
    };
    unsigned IB[36];
    for (int i = 0; i < 6; i++)
    {
        IB[6 * i] = IBbase[0] + 4 * i;
        IB[6 * i + 1] = IBbase[1] + 4 * i;
        IB[6 * i + 2] = IBbase[2] + 4 * i;
        IB[6 * i + 3] = IBbase[3] + 4 * i;
        IB[6 * i + 4] = IBbase[4] + 4 * i;
        IB[6 * i + 5] = IBbase[5] + 4 * i;
    }

    //No duplicate  vertex needed  since  the  we havea symetric map
    float textureMap[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
    };

    //Make function to generalize some day
    float VBMerge[120];
    for (int i = 0; i < 24; i++)
    {
        VBMerge[5*i] = VB[3*i];
        VBMerge[5*i + 1] = VB[3*i + 1];
        VBMerge[5*i + 2] = VB[3*i + 2];
        VBMerge[5*i + 3] = textureMap[2*i];
        VBMerge[5*i + 4] = textureMap[2*i + 1];
    }


    DrgnDB.VBs.push_back(new VertexBuffer(VBMerge, 120 * sizeof(float)));
    DrgnDB.VAOs.push_back(new VertexArray());
    VertexBufferLayout Layout;
    Layout.push<float>(3);//3 values per Vertex
    Layout.push<float>(2);//2 values per texture
    DrgnDB.VAOs.back()->addBuffer(*(DrgnDB.VBs.back()), Layout);
    
    DrgnDB.IBs.push_back(new IndexBuffer(IB, 36, GL_STATIC_DRAW));
    DrgnDB.SPs.push_back(new Shader("res/shaders/worldTexMVP.shader"));
    DrgnDB.SPs.back()->bind();
    DrgnDB.SPs.back()->setUniformMatrix4f("MVP", MathMatRMaj<float>(4, 4, 1).getContent(), GL_TRUE);
    DrgnDB.SPs.back()->setUniform1i("u_Texture", 1);
    
    //DrgnDB.SPs.back()->bind();
    setVB(DrgnDB.VBs);
    setIB(DrgnDB.IBs);
    setVAO(DrgnDB.VAOs);
    setSP(DrgnDB.SPs);
    setTX(DrgnDB.TXs, 1);
}

Box::~Box()
{
}
