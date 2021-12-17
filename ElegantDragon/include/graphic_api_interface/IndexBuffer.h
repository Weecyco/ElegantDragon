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

    /// <summary>
    /// Sets Index Buffer to count number of index values with data
    /// </summary>
    /// <param name="data">Input data</param>
    /// <param name="count">index value count</param>
    /// <param name="drawType">sets the OpenGL draw hint for optimization</param>
    void setData(const void* data, const unsigned int& count, const unsigned short& drawType = GL_STATIC_DRAW);

    /// <summary>
    /// Binds and Sets Index Buffer to count number of index values with data
    /// </summary>
    /// <param name="data">Input data</param>
    /// <param name="count">index value count</param>
    /// <param name="drawType">sets the OpenGL draw hint for optimization</param>
    void setDataB(const void* data, const unsigned int& count, const unsigned short& drawType = GL_STATIC_DRAW);

    /// <summary>
    /// Edits Index Buffer at offset for count number of index values with data
    /// </summary>
    /// <param name="data">Input data</param>
    /// <param name="offset">Buffer offset</param>
    /// <param name="count">index value count</param>
    void editData(const void* data, const unsigned int& offset , const unsigned int& count);

    /// <summary>
    /// Binds and Edits Index Buffer at offset for count number of index values with data
    /// </summary>
    /// <param name="data">Input data</param>
    /// <param name="offset">Buffer offset</param>
    /// <param name="count">index value count</param>
    void editDataB(const void* data, const unsigned int& offset, const unsigned int& count);
    void bind() const;
    static void unbind();

    inline unsigned int getID() const { return renderID; }
    inline unsigned int getCount() const { return count; }
};