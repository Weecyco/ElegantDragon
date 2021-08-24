#pragma once
#include "gl/glew.h"
#include "graphicUtilities.h"

//Note: our ib and vb does not contain the draw poll since it will be handled by the renderer

class IndexBuffer
{
private:
    unsigned int renderID;
    unsigned int count;
public:
    IndexBuffer();
    IndexBuffer(const unsigned int* data, unsigned int count, const unsigned short& drawType = GL_STATIC_DRAW);
    ~IndexBuffer();

    void setData(const void* data, const unsigned int& count, const unsigned short& drawType = GL_STATIC_DRAW);
    void setDataB(const void* data, const unsigned int& count, const unsigned short& drawType = GL_STATIC_DRAW);
    void editData(const void* data, const unsigned int& offset , const unsigned int& count);
    void editDataB(const void* data, const unsigned int& offset, const unsigned int& count);
    void bind() const;
    static void unbind();

    inline unsigned int getID() const { return renderID; }
    inline unsigned int getCount() const { return count; }
};