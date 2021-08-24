#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
    unsigned int m_renderVAID;
    const VertexBuffer* lastBuffer = nullptr;
    const VertexBufferLayout* lastLayout = nullptr;
public:
    VertexArray();
    VertexArray(const VertexBuffer& VB, const VertexBufferLayout& Layout);
    ~VertexArray();

    void bind() const;
    static void unbind();
    void addBuffer(const VertexBuffer& VB, const VertexBufferLayout& Layout);
};