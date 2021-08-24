#pragma once
#include "gl/glew.h"
#include "graphicUtilities.h"

//Note: our ib and vb does not contain the draw poll since it will be handled by the renderer

class VertexBuffer
{
private:
    unsigned int renderID;
public:
    VertexBuffer();
    VertexBuffer(const void* data, unsigned int size, const unsigned short& drawType = GL_STATIC_DRAW);
    ~VertexBuffer();

    void setData(const void* data, unsigned int size, const unsigned short& drawType = GL_STATIC_DRAW);
    void setDataB(const void* data, unsigned int size, const unsigned short& drawType = GL_STATIC_DRAW);
    void bind() const;
    static void unbind();

    unsigned int getID() { return renderID; }
};